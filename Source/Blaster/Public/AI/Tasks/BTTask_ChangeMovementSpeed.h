// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChangeMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UBTTask_ChangeMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(VisibleAnywhere)
	float OriginMaxWalkSpeed;

	UPROPERTY(EditAnywhere)
	float MaxWalkSpeedToModify;
};
