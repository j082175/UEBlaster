// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Item/Item.h"
#include "Item/Pickups/Pickup.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ASoul : public APickup
{
	GENERATED_BODY()
public:
	ASoul();

	FORCEINLINE int32 GetSoul() { return Soul; }
	FORCEINLINE void SetSoul(int32 Amount) { Soul = Amount; }
	
protected:
	virtual void OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnEmitter();

	UPROPERTY(EditAnywhere, Category = "Soul Properties", meta = (AllowPrivateAccess = true))
	int32 Soul = 10;

	UPROPERTY(EditAnywhere, Category = "Soul Properties", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystemComponent> SoulEffect;

	UPROPERTY(EditAnywhere, Category = "Soul Properties", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystem> SoulEffectEnd;

	UPROPERTY(EditAnywhere, Category = "Soul Properties", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> TakeSound;
};
