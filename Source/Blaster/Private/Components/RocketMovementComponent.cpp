// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RocketMovementComponent.h"


URocketMovementComponent::EHandleBlockingHitResult URocketMovementComponent::HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining)
{
	Super::HandleBlockingHit(Hit, TimeTick, MoveDelta, SubTickTimeRemaining);

	//UE_LOG(LogTemp, Display, TEXT("URocketMovementComponent Owner : %s"), *GetOwner()->GetName());
	//UE_LOG(LogTemp, Display, TEXT("HitActor : %s"), *Hit.GetActor()->GetName());
	//
	//UE_LOG(LogTemp, Warning, TEXT("HandleBlockingHit"));
	return EHandleBlockingHitResult::AdvanceNextSubstep;
}

void URocketMovementComponent::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	// Rockets should not stop; only explode when ther CollisionBox detects a hit
	//UE_LOG(LogTemp, Warning, TEXT("HandleImpact"));
}
