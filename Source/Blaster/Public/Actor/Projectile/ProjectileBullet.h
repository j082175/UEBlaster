// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Projectile/Projectile.h"
#include "ProjectileBullet.generated.h"

/**
 *
 */
UCLASS()
class BLASTER_API AProjectileBullet : public AProjectile
{
	GENERATED_BODY()
public:
	AProjectileBullet();
	virtual void Destroyed() override;
	virtual void SetIsActive(bool InIsActive) override;
	virtual void SetProjectileMovementVelocity(const FVector& InVelocity) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
protected:
	virtual void BeginPlay() override;
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor*
		OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
