// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController/SupporterAIController.h"
#include "Characters/Enemy/Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Perception/AIPerceptionComponent.h"
//#include "Perception/AISense.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "AISystem.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interfaces/AIInterface.h"

#include "BlasterTypes/BlackboardKeys.h"

#include "Characters/CharacterBase.h"


ASupporterAIController::ASupporterAIController()
{
	Tags.Add(TEXT("Supporter"));

	PrimaryActorTick.TickInterval = 0.01f;
}

void ASupporterAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ASupporterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void ASupporterAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	AController* C = Actor->GetInstigatorController();

	if (!Actor->GetInstigatorController()) return;
	if (Actor->GetInstigatorController()->Tags.Num() == 0) return;

	ACharacterBase* CharacterBase = Cast<ACharacterBase>(Actor);
	ACharacterBase* ThisCharacter = Cast<ACharacterBase>(GetPawn());

	if (Actor->GetInstigatorController()->Tags[0] == TEXT("Enemy") || CharacterBase->IGetTeam() != ThisCharacter->IGetTeam())
	{
		//UE_LOG(LogTemp, Display, TEXT("Owner Team : %s, This Team : %s"), CharacterBase->GetTeam(), ThisCharacter->GetTeam());

		Super::OnTargetPerceptionUpdated(Actor, Stimulus);
	}
}

void ASupporterAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::OnPerceptionUpdated(UpdatedActors);
}

void ASupporterAIController::OnTargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	Super::OnTargetPerceptionInfoUpdated(UpdateInfo);
}

void ASupporterAIController::OnTargetPerceptionForgotten(AActor* Actor)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnTargetPerceptionForgotten : %s"), *Actor->GetName());

	AActor* TargetActor = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TARGET_ACTOR));
	if (TargetActor == Actor && GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsObject(TARGET_ACTOR, nullptr);
		SetFocus(nullptr);
		if (AIPawn) AIPawn->ISetAIState(EAIState::Passive);
	}

}
