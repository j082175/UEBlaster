// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Item/Pickable/Weapon/Weapon_Gun.h"
#include "Item/Pickable/Weapon/Weapon_Melee.h"
//#include "Item/Item.h"
#include "BlasterTypes/Team.h"
#include "Flag.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AFlag : public AWeapon_Melee
{
	GENERATED_BODY()
public:
	AFlag();
	void ResetFlag();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastResetFlag();

	FORCEINLINE ETeam IGetTeam() const { return Team; }

protected:
	//virtual void OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//virtual void OnCapsuleEndOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void BeginPlay() override;

	virtual void Dropped() ; 
	virtual void OnEquipped() ;
	virtual void OnDropped() ;
	
	FORCEINLINE FTransform GetInitialTransform() const { return InitialTransform; }
private:
	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<UStaticMeshComponent> FlagMesh;

	UPROPERTY()
	FTransform InitialTransform;

	UPROPERTY(EditAnywhere)
	ETeam Team;
};
