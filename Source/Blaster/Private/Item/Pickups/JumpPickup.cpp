// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickups/JumpPickup.h"
#include "Components/BuffComponent.h"
#include "Components/CombatComponent.h"

void AJumpPickup::OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCapsuleBeginOverlapFunc(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UBuffComponent* Buff = OtherActor->GetComponentByClass<UBuffComponent>();
	if (Buff)
	{
		Buff->BuffJump(JumpZVelocityBuff, JumpBuffTime);
	}

	Destroy();
}
