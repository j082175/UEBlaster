// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Pickable/Weapon/Weapon_Gun.h"
#include "HitScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AHitScanWeapon : public AWeapon_Gun
{
	GENERATED_BODY()
public:
	virtual void Fire(const FVector& HitTarget) override;

protected:
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystem> ImpactParticle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundCue> HitSound;


private:




	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystem> BeamParticles;

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<class UParticleSystem> MuzzleFlash;

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<class USoundCue> FireSound;


};
