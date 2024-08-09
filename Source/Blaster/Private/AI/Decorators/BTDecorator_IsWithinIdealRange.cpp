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

	AActor* TargetActor = Cast<AActor>(BC->GetValueAsObject(TARGET_ACTOR));
	if (TargetActor == nullptr) return false;

	if (RangeThreshold < (AIPawn->GetActorLocation() - TargetActor->GetActorLocation()).Size2D())
	{
		return false;
	}
	else
	{
		return true;
	}
}
