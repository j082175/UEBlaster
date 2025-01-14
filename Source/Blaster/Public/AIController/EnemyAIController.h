// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BlasterTypes/AIState.h"
#include "EnemyAIController.generated.h"

/**
 *
 */
UCLASS()
class BLASTER_API AEnemyAIController : public ABaseAIController
{
	GENERATED_BODY()
	AEnemyAIController();

private:

	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) override;

	virtual void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

	virtual void OnTargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo) override;

	virtual void OnTargetPerceptionForgotten(AActor* Actor) override;

protected:
};
