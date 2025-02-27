// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickups/SpeedPickup.h"
#include "Components/BuffComponent.h"

void ASpeedPickup::OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCapsuleBeginOverlapFunc(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UBuffComponent* Buff = OtherActor->GetComponentByClass<UBuffComponent>();
	if (Buff)
	{
		Buff->BuffSpeed(BaseSpeedBuff, CrouchSpeedBuff, SpeedBuffTime);
		Buff->SpeedBuff = this;
	}

	Destroy();
}

void ASpeedPickup::Destroyed()
{
	Super::Destroyed();

}
