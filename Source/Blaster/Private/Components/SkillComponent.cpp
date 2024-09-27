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
#include "GameData/DataSingleton.h"
#include "Kismet/GameplayStatics.h"
#include "Blaster.h"

// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
	bWantsInitializeComponent = true;

	UltimateEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("UltimateEffect"));
}

void USkillComponent::InitializeComponent()
{
	Super::InitializeComponent();
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

	InitializeCoolTimeMap();

}

void USkillComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SkillPoint);
	DOREPLIFETIME(ThisClass, TempWeapon);
	DOREPLIFETIME(ThisClass, CurrentMontage);

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


	//AB_SUBLOG(LogABDisplay, Warning, TEXT(""));

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

	ESkillType Skill = SkillList[InIndex];

	FSkillManagementStruct* S = &CoolTimeMap[InIndex];


	if (S)
	{
		//if (NeededSkillPoints.Num() <= InIndex) return;
		//S->bSkillPointEnough = SkillPoint >= NeededSkillPoints[InIndex] ? true : false;

		S->CoolTimeCheckStruct.bSkillPointEnough = SkillPoint >= S->SkillStat.RequiredPoint ? true : false;
	}

	switch (InIndex)
	{
	case 0:

		if (S->CoolTimeCheckStruct.bCanExecute && S->CoolTimeCheckStruct.bSkillPointEnough)
		{
			ServerProcedure(InIndex, S->SkillData.SkillMontage);
		}
		else
		{
			//ServerSpawnAttributeAssistantDetach(InIndex);
			OnSkillCoolTimeCheck.Broadcast(InIndex);
		}
		SkillButtonPressedChecker[InIndex] = !SkillButtonPressedChecker[InIndex];


		break;
	case 1:
		if (S->CoolTimeCheckStruct.bCanExecute && S->CoolTimeCheckStruct.bSkillPointEnough)
		{
			ServerProcedure(InIndex, S->SkillData.SkillMontage);
		}
		else
		{
			OnSkillCoolTimeCheck.Broadcast(InIndex);

		}

		break;
	case 2:
		if (S->CoolTimeCheckStruct.bCanExecute && S->CoolTimeCheckStruct.bSkillPointEnough)
		{
			ServerProcedure(InIndex, S->SkillData.SkillMontage);
		}
		else
		{
			OnSkillCoolTimeCheck.Broadcast(InIndex);
		}
		break;
	case 3:
		if (S->CoolTimeCheckStruct.bCanExecute && S->CoolTimeCheckStruct.bSkillPointEnough)
		{
			ServerProcedure(InIndex, S->SkillData.SkillMontage);
		}
		else
		{
			OnSkillCoolTimeCheck.Broadcast(InIndex);
		}

		break;
	default:
		break;
	}


}

void USkillComponent::SkillCast(int32 InCurrentSkillIndex)
{
	if (!CharacterOwner->HasAuthority()) return;

	AB_SUBLOG(LogABDisplay, Warning, TEXT(""));


	//UE_LOG(LogTemp, Display, TEXT("SkillCast"));
	CharacterOwner->SetCombatState(ECombatState::Unoccupied);


	MulticastCastEnd(InCurrentSkillIndex);

	ESkillType SkillType = CoolTimeMap[InCurrentSkillIndex].SkillData.SkillType;
	switch (SkillType)
	{
	case ESkillType::Slide:
		break;
	case ESkillType::Dash:
		break;
	case ESkillType::Dodge:
		break;
	case ESkillType::Spawn:
		SpawnAttributeAssistant(InCurrentSkillIndex);
		break;
	case ESkillType::Active:
		break;
	case ESkillType::Passive:
		break;
	case ESkillType::Ultimate:
		break;
	case ESkillType::ESA_MAX:
		break;
	default:
		break;
	}

}

void USkillComponent::UltimateCast()
{
	if (!CharacterOwner->HasAuthority()) return;


	FSkillManagementStruct* S = CoolTimeMap.Find(3);




	FTimerHandle H;
	GetWorld()->GetTimerManager().SetTimer(H, this, &ThisClass::MulticastUltimateCastFinished, S->SkillStat.MaintainTime);

	MulticastCastEnd(3);



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
	FSkillManagementStruct* S = CoolTimeMap.Find(Index);

	if (S)
	{

		if (Str.Contains(*UEnum::GetDisplayValueAsText(ESkillAnimType::CoolTime).ToString()))
		{
			S->CoolTimeCheckStruct.bCanExecute = true;
		}
		else if (Str.Contains(*UEnum::GetDisplayValueAsText(ESkillAnimType::Maintain).ToString()))
		{
			OnSkillAnimStarted.Broadcast(ESkillAnimType::CoolTime, Index, S->SkillStat.CoolTime);
		}
	}







}

