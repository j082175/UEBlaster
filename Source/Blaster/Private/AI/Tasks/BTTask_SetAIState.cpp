// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_SetAIState.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interfaces/AIInterface.h"

EBTNodeResult::Type UBTTask_SetAIState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr) return EBTNodeResult::Failed;

	//AIController->GetPawn()
	IAIInterface* AIPawn = Cast<IAIInterface>(AIController->GetPawn());
	if (AIPawn == nullptr) return EBTNodeResult::Failed;
	
	AIPawn->ISetAIState(AIState);

	return EBTNodeResult::Succeeded;
}