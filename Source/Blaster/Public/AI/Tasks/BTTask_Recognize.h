// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Recognize.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UBTTask_Recognize : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	bool CheckHistory();
	void SetupRecognizeTime();
	void AdjustRecognizeTime();


	class AEnemy* AIEnemy;
	UBlackboardComponent* BC;
	float RecognizeTime;
	float FirstSeeTime;
	float LastDistance;

	float TempCurrentDistance;
};
