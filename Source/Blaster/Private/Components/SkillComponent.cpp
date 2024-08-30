// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent.h"
#include "Characters/CharacterBase.h"
#include "Actor/HealArea.h"
#include "Actor/ShieldBarrier.h"
#include "Characters/Enemy/EnemyRange.h"

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

	SetComponentTickEnabled(true);
	SetComponentTickInterval(0.1f);

	// ...

	CharacterOwner = Cast<ACharacterBase>(GetOwner());
	if (!CharacterOwner) UE_LOG(LogTemp, Error, TEXT("SkillComponent : Getting Owner Failed!"));
}


// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//UE_LOG(LogTemp, Error, TEXT("SkillComponent Ticking"));

	CoolTimeChecker(DeltaTime);

}

void USkillComponent::SkillButtonPressed(int32 InIndex)
{
	FCoolTimeCheckStruct* S = CoolTimeMap.Find(UEnum::GetDisplayValueAsText(ESkillAssistant(InIndex)).ToString());


	switch (InIndex)
	{
	case 0:
		if (S->bCanExecute)
		{
			UE_LOG(LogTemp, Display, TEXT(""));
			if (!SkillButtonPressedChecker[InIndex])
			{
				OnSkillCoolTimeStarted.Broadcast(TEXT("Skill"), InIndex, S->CoolTime);
				S->bCanExecute = false;

				ServerSpawnAttributeAssistant((ESkillAssistant)InIndex);
			}
		}
		else
		{
			ServerSpawnAttributeAssistantDetach((ESkillAssistant)InIndex);
		}
		SkillButtonPressedChecker[InIndex] = !SkillButtonPressedChecker[InIndex];


		break;
	case 1:
		if (S->bCanExecute)
		{
			UE_LOG(LogTemp, Display, TEXT("1 Executed"));
			OnSkillCoolTimeStarted.Broadcast(TEXT("Skill"), InIndex, S->CoolTime);
			S->bCanExecute = false;

			ServerSpawnAttributeAssistant((ESkillAssistant)InIndex);
		}

		break;
	case 2:
		if (S->bCanExecute)
		{
			UE_LOG(LogTemp, Display, TEXT("2 Executed"));
			OnSkillCoolTimeStarted.Broadcast(TEXT("Skill"), InIndex, S->CoolTime);
			S->bCanExecute = false;

			ServerSpawnAttributeAssistant((ESkillAssistant)InIndex);
		}
		break;
	default:
		break;
	}
}

void USkillComponent::SpawnAttributeAssistant(ESkillAssistant InSkillAssistant)
{
	if (!CharacterOwner) return;

	switch (InSkillAssistant)
	{
	case ESkillAssistant::ESA_HealArea:
	{
		HealArea = GetWorld()->SpawnActor<AHealArea>(HealAreaClass, CharacterOwner->GetTransform());
		if (HealArea.IsValid())
		{
			HealArea->SetOwner(CharacterOwner);
			HealArea->SetInstigator(CharacterOwner);

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
			ShieldBarrier->SetOwner(CharacterOwner);
			ShieldBarrier->SetInstigator(CharacterOwner);

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

			EnemyRange = GetWorld()->SpawnActor<AEnemyRange>(EnemyRangeClass, SpawnTo);
			if (EnemyRange.IsValid())
			{
				EnemyRange->SpawnDefaultController();
				EnemyRange->SetOwner(CharacterOwner);
				EnemyRange->SetInstigator(CharacterOwner);
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
	if (!CharacterOwner) return;

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

void USkillComponent::CoolTimeChecker(float DeltaTime)
{

	for (auto& Iter : CoolTimeMap)
	{

		//if (Iter.Key == UEnum::GetDisplayValueAsText(ESkillAssistant::ESA_HealArea).ToString())
		//{
		//	UE_LOG(LogTemp, Display, TEXT("cooltime : %f"), Iter.Value.CoolTime);
		//}

		if (!Iter.Value.bCanExecute)
		{
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
