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
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->SetAutoActivate(true);
}

void AProjectileGrenade::PostLoad()
{
	Super::PostLoad();

	ProjectileMovementComponent->SetIsReplicated(false);
}

void AProjectileGrenade::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Display, TEXT("ProjectileGrenade BeginPlay"));

	if (bIsNotPoolable)
	{
		SpawnTrailSystem();
		StartDestroyTimer();

		ProjectileMovementComponent->OnProjectileBounce.AddUniqueDynamic(this, &ThisClass::OnBounce);

		CollisionBox->IgnoreActorWhenMoving(GetOwner(), true);
		SetIsActive(true);
	}

}

void AProjectileGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("ProjectileGrenade tick"));
}

void AProjectileGrenade::Destroyed()
{
	ExplodeDamage();
	FHitResult Result;
	Result.ImpactPoint = GetActorLocation();

	Super::Destroyed();

}

void AProjectileGrenade::SetIsActive(bool InIsActive)
{
	Super::SetIsActive(InIsActive);

	if (InIsActive)
	{
		SpawnTrailSystem();
		StartDestroyTimer();

		ProjectileMovementComponent->OnProjectileBounce.AddUniqueDynamic(this, &ThisClass::OnBounce);

		CollisionBox->IgnoreActorWhenMoving(GetOwner(), true);

		ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
		ProjectileMovementComponent->Activate(true);
	}
	else
	{
		ProjectileMovementComponent->SetUpdatedComponent(nullptr);
		ProjectileMovementComponent->Activate(false);
	}
}

void AProjectileGrenade::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if (BounceSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BounceSound, GetActorLocation());
	}
}

void AProjectileGrenade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

void AProjectileGrenade::SetProjectileMovementVelocity(const FVector& InVelocity)
{
	ProjectileMovementComponent->Velocity = InVelocity.GetSafeNormal() * ProjectileMovementComponent->MaxSpeed;
}