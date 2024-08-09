// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Interfaces/AIInterface.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	bNotifyTaskFinished = true;
	
	//UE_LOG(LogTemp, Display, TEXT("Attack"));

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		//UE_LOG(LogTemp, Display, TEXT("ControllingPawn is null"));

		return EBTNodeResult::Failed;
	}

	IAIInterface* AIPawn = Cast<IAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		//UE_LOG(LogTemp, Display, TEXT("AIPawn is null"));
		return EBTNodeResult::Failed;
	}


	FAttackEndedDelegate OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			//UE_LOG(LogTemp, Display, TEXT("FinishLatentTask"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	AIPawn->IAttack(OnAttackFinished, *UEnum::GetDisplayValueAsText(BossAttackType).ToString());


	//UE_LOG(LogTemp, Display, TEXT("InProgress"));
	return EBTNodeResult::InProgress;
}

