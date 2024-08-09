// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsWithinIdealRange.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UBTDecorator_IsWithinIdealRange : public UBTDecorator
{
	GENERATED_BODY()
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float RangeThreshold = 500.f;

};
