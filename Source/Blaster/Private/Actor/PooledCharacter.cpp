// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PooledCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values


APooledCharacter::APooledCharacter(const FObjectInitializer& ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent->SetMobility(EComponentMobility::Static);
}

void APooledCharacter::Deactivate()
{
	//UE_LOG(LogTemp, Display, TEXT("Deactivate"));
	SetIsActive(false);
	GetWorldTimerManager().ClearAllTimersForObject(this);
	//OnPooledObjectDespawn.Broadcast(this);
}

void APooledCharacter::SetIsActive(bool InIsActive)
{
	bIsActive = InIsActive;
	SetActorHiddenInGame(!InIsActive);
	SetActorEnableCollision(InIsActive);
	PrimaryActorTick.bCanEverTick = true;
	
	SetActorTickEnabled(InIsActive);
	//SetActorTickInterval(0.1f);
	//GetMesh()->SetComponentTickEnabled(InIsActive);
	GetCharacterMovement()->SetComponentTickEnabled(InIsActive);

	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &ThisClass::Deactivate, LifeSpan, false);

	if (InIsActive)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}

}

void APooledCharacter::SetLifeTime(float InLifeTime)
{
	LifeSpan = InLifeTime;
}

void APooledCharacter::SetPoolIndex(int InIndex)
{
	PoolIndex = InIndex;
}

bool APooledCharacter::IsActive()
{
	return bIsActive;
}

int APooledCharacter::GetPoolIndex()
{
	return PoolIndex;
}

