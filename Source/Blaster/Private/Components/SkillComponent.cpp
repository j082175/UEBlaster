// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent.h"
#include "Characters/CharacterBase.h"
#include "Actor/HealArea.h"
#include "Actor/ShieldBarrier.h"
#include "Characters/Enemy/EnemyRange.h"
#include "Net/UnrealNetwork.h"
#include "Animation/WidgetAnimation.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/InventoryComponent.h"
#include "Item/Pickable/Weapon/Weapon_Gun.h"
#include "Blaster.h"

// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
	InitializeCoolTimeMap();

	UltimateEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("UltimateEffect"));
}


// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...


	CharacterOwner = Cast<ACharacterBase>(GetOwner());

	CharacterOwner->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ThisClass::OnPlayMontageNotifyBeginFunc);

	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
	UltimateEffectComponent->AttachToComponent(CharacterOwner->GetMesh(), Rules, TEXT("pelvis"));
	UltimateEffectComponent->AddRelativeRotation(FRotator(90.f, 0.f, 0.f));

	UltimateEffectComponent->Deactivate();

	// 
	//if (!CharacterOwner) UE_LOG(LogTemp, Error, TEXT("SkillComponent : Getting Owner Failed!"));
}

void USkillComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SkillPoint);
	DOREPLIFETIME(ThisClass, CurrentSkill);
	DOREPLIFETIME(ThisClass, CurrentMontage);
	DOREPLIFETIME(ThisClass, TempWeapon);

}

void USkillComponent::PostLoad()
{
	Super::PostLoad();

	SetIsReplicated(true);
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetComponentTickInterval(0.01f);
}


// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//UE_LOG(LogTemp, Error, TEXT("SkillComponent Ticking"));

	//CoolTimeChecker(DeltaTime);

	InitForWaiting();
}

void USkillComponent::OnPlayMontageNotifyBeginFunc(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
#define SKILL_CAST_END TEXT("SkillCastEnd")

	if (NotifyName == SKILL_CAST_END)
	{
		//SkillCast(CurrentIndex);
	}
}

void USkillComponent::SetSkillPoint(int32 InSkillPoint)
{
	SkillPoint = InSkillPoint;
	OnSoulCountChanged.Broadcast(SkillPoint);
}

void USkillComponent::AddSkillPoint(int32 InAddAmount)
{
	SkillPoint += InAddAmount;
	OnSoulCountChanged.Broadcast(SkillPoint);
}

void USkillComponent::SkillButtonPressed(int32 InIndex)
{

	ESkillAssistant Skill = SkillList[InIndex];

	FCoolTimeCheckStruct* S = &CoolTimeMap[Skill];


	if (S)
	{
		//if (NeededSkillPoints.Num() <= InIndex) return;
		//S->bSkillPointEnough = SkillPoint >= NeededSkillPoints[InIndex] ? true : false;

		S->bSkillPointEnough = SkillPoint >= S->RequiredPoint ? true : false;
	}

	switch (InIndex)
	{
	case 0:

		if (S->bCanExecute && S->bSkillPointEnough)
		{
			ServerProcedure(Skill, SkillCastingMontage);
		}
		else
		{
			ServerSpawnAttributeAssistantDetach(Skill);
			OnSkillCoolTimeCheck.Broadcast(Skill);
		}
		SkillButtonPressedChecker[InIndex] = !SkillButtonPressedChecker[InIndex];


		break;
	case 1:
		if (S->bCanExecute && S->bSkillPointEnough)
		{
			ServerProcedure(Skill, SkillCastingMontage);
		}
		else
		{
			OnSkillCoolTimeCheck.Broadcast(Skill);

		}

		break;
	case 2:
		if (S->bCanExecute && S->bSkillPointEnough)
		{
			ServerProcedure(Skill, SkillCastingMontage);
		}
		else
		{
			OnSkillCoolTimeCheck.Broadcast(Skill);
		}
		break;
	case 3:
		if (S->bCanExecute && S->bSkillPointEnough)
		{
			ServerProcedure(Skill, UltimateMontage);
		}
		else
		{
			OnSkillCoolTimeCheck.Broadcast(Skill);
		}

		break;
	default:
		break;
	}


}

void USkillComponent::SkillCast(ESkillAssistant InSkillAssistant)
{
	if (!CharacterOwner->HasAuthority()) return;

	AB_SUBLOG(LogABDisplay, Warning, TEXT(""));


	//UE_LOG(LogTemp, Display, TEXT("SkillCast"));
	CharacterOwner->SetCombatState(ECombatState::Unoccupied);


	MulticastCastEnd(InSkillAssistant);
	SpawnAttributeAssistant(InSkillAssistant);
}

