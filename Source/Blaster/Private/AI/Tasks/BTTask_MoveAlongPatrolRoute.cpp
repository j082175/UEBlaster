// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_MoveAlongPatrolRoute.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interfaces/GetPatrolRouteInterface.h"
#include "Actor/SplineActor.h"
#include "Navigation/PathFollowingComponent.h"

EBTNodeResult::Type UBTTask_MoveAlongPatrolRoute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//UE_LOG(LogTemp, Display, TEXT("fefwe"));

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	IGetPatrolRouteInterface* PatrolRoute = Cast<IGetPatrolRouteInterface>(AIPawn);
	if (!PatrolRoute) return EBTNodeResult::Failed;

	ASplineActor* SplineActor = PatrolRoute->IGetPatrolRoute();
	if (SplineActor)
	{
		FVector Direction = SplineActor->GetSplinePointAsWorldPosition();

		EPathFollowingRequestResult::Type R = OwnerComp.GetAIOwner()->MoveToLocation(Direction, 10.f);

		UE_LOG(LogTemp, Display, TEXT("R : %s"), *UEnum::GetDisplayValueAsText(R).ToString());
		if (R == EPathFollowingRequestResult::Type::AlreadyAtGoal)
		{
			SplineActor->IncrementPatrolRoute();
			//FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);

			return EBTNodeResult::Succeeded;
		}

	}

	

	return EBTNodeResult::Failed;
}
