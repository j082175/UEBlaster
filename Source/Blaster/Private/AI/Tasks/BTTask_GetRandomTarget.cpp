// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_GetRandomTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlasterTypes/BlackboardKeys.h"

EBTNodeResult::Type UBTTask_GetRandomTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	TArray<APawn*> PArr;

	for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; iter++)
	{

		APawn* OwingPawn = iter->Get()->AcknowledgedPawn;
		if (OwingPawn)
		{
			PArr.Add(OwingPawn);
		}
	}

	if (PArr.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	int32 RandResult = FMath::RandRange(0, PArr.Num() - 1);

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TARGET_ACTOR, PArr[RandResult]);


	return EBTNodeResult::Succeeded;
}
