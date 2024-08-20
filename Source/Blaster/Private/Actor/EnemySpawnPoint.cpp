// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EnemySpawnPoint.h"
#include "Characters/Enemy/Enemy.h"
#include "Components/ObjectPoolComponent.h"
#include "GameState/BlasterGameState.h"

// Sets default values
AEnemySpawnPoint::AEnemySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	StartSpawnPickupTimer(static_cast<AActor*>(nullptr));
}

// Called every frame
void AEnemySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawnPoint::SpawnPickup()
{
	int32 NumPickupClasses = EnemyClass.Num();
	if (NumPickupClasses > 0)
	{
		int32 Selection = FMath::RandRange(0, NumPickupClasses - 1);

		//SpawnedEnemy = GetWorld()->SpawnActorDeferred<AEnemy>(EnemyClass[Selection], GetActorTransform());

		UE_LOG(LogTemp, Display, TEXT("Spawned"));
		SpawnedEnemy = Cast<AEnemy>(GetWorld()->GetGameState<ABlasterGameState>()->GetComponentByClass<UObjectPoolComponent>()->GetSpawnedCharacter(GetActorTransform(), EnemyClass[Selection]));

		if (!SpawnedEnemy)
		{
			UE_LOG(LogTemp, Display, TEXT("SpawnFailed"));
		}

		//UE_LOG(LogTemp, Display, TEXT("SpawnEnemy : %x"), SpawnedEnemy.Get());

		//SpawnedEnemy->SpawnDefaultController();

		if (HasAuthority() && SpawnedEnemy)
		{
			//SpawnedPickup->OnDestroyed.AddUniqueDynamic(this, &ThisClass::StartSpawnPickupTimer);
			// 
			// 
			if (!SpawnedEnemy->OnSpawnedEnemyDisabled.IsBound())
			{
				SpawnedEnemy->OnSpawnedEnemyDisabled.BindUObject(this, &ThisClass::StartSpawnPickupTimer);
			}
		}

		//SpawnedEnemy->FinishSpawning(GetActorTransform());
	}
}

void AEnemySpawnPoint::SpawnPickupTimerFinished()
{
	if (HasAuthority())
	{
		SpawnPickup();
	}
}

void AEnemySpawnPoint::StartSpawnPickupTimer(AActor* DestroyedActor)
{
	const float SpawnTime = FMath::FRandRange(SpawnPickupTimeMin, SpawnPickupTimeMax);
	GetWorldTimerManager().SetTimer(SpawnPickupTimer, this, &ThisClass::SpawnPickupTimerFinished, SpawnTime);
}

