// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsHit.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UBTDecorator_IsHit : public UBTDecorator
{
	GENERATED_BODY()
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	 virtual FString GetStaticDescription() const override;

	 virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;

	/** called when underlying node has finished
	 * this function should be considered as const (don't modify state of object) if node is not instanced!
	 * bNotifyDeactivation must be set to true for this function to be called
	 * Calling INIT_DECORATOR_NODE_NOTIFY_FLAGS in the constructor of the decorator will set this flag automatically */
	 virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult);

	/** called when underlying node was processed (deactivated or failed to activate)
	 * this function should be considered as const (don't modify state of object) if node is not instanced!
	 * bNotifyProcessed must be set to true for this function to be called
	 * Calling INIT_DECORATOR_NODE_NOTIFY_FLAGS in the constructor of the decorator will set this flag automatically */
	 virtual void OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult);
};
