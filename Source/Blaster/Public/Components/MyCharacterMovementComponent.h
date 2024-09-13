// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UMyCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	UMyCharacterMovementComponent();
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void PostLoad() override;


	FORCEINLINE void SetUseAccelerationForPaths(bool InParam) { bUseAccelerationForPaths = InParam; }



};
