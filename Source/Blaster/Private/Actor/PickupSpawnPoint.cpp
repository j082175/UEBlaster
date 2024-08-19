// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PickupSpawnPoint.h"
#include "Item/Pickups/Pickup.h"
#include "GameState/BlasterGameState.h"
#include "Components/ObjectPoolComponent.h"

// Sets default values
APickupSpawnPoint::APickupSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

void APickupSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	StartSpawnPickupTimer(static_cast<AActor*>(nullptr));
}

void APickupSpawnPoint::SpawnPickup()
{
	int32 NumPickupClasses = PickupClasses.Num();
	if (NumPickupClasses > 0)
	{
		int32 Selection = FMath::RandRange(0, NumPickupClasses - 1);

		//SpawnedPickup = GetWorld()->SpawnActorDeferred<APickup>(PickupClasses[Selection], GetActorTransform());
		SpawnedPickup = Cast<APickup>(GetWorld()->GetGameState<ABlasterGameState>()->GetComponentByClass<UObjectPoolComponent>()->GetSpawnedObject(GetActorTransform(), PickupClasses[Selection]));


		if (HasAuthority() && SpawnedPickup)
		{
			//SpawnedPickup->OnDestroyed.AddUniqueDynamic(this, &ThisClass::StartSpawnPickupTimer);
			// 
			// 
			if (!SpawnedPickup->OnSpawnedPickupDisabled.IsBound())
			{
				SpawnedPickup->OnSpawnedPickupDisabled.BindUObject(this, &ThisClass::StartSpawnPickupTimer);
			}
		}

		//SpawnedPickup->FinishSpawning(GetActorTransform());
	}
}

void APickupSpawnPoint::SpawnPickupTimerFinished()
{
	if (HasAuthority())
	{
		SpawnPickup();
	}
}

void APickupSpawnPoint::StartSpawnPickupTimer(AActor* DestroyedActor)
{
	const float SpawnTime = FMath::FRandRange(SpawnPickupTimeMin, SpawnPickupTimeMax);
	GetWorldTimerManager().SetTimer(SpawnPickupTimer, this, &ThisClass::SpawnPickupTimerFinished, SpawnTime);
}



