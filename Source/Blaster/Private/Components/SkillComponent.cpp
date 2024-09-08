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

	// 
	//if (!CharacterOwner) UE_LOG(LogTemp, Error, TEXT("SkillComponent : Getting Owner Failed!"));
}

void USkillComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SkillPoint);
	DOREPLIFETIME(ThisClass, CurrentSkill);

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
	CharacterOwner->SetCombatState(ECombatState::ECS_Unoccupied);


	MulticastCastEnd(InSkillAssistant);
	SpawnAttributeAssistant(InSkillAssistant);
}

void USkillComponent::CastUltimate()
{
	if (!CharacterOwner->HasAuthority()) return;

	UMaterial* M = CharacterOwner->IGetTeam() == ETeam::ET_RedTeam ? UltimateWeaponMaterial_Red : UltimateWeaponMaterial_Blue;

	if (M) CharacterOwner->GetMesh()->SetMaterial(WeaponMaterialIndex, M);

	FTimerHandle H;
	GetWorld()->GetTimerManager().SetTimer(H, this, &ThisClass::CastUltimateFinished, UltimateSustainingTime);

	MulticastCastEnd(ESkillAssistant::ESA_Ultimate);

	UltimateEffectComponent->SetActive(true);
	//UNiagaraFunctionLibrary::SpawnSystemAttached(UltimateEffect, CharacterOwner->GetMesh(), TEXT("pelvis"), CharacterOwner->GetActorLocation(), CharacterOwner->GetActorRotation(), EAttachLocation::KeepWorldPosition, true);
}

void USkillComponent::SkillCoolTimeEnded(const UWidgetAnimation* InWidgetAnimation)
{
	UE_LOG(LogTemp, Warning, TEXT("USkillComponent::SkillCoolTimeEnded : %s"), *InWidgetAnimation->GetName());

	FString Str = InWidgetAnimation->GetName();

	Str.RemoveFromEnd(TEXT("_INST"));

	wchar_t LastStr = Str[Str.Len() - 1];
	
	int32 Index = static_cast<int32>(LastStr);
	Index -= 48;

	FCoolTimeCheckStruct* S = CoolTimeMap.Find(SkillList[Index]);
	if (S)
	{
		S->bCanExecute = true;
	}
}

void USkillComponent::MulticastCastEnd_Implementation(ESkillAssistant InSkillAssistant)
{
	FCoolTimeCheckStruct* S = CoolTimeMap.Find(InSkillAssistant);

	OnSkillCoolTimeStarted.Broadcast(TEXT("Skill"), *SkillList.FindKey(InSkillAssistant), S->CoolTime);
	S->bCanExecute = false;
	//SkillPoint -= NeededSkillPoints[InIndex];
	SkillPoint -= S->RequiredPoint;
	OnSoulCountChanged.Broadcast(SkillPoint);
}

void USkillComponent::SpawnAttributeAssistant(ESkillAssistant InSkillAssistant)
{
	if (!CharacterOwner.Get()) return;

	switch (InSkillAssistant)
	{
	case ESkillAssistant::ESA_HealArea:
	{
		HealArea = GetWorld()->SpawnActor<AHealArea>(HealAreaClass, CharacterOwner->GetTransform());
		if (HealArea)
		{
			HealArea->SetOwner(CharacterOwner.Get());
			HealArea->SetInstigator(CharacterOwner.Get());

			FAttachmentTransformRules Rules(EAttachmentRule::KeepWorld, true);
			HealArea->AttachToComponent(CharacterOwner->GetMesh(), Rules, TEXT("pelvis"));
		}
	}

	break;
	case ESkillAssistant::ESA_ShieldRecovery:
	{
		ShieldBarrier = GetWorld()->SpawnActorDeferred<AShieldBarrier>(ShieldBarrierClass, CharacterOwner->GetTransform());
		if (ShieldBarrier)
		{
			ShieldBarrier->SetOwner(CharacterOwner.Get());
			ShieldBarrier->SetInstigator(CharacterOwner.Get());

			ShieldBarrier->FinishSpawning(CharacterOwner->GetTransform());

			FAttachmentTransformRules Rules(EAttachmentRule::KeepWorld, true);
			ShieldBarrier->AttachToComponent(CharacterOwner->GetMesh(), Rules, TEXT("pelvis"));
		}
	}

		break;
	case ESkillAssistant::ESA_Supporter:
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
	case ESkillAssistant::ESA_HealArea:
	{
		if (HealArea)
		{
			FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, true);
			HealArea->DetachFromActor(Rules);

			
		}
	}

	break;
	case ESkillAssistant::ESA_ShieldRecovery:
		break;
	case ESkillAssistant::ESA_MAX:
		break;
	default:
		break;
	}
}

void USkillComponent::ServerProcedure_Implementation(ESkillAssistant InSkillAssistant, UAnimMontage* InMontage)
{
	CharacterOwner->GetMesh()->GetAnimInstance()->Montage_Play(InMontage);
	CharacterOwner->SetCombatState(ECombatState::ECS_SkillCasting);
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

	CoolTimeMap.Add(ESkillAssistant::ESA_HealArea, FCoolTimeCheckStruct(3, 20.f));
	CoolTimeMap.Add(ESkillAssistant::ESA_ShieldRecovery, FCoolTimeCheckStruct(3, 25.f));
	CoolTimeMap.Add(ESkillAssistant::ESA_Supporter, FCoolTimeCheckStruct(5, 20.f));
	CoolTimeMap.Add(ESkillAssistant::ESA_Ultimate, FCoolTimeCheckStruct(20, 50.f));

	CoolTimeMap.Add(ESkillAssistant::ESA_Slide, FCoolTimeCheckStruct(0, 1.f));
	CoolTimeMap.Add(ESkillAssistant::ESA_Dash, FCoolTimeCheckStruct(0, 2.f));
	CoolTimeMap.Add(ESkillAssistant::ESA_Dodge, FCoolTimeCheckStruct(0, 3.f));

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
	CharacterOwner->GetMesh()->GetAnimInstance()->Montage_Play(SkillCastingMontage);
}

void USkillComponent::InitForWaiting()
{
	if (!IsSkillCostChangedBroadcasted && OnSkillCostChanged.IsBound() && OnSoulCountChanged.IsBound())
	{
		for (size_t i = 0; i < NeededSkillPoints.Num(); i++)
		{
			OnSkillCostChanged.Broadcast(i, FString::FromInt(NeededSkillPoints[i]));

		}

		OnSoulCountChanged.Broadcast(SkillPoint);

		IsSkillCostChangedBroadcasted = true;
	}
}

void USkillComponent::CastUltimateFinished()
{
	CharacterOwner->GetMesh()->SetMaterial(WeaponMaterialIndex, TransparentMaterial);
	CharacterOwner->SetCombatState(ECombatState::ECS_Unoccupied);
	UltimateEffectComponent->SetActive(false);
}
