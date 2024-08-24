// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "UObject/Object.h"

#include "PooledObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledObjectDespawnDelegate, APooledObject*, PoolActor);

UCLASS()
class BLASTER_API APooledObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APooledObject();
	virtual void Tick(float DeltaTime) override;

	FOnPooledObjectDespawnDelegate OnPooledObjectDespawn;

	UFUNCTION(BlueprintCallable)
	void Deactivate();

	//virtual void Destroyed() override;

	virtual void SetIsActive(bool InIsActive);
	void SetLifeTime(float InLifeTime);
	void SetPoolIndex(int InIndex);

	bool IsActive();
	int GetPoolIndex();

	void SetIsNotPoolable(bool InIsNotPoolable);


protected:
	uint8 bIsActive : 1;
	float LifeSpan = 0.f;
	int PoolIndex;

	FTimerHandle LifeSpanTimer;

	UPROPERTY(EditAnywhere)
	uint8 bIsNotPoolable : 1;
};
