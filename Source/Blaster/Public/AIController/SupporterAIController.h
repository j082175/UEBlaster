// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController/BaseAIController.h"
#include "SupporterAIController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ASupporterAIController : public ABaseAIController
{
	GENERATED_BODY()
	ASupporterAIController();
public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:

	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) override;

	virtual void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

	virtual void OnTargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo) override;

	virtual void OnTargetPerceptionForgotten(AActor* Actor) override;

protected:
};
