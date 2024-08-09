// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	// 1. Set a random point location to blackboard
	ControllingPawn->GetActorLocation();

	UNavigationSystemV1* NavigationSystemV1 = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavigationSystemV1 == nullptr) return EBTNodeResult::Failed;

	//Radius = 500.f; // Temporary value.

	UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();
	if (BC == nullptr) return EBTNodeResult::Failed;

	BC->ClearValue(TEXT("HeardNoise"));
	bool bIsHeardSomething = BC->GetValueAsBool(TEXT("HeardNoise"));

	// Reduce radius if heard something
	float RealRadius = bIsHeardSomething ? Radius / 3.f : Radius;

	FNavLocation NavLocation;
	NavigationSystemV1->GetRandomPointInNavigableRadius(ControllingPawn->GetActorLocation(), RealRadius, NavLocation);

	BC->SetValueAsVector(TEXT("MoveToLocation"), NavLocation.Location);

	return EBTNodeResult::Succeeded;
}
