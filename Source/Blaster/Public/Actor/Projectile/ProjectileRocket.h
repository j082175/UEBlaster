// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Projectile/Projectile.h"
#include "ProjectileRocket.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()
public:
	AProjectileRocket();
	virtual void Destroyed() override;

	virtual void SetIsActive(bool InIsActive) override;
	virtual void SetProjectileMovementVelocity(const FVector& InVelocity) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;


	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundCue> ProjectileLoop;

	UPROPERTY()
	TObjectPtr<class UAudioComponent> ProjectileLoopComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundAttenuation> LoopingSoundAttenuation;


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class URocketMovementComponent> RocketMovementComponent;

};
