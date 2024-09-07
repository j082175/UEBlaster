// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_Teleport.h"
#include "AIController.h"
#include "BlasterTypes/BlackboardKeys.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController/BaseAIController.h"

EBTNodeResult::Type UBTTask_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABaseAIController* BaseAIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (!BaseAIController) return EBTNodeResult::Failed;

	APawn* ControllingPawn = BaseAIController->GetPawn();
	if (!ControllingPawn) return EBTNodeResult::Failed;

	UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();
	if (!BC) return EBTNodeResult::Failed;

	AActor* Owner = Cast<AActor>(BC->GetValueAsObject(OWING_ACTOR));
	if (!Owner) return EBTNodeResult::Failed;

	if (ControllingPawn->TeleportTo(Owner->GetActorLocation(), Owner->GetActorRotation()))
	{
		BC->SetValueAsFloat(OWING_ACTOR_DISTANCE, 0.f);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}


}