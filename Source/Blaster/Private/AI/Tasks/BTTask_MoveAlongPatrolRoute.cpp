// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_MoveAlongPatrolRoute.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_MoveAlongPatrolRoute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);




	return EBTNodeResult::Succeeded;
}
