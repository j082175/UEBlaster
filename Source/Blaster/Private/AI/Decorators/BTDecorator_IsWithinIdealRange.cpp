// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_IsWithinIdealRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlasterTypes/BlackboardKeys.h"


bool UBTDecorator_IsWithinIdealRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIPawn == nullptr) return false;

	UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();
	if (BC == nullptr) return false;

	FString DistanceStr = Target == ETarget::OwingActor ? OWING_ACTOR_DISTANCE : TARGET_ACTOR_DISTANCE;

	float Distance = BC->GetValueAsFloat(*DistanceStr);

	if (RangeThreshold < Distance)
	{
		return false;
	}
	else
	{
		return true;
	}
}
