// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickups/AmmoPickup.h"
#include "Characters/BlasterCharacter.h"

AAmmoPickup::AAmmoPickup()
{
	PickupMesh->SetRelativeScale3D(FVector(5.f));
}

void AAmmoPickup::OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCapsuleBeginOverlapFunc(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		BlasterCharacter->PickupAmmo(WeaponType, AmmoAmount);
	}

	Destroy();
}
