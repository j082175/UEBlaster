// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Item/Item.h"
#include "Item/Pickups/Pickup.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ATreasure : public APickup
{
	GENERATED_BODY()
public:
	FORCEINLINE int32 GetGold() { return Gold; }
	
protected:
	virtual void OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Treasure Properties", meta = (AllowPrivateAccess = true))
	int32 Gold = 10.f;
};