void USkillComponent::MulticastCastEnd_Implementation(int32 InCurrentSkillIndex)
{
	FSkillManagementStruct* S = CoolTimeMap.Find(InCurrentSkillIndex);
	S->SkillData.SkillCastingSound;

	UGameplayStatics::PlaySoundAtLocation(this, S->SkillData.SkillCastingSound, CharacterOwner->GetActorLocation());


	//OnSkillAnimStarted.Broadcast(S->SkillData.SkillAnimType, *SkillList.FindKey(InSkillAssistant), S->SkillStat.CoolTime);

	ESkillAnimType SkillAnimType = S->SkillData.SkillAnimType;

	switch (SkillAnimType)
	{
	case ESkillAnimType::CoolTime:
		OnSkillAnimStarted.Broadcast(S->SkillData.SkillAnimType, InCurrentSkillIndex, S->SkillStat.CoolTime);
		break;
	case ESkillAnimType::Maintain:
		OnSkillAnimStarted.Broadcast(S->SkillData.SkillAnimType, InCurrentSkillIndex, S->SkillStat.MaintainTime);
		break;
	case ESkillAnimType::ESA_MAX:
		break;
	default:
		break;
	}


	//switch (InSkillAssistant)
	//{
	//case ESkillType::Dash:
	//	break;
	//case ESkillType::Dodge:
	//	OnSkillAnimStarted.Broadcast(ESkillAnimType::CoolTime, *SkillList.FindKey(InSkillAssistant), S->SkillStat.CoolTime);
	//	break;
	//case ESkillType::Slide:
	//	OnSkillAnimStarted.Broadcast(ESkillAnimType::CoolTime, *SkillList.FindKey(InSkillAssistant), S->SkillStat.CoolTime);
	//	break;
	//case ESkillType::Spawn:
	//	OnSkillAnimStarted.Broadcast(ESkillAnimType::Maintain, *SkillList.FindKey(InSkillAssistant), S->SkillStat.MaintainTime);

	//	break;
	//case ESkillType::Active:
	//	OnSkillAnimStarted.Broadcast(ESkillAnimType::Maintain, *SkillList.FindKey(InSkillAssistant), S->SkillStat.MaintainTime);

	//	break;
	//case ESkillType::Passive:
	//	OnSkillAnimStarted.Broadcast(ESkillAnimType::CoolTime, *SkillList.FindKey(InSkillAssistant), S->SkillStat.CoolTime);

	//	break;
	//case ESkillType::Ultimate:
	//{
	//	OnSkillAnimStarted.Broadcast(ESkillAnimType::Maintain, *SkillList.FindKey(InSkillAssistant), S->SkillStat.MaintainTime);
	//	CharacterOwner->SetCombatState(ECombatState::UltimateMode);

	//	UMaterial* M = TransparentMaterial;

	//	if (CharacterOwner->IGetTeam() == ETeam::RedTeam)
	//	{
	//		M = UltimateWeaponMaterial_Red;
	//		WeaponMaterialIndex = 4; // suppose to...
	//	}
	//	else if (CharacterOwner->IGetTeam() == ETeam::BlueTeam)
	//	{
	//		M = UltimateWeaponMaterial_Blue;
	//		WeaponMaterialIndex = 5;
	//	}

	//	if (M) CharacterOwner->GetMesh()->SetMaterial(WeaponMaterialIndex, M);

	//	UltimateEffectComponent->Activate();
	//}

	//	break;
	//case ESkillType::ESA_MAX:
	//	break;
	//default:
	//	break;
	//}

	if (S->SkillData.SkillType == ESkillType::Ultimate)
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

		if (M) CharacterOwner->GetMesh()->SetMaterial(WeaponMaterialIndex, M);

		UltimateEffectComponent->Activate();
	}


	S->CoolTimeCheckStruct.bCanExecute = false;

	SkillPoint -= S->SkillStat.RequiredPoint;
	OnSoulCountChanged.Broadcast(SkillPoint);
}

