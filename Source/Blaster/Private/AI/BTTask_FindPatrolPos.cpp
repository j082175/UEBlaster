// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BlasterTypes/BlackboardKeys.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavigationSystemV1 = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (!NavigationSystemV1)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	FNavLocation NavLocation;
	//NavigationSystemV1->GetRandomPointInNavigableRadius(ControllingPawn->GetActorLocation(), NavRadius, NavLocation);
	NavigationSystemV1->GetRandomReachablePointInRadius(ControllingPawn->GetActorLocation(), NavRadius, NavLocation);
	//NavigationSystemV1->GetRandomPoint(NavLocation);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(OwnerComp.GetAIOwner(), NavLocation);

	BlackboardComponent->SetValueAsVector(PATROL_POS, NavLocation.Location);

	return EBTNodeResult::Succeeded;
}
