// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_ResetTargetLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "BlasterTypes/BlackboardKeys.h"

EBTNodeResult::Type UBTTask_ResetTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Reset character speed to normal & forget last target location by reseting last target location key

	UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();
	if (BC == nullptr) return EBTNodeResult::Failed;

	// TODO: Reset AI Character's Speed to normal
	
	BC->ClearValue(DETECTED_LOCATION);
	//BC->SetValueAsVector(TEXT("TargetLocation"), FVector(0.f));
	BC->SetValueAsBool(TEXT("DoRecognize"), false);

	return EBTNodeResult::Succeeded;
}
