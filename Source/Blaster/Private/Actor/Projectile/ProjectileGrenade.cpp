// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/ProjectileGrenade.h"

// Components
#include "Components/BoxComponent.h"

// GameFramework
#include "GameFramework/ProjectileMovementComponent.h"

// Sound
#include "Sound/SoundCue.h"

#include "Field/FieldSystemComponent.h"

// Kismet
#include "Kismet/GameplayStatics.h"


AProjectileGrenade::AProjectileGrenade()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade Mesh"));
	ProjectileMesh->SetupAttachment(GetRootComponent());
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true; // 총알이 속도에 맞춰 회전을 유지함 중력으로 인한 FallOff 를 추가하면 경로 궤적에 따라 회전함.
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->bShouldBounce = true;
}

void AProjectileGrenade::BeginPlay()
{
	AActor::BeginPlay();

	//UE_LOG(LogTemp, Display, TEXT("ProjectileGrenade BeginPlay"));

	SpawnTrailSystem();
	StartDestroyTimer();

	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &ThisClass::OnBounce);

	CollisionBox->IgnoreActorWhenMoving(GetOwner(), true);
}

void AProjectileGrenade::Destroyed()
{
	ExplodeDamage();
	FHitResult Result;
	Result.ImpactPoint = GetActorLocation();

	Super::Destroyed();
}

void AProjectileGrenade::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if (BounceSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BounceSound, GetActorLocation());
	}
}
