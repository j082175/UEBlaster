// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickups/HealthPickup.h"
#include "Characters/BlasterCharacter.h"
#include "Components/BuffComponent.h"


AHealthPickup::AHealthPickup()
{
}

void AHealthPickup::Destroyed()
{


	Super::Destroyed();
}

void AHealthPickup::OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCapsuleBeginOverlapFunc(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		UBuffComponent* Buff = BlasterCharacter->GetBuffComponent();
		if (Buff)
		{
			Buff->Heal(HealAmount, HealingTime);
		}
	}

	Destroy();
}