void USkillComponent::SpawnAttributeAssistant(int32 InCurrentSkillIndex)
{
	if (!CharacterOwner.IsValid()) return;

	FSkillManagementStruct* S = CoolTimeMap.Find(InCurrentSkillIndex);

	FTransform SpawnTo(CharacterOwner->GetActorRotation(), CharacterOwner->GetActorLocation() + FVector(0.f, 0.f, 100.f));
	AActor* SpawnActor = GetWorld()->SpawnActorDeferred<AActor>(S->SkillData.SpawnActorClass, SpawnTo);
	if (SpawnActor)
	{
		SpawnActor->SetOwner(CharacterOwner.Get());
		SpawnActor->SetInstigator(CharacterOwner.Get());
		SpawnActor->FinishSpawning(SpawnTo);
		SpawnActor->SetLifeSpan(S->SkillStat.MaintainTime);
	}


	//switch (InSkillAssistant)
	//{
	//case ESkillType::Spawn:
	//{
	//	Spawn = GetWorld()->SpawnActor<AHealArea>(HealAreaClass, CharacterOwner->GetTransform());
	//	if (Spawn)
	//	{
	//		Spawn->SetOwner(CharacterOwner.Get());
	//		Spawn->SetInstigator(CharacterOwner.Get());
	//		Spawn->SetLifeSpan(S->SkillStat.MaintainTime);

	//		FAttachmentTransformRules Rules(EAttachmentRule::KeepWorld, true);
	//		Spawn->AttachToComponent(CharacterOwner->GetMesh(), Rules, TEXT("pelvis"));
	//	}
	//}

	//break;
	//case ESkillType::Active:
	//{
	//	ShieldBarrier = GetWorld()->SpawnActorDeferred<AShieldBarrier>(ShieldBarrierClass, CharacterOwner->GetTransform());
	//	if (ShieldBarrier)
	//	{
	//		ShieldBarrier->SetOwner(CharacterOwner.Get());
	//		ShieldBarrier->SetInstigator(CharacterOwner.Get());

	//		ShieldBarrier->FinishSpawning(CharacterOwner->GetTransform());
	//		ShieldBarrier->SetLifeSpan(S->SkillStat.MaintainTime);

	//		FAttachmentTransformRules Rules(EAttachmentRule::KeepWorld, true);
	//		ShieldBarrier->AttachToComponent(CharacterOwner->GetMesh(), Rules, TEXT("pelvis"));
	//	}
	//}

	//break;
	//case ESkillType::Passive:
	//{
	//	if (CharacterOwner->HasAuthority())
	//	{
	//		//UE_LOG(LogTemp, Display, TEXT("Getlocalrole : %s"), *UEnum::GetDisplayValueAsText(CharacterOwner->GetLocalRole()).ToString());
	//		FTransform SpawnTo(CharacterOwner->GetActorRotation(), CharacterOwner->GetActorLocation() + FVector(0.f, 0.f, 100.f));

	//		EnemyRange = GetWorld()->SpawnActorDeferred<AEnemyRange>(EnemyRangeClass, SpawnTo);
	//		if (EnemyRange)
	//		{
	//			EnemyRange->ISetTeam(CharacterOwner->IGetTeam());
	//			EnemyRange->SetTeamColor(CharacterOwner->IGetTeam());
	//			EnemyRange->FinishSpawning(SpawnTo);
	//			EnemyRange->SpawnDefaultController();
	//			EnemyRange->SetOwner(CharacterOwner.Get());
	//			EnemyRange->SetInstigator(EnemyRange.Get());



	//		}
	//	}

	//}

	//break;
	//case ESkillType::ESA_MAX:
	//	break;
	//default:
	//	break;
	//}
}

void USkillComponent::SpawnAttributeAssistantDetach(int32 InCurrentSkillIndex)
{
	if (!CharacterOwner.Get()) return;

	FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, true);
	Spawn->DetachFromActor(Rules);

	//switch (InCurrentSkillIndex)
	//{
	//case ESkillType::Spawn:
	//{
	//	if (Spawn)
	//	{
	//		FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, true);
	//		Spawn->DetachFromActor(Rules);


	//	}
	//}

	//break;
	//case ESkillType::Active:
	//	break;
	//case ESkillType::ESA_MAX:
	//	break;
	//default:
	//	break;
	//}
}

void USkillComponent::ProcedureFunc(int32 InCurrentSkillIndex, UAnimMontage* InMontage)
{
	CurrentMontage = InMontage;

	CharacterOwner->GetMesh()->GetAnimInstance()->Montage_Play(CurrentMontage);
	CharacterOwner->SetCombatState(ECombatState::SkillCasting);
	CurrentSkillIndex = InCurrentSkillIndex;

}

void USkillComponent::ServerProcedure_Implementation(int32 InCurrentSkillIndex, UAnimMontage* InMontage)
{
	ProcedureFunc(InCurrentSkillIndex, InMontage);
}

void USkillComponent::ServerSpawnAttributeAssistant_Implementation(int32 InCurrentSkillIndex)
{
	MulticastSpawnAttributeAssistant(InCurrentSkillIndex);
}

