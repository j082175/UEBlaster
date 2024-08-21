// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ParticleSpawner.h"

#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Actor/Projectile/Projectile.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
AParticleSpawner::AParticleSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	SetRootComponent(ParticleSystemComponent);
}

void AParticleSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ThisClass, SkyLaserParticle);
}

// Called when the game starts or when spawned
void AParticleSpawner::BeginPlay()
{
	Super::BeginPlay();

	ParticleSystemComponent->SetActive(true);

	if (GetOwner())
	{
		FRotator Rotation(-90.f, 0.f, 0.f);
		SkyLaserSpawnPoint = FTransform(Rotation, GetOwner()->GetActorLocation() + GetOwner()->GetActorUpVector() * 1000.f);

		SetActorTransform(SkyLaserSpawnPoint);
	}

}

// Called every frame
void AParticleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetOwner())
	{
		FRotator Rotation(-90.f, 0.f, 0.f);

		SkyLaserSpawnPoint = FTransform(Rotation, GetOwner()->GetActorLocation() + GetOwner()->GetActorUpVector() * 1000.f);

		float XResult = FMath::FInterpTo(GetActorLocation().X, GetOwner()->GetActorLocation().X, DeltaTime, MoveToTargetSpeed);
		float YResult = FMath::FInterpTo(GetActorLocation().Y, GetOwner()->GetActorLocation().Y, DeltaTime, MoveToTargetSpeed);
		FVector Location(XResult, YResult, GetActorLocation().Z);

		SkyLaserSpawnPoint.SetLocation(Location);

		SetActorTransform(SkyLaserSpawnPoint);
	}
}

void AParticleSpawner::Launch()
{
	if (IsValid(SkyLaserParticle))
	{

		MulticastSpawnLaser();
		AProjectile* Explosive = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileToSpawn, SkyLaserSpawnPoint);
		if (Explosive)
		{
			Explosive->SetIsNotPoolable(true);
			Explosive->SetOwner(this);
			Explosive->SetInstigator(GetInstigator());
			//UE_LOG(LogTemp, Display, TEXT("ParticleSpawner's instigator : %s"), *GetInstigator()->GetName());

			Explosive->FinishSpawning(SkyLaserSpawnPoint);
		}

		Destroy();
	}
}

void AParticleSpawner::MulticastSpawnLaser_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SkyLaserParticle, SkyLaserSpawnPoint);
}

