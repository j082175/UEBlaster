// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent.h"
#include "Characters/CharacterBase.h"
#include "Actor/HealArea.h"
#include "Actor/ShieldBarrier.h"
#include "Characters/Enemy/EnemyRange.h"
#include "Net/UnrealNetwork.h"
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
	DOREPLIFETIME(ThisClass, CurrentIndex);

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

	CoolTimeChecker(DeltaTime);

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

void USkillComponent::SkillButtonPressed(int32 InIndex)
{
	FCoolTimeCheckStruct* S = CoolTimeMap.Find(UEnum::GetDisplayValueAsText(ESkillAssistant(InIndex)).ToString());

	//UE_LOG(LogTemp, Display, TEXT("SkillPoint : %d"), SkillPoint);

	switch (InIndex)
	{
	case 0:
		if (S->bCanExecute && SkillPoint >= NeededSkillPoints[0])
		{
			ServerProcedure(InIndex);
		}
		else
		{
			ServerSpawnAttributeAssistantDetach((ESkillAssistant)InIndex);
			OnSkillCoolTimeCheck.Broadcast(InIndex);
		}
		SkillButtonPressedChecker[InIndex] = !SkillButtonPressedChecker[InIndex];


		break;
	case 1:
		if (S->bCanExecute && SkillPoint >= NeededSkillPoints[1])
		{
			ServerProcedure(InIndex);
		}
		else
		{
			OnSkillCoolTimeCheck.Broadcast(InIndex);
		}

		break;
	case 2:
		if (S->bCanExecute && SkillPoint >= NeededSkillPoints[2])
		{
			ServerProcedure(InIndex);
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

void USkillComponent::SkillCast(int32 InIndex)
{
	if (!CharacterOwner->HasAuthority()) return;

	AB_SUBLOG(LogABDisplay, Warning, TEXT(""));


	//UE_LOG(LogTemp, Display, TEXT("SkillCast"));
	CharacterOwner->SetCombatState(ECombatState::ECS_Unoccupied);


	MulticastCastEnd(InIndex);
	SpawnAttributeAssistant((ESkillAssistant)InIndex);
}

void USkillComponent::MulticastCastEnd_Implementation(int32 InIndex)
{
	FCoolTimeCheckStruct* S = CoolTimeMap.Find(UEnum::GetDisplayValueAsText(ESkillAssistant(InIndex)).ToString());

	OnSkillCoolTimeStarted.Broadcast(TEXT("Skill"), InIndex, S->CoolTime);
	S->bCanExecute = false;
	SkillPoint -= NeededSkillPoints[InIndex];
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
		if (HealArea.IsValid())
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
		if (ShieldBarrier.IsValid())
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
			if (EnemyRange.IsValid())
			{
				EnemyRange->FinishSpawning(SpawnTo);
				EnemyRange->SpawnDefaultController();
				EnemyRange->SetOwner(CharacterOwner.Get());
				EnemyRange->SetInstigator(CharacterOwner.Get());

				EnemyRange->ISetTeam(CharacterOwner->IGetTeam());
				EnemyRange->SetTeamColor(CharacterOwner->IGetTeam());

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
		if (HealArea.IsValid())
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

void USkillComponent::ServerProcedure_Implementation(int32 InIndex)
{
	CharacterOwner->GetMesh()->GetAnimInstance()->Montage_Play(SkillCastingMontage);
	CharacterOwner->SetCombatState(ECombatState::ECS_SkillCasting);
	CurrentIndex = InIndex;
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

	for (auto& Iter : CoolTimeMap)
	{

		if (!Iter.Value.bCanExecute)
		{
			//UE_LOG(LogTemp, Display, TEXT("CoolTimeChecker"));

			//if (Iter.Key == UEnum::GetDisplayValueAsText(ESkillAssistant::ESA_HealArea).ToString())
//{
//	UE_LOG(LogTemp, Display, TEXT("cooltime : %f"), Iter.Value.CoolTime);
//}

			if (Iter.Value.CoolTimeCount > Iter.Value.CoolTime)
			{
				//UE_LOG(LogTemp, Display, TEXT("CoolTime : %f"), Iter.Value.CoolTime);
				//UE_LOG(LogTemp, Display, TEXT("End :%s"), *Iter.Key);
				Iter.Value.bCanExecute = true;
				Iter.Value.CoolTimeCount = 0.f;
			}
			else
			{
				//UE_LOG(LogTemp, Display, TEXT("Starting : %s, %f"), *Iter.Key, Iter.Value.CoolTimeCount);
				Iter.Value.CoolTimeCount += DeltaTime;
			}
		}
	}

}

void USkillComponent::InitializeCoolTimeMap()
{
	CoolTimeMap.Add(TEXT("Dodge"), FCoolTimeCheckStruct(3.f));
	CoolTimeMap.Add(TEXT("Dash"), FCoolTimeCheckStruct(2.f));

	CoolTimeMap.Reserve(5);
	SkillButtonPressedChecker.Reserve(5);

	for (size_t i = 0; i < 5; i++)
	{
		FString Str = UEnum::GetDisplayValueAsText((ESkillAssistant)i).ToString();
		CoolTimeMap.Add(Str);
		SkillButtonPressedChecker.Add(false);
	}


}

void USkillComponent::OnRep_CurrentIndex()
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