void USkillComponent::MulticastSpawnAttributeAssistant_Implementation(int32 InCurrentSkillIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("MulticastSpawnAttributeAssistant"));
	SpawnAttributeAssistant(InCurrentSkillIndex);
}

void USkillComponent::ServerSpawnAttributeAssistantDetach_Implementation(int32 InCurrentSkillIndex)
{
	MulticastSpawnAttributeAssistantDetach(InCurrentSkillIndex);
}

void USkillComponent::MulticastSpawnAttributeAssistantDetach_Implementation(int32 InCurrentSkillIndex)
{
	SpawnAttributeAssistantDetach(InCurrentSkillIndex);
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
	if (!CharacterOwner.IsValid()) return;

	CoolTimeMap.Reserve((int)ESkillType::ESA_MAX);
	SkillButtonPressedChecker.Reserve((int)ESkillType::ESA_MAX);

	UDataSingleton& DataSingleton = UDataSingleton::Get();

	for (size_t i = 0; i < 7; i++)
	{
		FSkillManagementStruct Management;
		Management.SkillStat = DataSingleton.GetSkillStat(CharacterOwner->GetCharacterType(), *FString::FromInt(i));
		Management.SkillData = DataSingleton.GetSkillData(CharacterOwner->GetCharacterType(), *FString::FromInt(i));

		CoolTimeMap.Add(i, Management);

		SkillButtonPressedChecker.Add(false);
	}

	/*int count = 0;
	for (size_t i = 3; i < (int)ESkillType::ESA_MAX; i++)
	{
		ESkillType SA = (ESkillType)i;

		FSkillManagementStruct Management;
		Management.SkillStat = DataSingleton.GetSkillStat(CharacterOwner->GetCharacterType(), *FString::FromInt(i));
		Management.SkillData = DataSingleton.GetSkillData(CharacterOwner->GetCharacterType(), *FString::FromInt(i));

		CoolTimeMap.Add(i, Management);

		SkillList.Add(count, SA);
		SkillButtonPressedChecker.Add(false);



		++count;
	}

	for (size_t i = 0; i <= (int)ESkillType::Dodge; i++)
	{
		ESkillType SA = (ESkillType)i;

		FSkillManagementStruct Management;
		Management.SkillStat = DataSingleton.GetSkillStat(CharacterOwner->GetCharacterType(), *FString::FromInt(i));
		Management.SkillData = DataSingleton.GetSkillData(CharacterOwner->GetCharacterType(), *FString::FromInt(i));

		CoolTimeMap.Add(i, Management);

		SkillList.Add(count, SA);
		SkillButtonPressedChecker.Add(false);
		++count;
	}*/
}

void USkillComponent::InitForWaiting()
{
	if (!IsSkillCostChangedBroadcasted && OnSkillCostChanged.IsBound() && OnSoulCountChanged.IsBound())
	{

		int32 Count = 0;
		for (const auto& i : CoolTimeMap)
		{
			//OnSkillCostChanged.Broadcast(*SkillList.FindKey(i.Key), FString::FromInt(i.Value.SkillStat.RequiredPoint));
			OnSkillCostChanged.Broadcast(Count, FString::FromInt(i.Value.SkillStat.RequiredPoint));
			++Count;
		}



		//for (size_t i = 0; i < NeededSkillPoints.Num(); i++)
		//{
		//	OnSkillCostChanged.Broadcast(i, FString::FromInt(NeededSkillPoints[i]));
		//}

		OnSoulCountChanged.Broadcast(SkillPoint);

		IsSkillCostChangedBroadcasted = true;

		AB_SUBLOG(LogABDisplay, Warning, TEXT(""));
	}
}

void USkillComponent::MulticastUltimateCastFinished_Implementation()
{
	//AB_SUBLOG(LogABDisplay, Warning, TEXT(""));
	CharacterOwner->GetMesh()->SetMaterial(WeaponMaterialIndex, TransparentMaterial);
	UltimateEffectComponent->Deactivate();

	if (UInventoryComponent* IC = CharacterOwner->GetComponentByClass<UInventoryComponent>())
	{
		AWeapon* CurrentWeapon = IC->GetEquippedWeapon();
		CurrentWeapon->Dropped();
		CurrentWeapon->Destroy();
		IC->SetEquippedWeapon(nullptr);

		CharacterOwner->bFireButtonPressed = true;
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

		CharacterOwner->SetCombatState(ECombatState::Unoccupied);
		CharacterOwner->bFireButtonPressed = false;
	}

}
