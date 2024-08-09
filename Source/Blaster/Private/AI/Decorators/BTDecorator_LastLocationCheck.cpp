// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_LastLocationCheck.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

bool UBTDecorator_LastLocationCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// 1. Check if target location is not near zero

	UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();
	if (BC == nullptr) return false;

	FVector LastTargetLocation = BC->GetValueAsVector(TEXT("LastTargetLocation"));
	return !LastTargetLocation.IsNearlyZero();

}
