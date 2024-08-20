// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EnemySpawnPoint.h"
#include "Characters/Enemy/Enemy.h"
#include "Components/ObjectPoolComponent.h"
#include "GameState/BlasterGameState.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

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
	if (EnemyClass.Num() > 0)
	{

		MustRandomSpawner();


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

void AEnemySpawnPoint::MustRandomSpawner()
{

	TSet<int32> Checker;
	int32 Selection = -1;
	int32 LoopThreshold = 50;
	int32 Count = 0;

	Checker.Add(Selection);

	while (Checker.Contains(Selection))
	{
		Selection = FMath::RandRange(0, EnemyClass.Num() - 1);

		//UE_LOG(LogTemp, Display, TEXT("Spawned"));

		FTransform SpawnT(GetActorRotation(), GetActorLocation() + FVector(0.f, 0.f, 85.f));

		SpawnedEnemy = Cast<AEnemy>(GetWorld()->GetGameState<ABlasterGameState>()->GetComponentByClass<UObjectPoolComponent>()->GetSpawnedCharacter(SpawnT, EnemyClass[Selection]));

		if (SpawnEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, SpawnT);
		}
		else if (SpawnEffectNiagara)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, SpawnEffectNiagara, SpawnT.GetLocation());
		}





		//GetActorTransform().DebugPrint();

		if (!SpawnedEnemy)
		{
			//UE_LOG(LogTemp, Display, TEXT("SpawnFailed"));
			Checker.Add(Selection);
		}
		else
		{
			break;
		}

		if (Count++ > LoopThreshold)
		{
			break;
		}
	}
}

