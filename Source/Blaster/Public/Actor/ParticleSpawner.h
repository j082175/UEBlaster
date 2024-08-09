// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ParticleSpawner.generated.h"

UCLASS()
class BLASTER_API AParticleSpawner : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParticleSpawner();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Launch();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnLaser();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystemComponent> ParticleSystemComponent;

	FTransform SkyLaserSpawnPoint;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystem> SkyLaserParticle;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AProjectile> ProjectileToSpawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float MoveToTargetSpeed = 5.f;
};