void USkillComponent::UltimateCast()
{
	CharacterOwner->SetCombatState(ECombatState::UltimateMode);

	UMaterial* M = TransparentMaterial;

	if (CharacterOwner->IGetTeam() == ETeam::RedTeam)
	{
		M = UltimateWeaponMaterial_Red;
		WeaponMaterialIndex = 4; // suppose to...
	}
	else if (CharacterOwner->IGetTeam() == ETeam::BlueTeam)
	{
		M = UltimateWeaponMaterial_Blue;
		WeaponMaterialIndex = 5;
	}

	FCoolTimeCheckStruct* S = CoolTimeMap.Find(ESkillAssistant::Ultimate);


	if (M) CharacterOwner->GetMesh()->SetMaterial(WeaponMaterialIndex, M);

	FTimerHandle H;
	GetWorld()->GetTimerManager().SetTimer(H, this, &ThisClass::UltimateCastFinished, S->MaintainTime);

	MulticastCastEnd(ESkillAssistant::Ultimate);

	UltimateEffectComponent->Activate();

	if (CharacterOwner->HasAuthority())
	{
		if (UInventoryComponent* IC = CharacterOwner->GetComponentByClass<UInventoryComponent>())
		{
			TransparentWeapon = TransparentWeapon == nullptr ? TransparentWeapon = GetWorld()->SpawnActor<AWeapon>(TransparentWeaponClass) : TransparentWeapon;
			if (TransparentWeapon)
			{
				TransparentWeapon->SetOwner(GetOwner());
				TransparentWeapon->SetInstigator(CharacterOwner.Get());

				TempWeapon = IC->GetEquippedWeapon();
				TempWeapon->SetActorHiddenInGame(true);

				IC->SetEquippedWeapon(TransparentWeapon);
				CharacterOwner->EquipWeaponFunc();
			}
		}
	}




}

void USkillComponent::SkillAnimFinished(const UWidgetAnimation* InWidgetAnimation)
{
	UE_LOG(LogTemp, Warning, TEXT("USkillComponent::SkillAnimFinished : %s"), *InWidgetAnimation->GetName());

	FString Str = InWidgetAnimation->GetName();

	Str.RemoveFromEnd(TEXT("_INST"));

	wchar_t LastStr = Str[Str.Len() - 1];

	int32 Index = static_cast<int32>(LastStr);
	Index -= 48;
	FCoolTimeCheckStruct* S = CoolTimeMap.Find(SkillList[Index]);

	if (S)
	{

		if (Str.Contains(*UEnum::GetDisplayValueAsText(ESkillAnimType::CoolTime).ToString()))
		{
			S->bCanExecute = true;
		}
		else if (Str.Contains(*UEnum::GetDisplayValueAsText(ESkillAnimType::Maintain).ToString()))
		{
			OnSkillAnimStarted.Broadcast(ESkillAnimType::CoolTime, Index, S->CoolTime);
		}
	}







}

void USkillComponent::MulticastCastEnd_Implementation(ESkillAssistant InSkillAssistant)
{
	FCoolTimeCheckStruct* S = CoolTimeMap.Find(InSkillAssistant);

	switch (InSkillAssistant)
	{
	case ESkillAssistant::Dash:
		break;
	case ESkillAssistant::Dodge:
		OnSkillAnimStarted.Broadcast(ESkillAnimType::CoolTime, *SkillList.FindKey(InSkillAssistant), S->CoolTime);
		break;
	case ESkillAssistant::Slide:
		OnSkillAnimStarted.Broadcast(ESkillAnimType::CoolTime, *SkillList.FindKey(InSkillAssistant), S->CoolTime);
		break;
	case ESkillAssistant::HealArea:
		OnSkillAnimStarted.Broadcast(ESkillAnimType::Maintain, *SkillList.FindKey(InSkillAssistant), S->MaintainTime);
		break;
	case ESkillAssistant::ShieldRecovery:
		OnSkillAnimStarted.Broadcast(ESkillAnimType::Maintain, *SkillList.FindKey(InSkillAssistant), S->MaintainTime);
		break;
	case ESkillAssistant::Supporter:
		OnSkillAnimStarted.Broadcast(ESkillAnimType::CoolTime, *SkillList.FindKey(InSkillAssistant), S->CoolTime);
		break;
	case ESkillAssistant::Ultimate:
		OnSkillAnimStarted.Broadcast(ESkillAnimType::Maintain, *SkillList.FindKey(InSkillAssistant), S->MaintainTime);
		break;
	case ESkillAssistant::ESA_MAX:
		break;
	default:
		break;
	}


	S->bCanExecute = false;
	SkillPoint -= S->RequiredPoint;
	OnSoulCountChanged.Broadcast(SkillPoint);
}

