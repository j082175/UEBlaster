// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BlasterTypes/AIState.h"
#include "BTTask_SetAIState.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UBTTask_SetAIState : public UBTTaskNode
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	EAIState AIState;
};
