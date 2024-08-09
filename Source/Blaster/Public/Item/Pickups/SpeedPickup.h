// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Pickups/Pickup.h"
#include "SpeedPickup.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ASpeedPickup : public APickup
{
	GENERATED_BODY()
public:
	FORCEINLINE float GetBaseSpeedBuff() const { return BaseSpeedBuff; }
	FORCEINLINE float GetCrouchSpeedBuff() const { return CrouchSpeedBuff; }
protected:
	virtual void OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void Destroyed() override;



private:
	class UCombatComponent* Combat;

	UPROPERTY(EditAnywhere)
	float BaseSpeedBuff = 1600.f;

	UPROPERTY(EditAnywhere)
	float CrouchSpeedBuff = 800.f;

	UPROPERTY(EditAnywhere)
	float SpeedBuffTime = 10.f;
};
