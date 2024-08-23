// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PickupSpawnPoint.h"
#include "Item/Pickups/Pickup.h"
#include "GameState/BlasterGameState.h"
#include "Components/ObjectPoolComponent.h"

// Sets default values
APickupSpawnPoint::APickupSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	//UE_LOG(LogTemp, Display, TEXT("SpawnPickup"));
	int32 NumPickupClasses = PickupClasses.Num();
	if (NumPickupClasses > 0)
	{
		int32 Selection = FMath::RandRange(0, NumPickupClasses - 1);

		//SpawnedPickup = GetWorld()->SpawnActorDeferred<APickup>(PickupClasses[Selection], GetActorTransform());

		if (GetWorld() && GetWorld()->GetGameState<ABlasterGameState>())
		{
			SpawnedPickup = Cast<APickup>(GetWorld()->GetGameState<ABlasterGameState>()->GetComponentByClass<UObjectPoolComponent>()->GetSpawnedObjectDeferred(GetActorTransform(), PickupClasses[Selection]));
		}


		if (!SpawnedPickup)
		{
			UE_LOG(LogTemp, Error, TEXT("SpawnedPickup null"));
		}

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
		GetWorld()->GetGameState<ABlasterGameState>()->GetComponentByClass<UObjectPoolComponent>()->FinishSpawning(GetActorTransform(), PickupClasses[Selection]);
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
	//UE_LOG(LogTemp, Display, TEXT("StartSpawnPickupTimer"));
	const float SpawnTime = FMath::FRandRange(SpawnPickupTimeMin, SpawnPickupTimeMax);
	GetWorldTimerManager().SetTimer(SpawnPickupTimer, this, &ThisClass::SpawnPickupTimerFinished, SpawnTime);
}



