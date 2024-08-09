// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_SetFocus.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BlasterTypes/BlackboardKeys.h"

EBTNodeResult::Type UBTTask_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Set focus to target if valid

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SetFocus : AIController is null"));
		return EBTNodeResult::Failed;
	}

	bool bFocusReset = false; // TODO:


	UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();
	if (BC == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SetFocus : UBlackboardComponent is null"));
		return EBTNodeResult::Failed;
	}

	UObject* Object = BC->GetValueAsObject(TARGET_ACTOR);
	if (Object)
	{
		AActor* Target = Cast<AActor>(Object);

		if (!bFocusReset)
		{
			AIController->SetFocus(Target);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SetFocus : Object Is null"));

		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
