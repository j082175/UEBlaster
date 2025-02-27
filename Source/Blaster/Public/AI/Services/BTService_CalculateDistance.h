// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BlasterTypes/Target.h"
#include "BTService_CalculateDistance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UBTService_CalculateDistance : public UBTService
{
	GENERATED_BODY()
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	ETarget Target;
};