void USkillComponent::SpawnAttributeAssistant(ESkillAssistant InSkillAssistant)
{
	if (!CharacterOwner.Get()) return;

	FCoolTimeCheckStruct* S = CoolTimeMap.Find(InSkillAssistant);

	switch (InSkillAssistant)
	{
	case ESkillAssistant::HealArea:
	{
		HealArea = GetWorld()->SpawnActor<AHealArea>(HealAreaClass, CharacterOwner->GetTransform());
		if (HealArea)
		{
			HealArea->SetOwner(CharacterOwner.Get());
			HealArea->SetInstigator(CharacterOwner.Get());
			HealArea->SetLifeSpan(S->MaintainTime);

			FAttachmentTransformRules Rules(EAttachmentRule::KeepWorld, true);
			HealArea->AttachToComponent(CharacterOwner->GetMesh(), Rules, TEXT("pelvis"));
		}
	}

	break;
	case ESkillAssistant::ShieldRecovery:
	{
		ShieldBarrier = GetWorld()->SpawnActorDeferred<AShieldBarrier>(ShieldBarrierClass, CharacterOwner->GetTransform());
		if (ShieldBarrier)
		{
			ShieldBarrier->SetOwner(CharacterOwner.Get());
			ShieldBarrier->SetInstigator(CharacterOwner.Get());

			ShieldBarrier->FinishSpawning(CharacterOwner->GetTransform());
			ShieldBarrier->SetLifeSpan(S->MaintainTime);

			FAttachmentTransformRules Rules(EAttachmentRule::KeepWorld, true);
			ShieldBarrier->AttachToComponent(CharacterOwner->GetMesh(), Rules, TEXT("pelvis"));
		}
	}

		break;
	case ESkillAssistant::Supporter:
	{
		if (CharacterOwner->HasAuthority())
		{
			//UE_LOG(LogTemp, Display, TEXT("Getlocalrole : %s"), *UEnum::GetDisplayValueAsText(CharacterOwner->GetLocalRole()).ToString());
			FTransform SpawnTo(CharacterOwner->GetActorRotation(), CharacterOwner->GetActorLocation() + FVector(0.f, 0.f, 100.f));

			EnemyRange = GetWorld()->SpawnActorDeferred<AEnemyRange>(EnemyRangeClass, SpawnTo);
			if (EnemyRange)
			{
				EnemyRange->ISetTeam(CharacterOwner->IGetTeam());
				EnemyRange->SetTeamColor(CharacterOwner->IGetTeam());
				EnemyRange->FinishSpawning(SpawnTo);
				EnemyRange->SpawnDefaultController();
				EnemyRange->SetOwner(CharacterOwner.Get());
				EnemyRange->SetInstigator(EnemyRange.Get());



			}
		}

	}

		break;
	case ESkillAssistant::ESA_MAX:
		break;
	default:
		break;
	}
}

void USkillComponent::SpawnAttributeAssistantDetach(ESkillAssistant InSkillAssistant)
{
	if (!CharacterOwner.Get()) return;

	switch (InSkillAssistant)
	{
	case ESkillAssistant::HealArea:
	{
		if (HealArea)
		{
			FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, true);
			HealArea->DetachFromActor(Rules);

			
		}
	}

	break;
	case ESkillAssistant::ShieldRecovery:
		break;
	case ESkillAssistant::ESA_MAX:
		break;
	default:
		break;
	}
}

void USkillComponent::ServerProcedure_Implementation(ESkillAssistant InSkillAssistant, UAnimMontage* InMontage)
{
	CurrentMontage = InMontage;

	CharacterOwner->GetMesh()->GetAnimInstance()->Montage_Play(CurrentMontage);
	CharacterOwner->SetCombatState(ECombatState::SkillCasting);
	CurrentSkill = InSkillAssistant;
}

void USkillComponent::ServerSpawnAttributeAssistant_Implementation(ESkillAssistant InSkillAssistant)
{
	MulticastSpawnAttributeAssistant(InSkillAssistant);
}

void USkillComponent::MulticastSpawnAttributeAssistant_Implementation(ESkillAssistant InSkillAssistant)
{
	//UE_LOG(LogTemp, Warning, TEXT("MulticastSpawnAttributeAssistant"));
	SpawnAttributeAssistant(InSkillAssistant);
}

