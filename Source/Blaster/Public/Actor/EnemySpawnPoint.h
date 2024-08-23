// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnPoint.generated.h"

UCLASS()
class BLASTER_API AEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void SpawnPickup();
	void SpawnPickupTimerFinished();
	UFUNCTION()
	void StartSpawnPickupTimer(AActor* DestroyedActor);

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AEnemy>> EnemyClass;

	UPROPERTY()
	TObjectPtr<class AEnemy> SpawnedEnemy;


private:
	void MustRandomSpawner();

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = true))
	uint8 bActive : 1 = true;

	FTimerHandle SpawnPickupTimer;
	UPROPERTY(EditAnywhere)
	float SpawnPickupTimeMin;
	UPROPERTY(EditAnywhere)
	float SpawnPickupTimeMax;


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystem> SpawnEffect;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UNiagaraSystem> SpawnEffectNiagara;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsTurnOff : 1;
};
