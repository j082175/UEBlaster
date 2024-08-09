// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "NavLinkEnemyAI.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ANavLinkEnemyAI : public ANavLinkProxy
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void SmartLinkReached(AActor* MovingActor, const FVector& DestinationPoint);
};