void USkillComponent::ServerSpawnAttributeAssistantDetach_Implementation(ESkillAssistant InSkillAssistant)
{
	MulticastSpawnAttributeAssistantDetach(InSkillAssistant);
}

void USkillComponent::MulticastSpawnAttributeAssistantDetach_Implementation(ESkillAssistant InSkillAssistant)
{
	SpawnAttributeAssistantDetach(InSkillAssistant);
}

void USkillComponent::OnRep_SkillPoint()
{
	//UE_LOG(LogTemp, Display, TEXT("OnRep"));

	OnSoulCountChanged.Broadcast(SkillPoint);
}

void USkillComponent::CoolTimeChecker(float DeltaTime)
{

	//for (auto& Iter : CoolTimeMap)
	//{
	//	if (!Iter.Value.bCanExecute)
	//	{
	//		if (Iter.Value.CoolTimeCount > Iter.Value.CoolTime)
	//		{
	//			Iter.Value.bCanExecute = true;
	//			Iter.Value.CoolTimeCount = 0.f;
	//		}
	//		else
	//		{
	//			Iter.Value.CoolTimeCount += DeltaTime;
	//		}
	//	}
	//}

}

void USkillComponent::InitializeCoolTimeMap()
{
	CoolTimeMap.Reserve((int)ESkillAssistant::ESA_MAX);
	SkillButtonPressedChecker.Reserve((int)ESkillAssistant::ESA_MAX);

	CoolTimeMap.Add(ESkillAssistant::HealArea, FCoolTimeCheckStruct(3, 5.f, 20.f));
	CoolTimeMap.Add(ESkillAssistant::ShieldRecovery, FCoolTimeCheckStruct(3, 5.f, 25.f));
	CoolTimeMap.Add(ESkillAssistant::Supporter, FCoolTimeCheckStruct(5, 0.f, 20.f));
	CoolTimeMap.Add(ESkillAssistant::Ultimate, FCoolTimeCheckStruct(20, 10.f, 50.f));

	CoolTimeMap.Add(ESkillAssistant::Slide, FCoolTimeCheckStruct(0, 0.f, 1.f));
	CoolTimeMap.Add(ESkillAssistant::Dash, FCoolTimeCheckStruct(0, 0.f, 2.f));
	CoolTimeMap.Add(ESkillAssistant::Dodge, FCoolTimeCheckStruct(0, 0.f, 3.f));

	int32 Count = 0;
	for (const auto& i : CoolTimeMap)
	{
		SkillList.Add(Count, i.Key);
		SkillButtonPressedChecker.Add(false);
		++Count;
	}
}

void USkillComponent::OnRep_CurrentSkill()
{
}

void USkillComponent::InitForWaiting()
{
	if (!IsSkillCostChangedBroadcasted && OnSkillCostChanged.IsBound() && OnSoulCountChanged.IsBound())
	{

		for (const auto& i : CoolTimeMap)
		{
			OnSkillCostChanged.Broadcast(*SkillList.FindKey(i.Key), FString::FromInt(i.Value.RequiredPoint));
		}



		//for (size_t i = 0; i < NeededSkillPoints.Num(); i++)
		//{
		//	OnSkillCostChanged.Broadcast(i, FString::FromInt(NeededSkillPoints[i]));
		//}

		OnSoulCountChanged.Broadcast(SkillPoint);

		IsSkillCostChangedBroadcasted = true;
	}
}

void USkillComponent::OnRep_CurrentMontage()
{
	CharacterOwner->GetMesh()->GetAnimInstance()->Montage_Play(CurrentMontage);
}

void USkillComponent::UltimateCastFinished()
{
	CharacterOwner->GetMesh()->SetMaterial(WeaponMaterialIndex, TransparentMaterial);
	CharacterOwner->SetCombatState(ECombatState::Unoccupied);
	UltimateEffectComponent->Deactivate();

	if (UInventoryComponent* IC = CharacterOwner->GetComponentByClass<UInventoryComponent>())
	{
		AWeapon* CurrentWeapon = IC->GetEquippedWeapon();
		CurrentWeapon->Dropped();
		CurrentWeapon->Destroy();

		FTimerHandle H;
		GetWorld()->GetTimerManager().SetTimer(H, this, &ThisClass::UltimateCastFinishedDelay, 0.1f, false);
	}


}

void USkillComponent::UltimateCastFinishedDelay()
{
	if (UInventoryComponent* IC = CharacterOwner->GetComponentByClass<UInventoryComponent>())
	{
		IC->SetEquippedWeapon(TempWeapon);
		CharacterOwner->EquipWeaponFunc();
		CharacterOwner->Fire(false);
	}

}
