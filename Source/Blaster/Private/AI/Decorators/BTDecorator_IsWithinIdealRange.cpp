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

	float Distance = BC->GetValueAsFloat(TARGET_ACTOR_DISTANCE);

	if (RangeThreshold < Distance)
	{
		return false;
	}
	else
	{
		return true;
	}
}
