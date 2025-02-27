// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PooledObject.h"
#include "Components/ObjectPoolComponent.h"

// Sets default values
APooledObject::APooledObject()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	//ObjectPoolComponent = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("ObjectPoolComponent"));
}

void APooledObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("PooledObject Tick"));
}

void APooledObject::Deactivate()
{
	//UE_LOG(LogTemp, Display, TEXT("Deactivate"));
	SetIsActive(false);
	GetWorldTimerManager().ClearAllTimersForObject(this);
	OnPooledObjectDespawn.Broadcast(this);
}

void APooledObject::SetIsActive(bool InIsActive)
{
	bIsActive = InIsActive;
	SetActorHiddenInGame(!InIsActive);
	SetActorEnableCollision(InIsActive);
	//SetActorTickEnabled(InIsActive);
	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &ThisClass::Deactivate, LifeSpan, false);
}

void APooledObject::SetLifeTime(float InLifeTime)
{
	LifeSpan = InLifeTime;
}

void APooledObject::SetPoolIndex(int InIndex)
{
	PoolIndex = InIndex;
}

bool APooledObject::IsActive()
{
	return bIsActive;
}

int APooledObject::GetPoolIndex()
{
	return PoolIndex;
}

void APooledObject::SetIsNotPoolable(bool InIsNotPoolable)
{
	bIsNotPoolable = InIsNotPoolable;
}
