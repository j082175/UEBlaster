// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PooledCharacter.generated.h"

UCLASS()
class BLASTER_API APooledCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APooledCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());



	//FOnPooledObjectDespawnDelegate OnPooledObjectDespawn;

	UFUNCTION(BlueprintCallable)
	void Deactivate();

	//virtual void Destroyed() override;

	virtual void SetIsActive(bool InIsActive);
	void SetLifeTime(float InLifeTime);
	void SetPoolIndex(int InIndex);

	bool IsActive();
	int GetPoolIndex();

protected:
	uint8 bIsActive : 1;
	float LifeSpan = 0.f;
	int PoolIndex;

	FTimerHandle LifeSpanTimer;
};
