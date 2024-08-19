// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/ProjectileRocket.h"

// Components
#include "Components/BoxComponent.h"

// Sound
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Kismet
#include "Kismet/GameplayStatics.h"

// Niagara
#include "NiagaraComponent.h"
#include "NiagaraSystemInstanceController.h"

// FieldSystem
#include "Field/FieldSystemComponent.h"

#include "Components/RocketMovementComponent.h"


AProjectileRocket::AProjectileRocket()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket Mesh"));
	ProjectileMesh->SetupAttachment(GetRootComponent());
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RocketMovementComponent = CreateDefaultSubobject<URocketMovementComponent>(TEXT("RocketMovementComponent"));
	RocketMovementComponent->bRotationFollowsVelocity = true;
	RocketMovementComponent->SetIsReplicated(false);
	RocketMovementComponent->SetAutoActivate(false);
}

void AProjectileRocket::Destroyed()
{ 
}

void AProjectileRocket::SetIsActive(bool InIsActive)
{
	Super::SetIsActive(InIsActive);

	if (InIsActive)
	{
		if (!HasAuthority()) // 로켓탄두만큼은 클라도 OnHit Event 가 작동하도록 설정.
		{
			CollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnHit);
		}

		SpawnTrailSystem();

		if (ProjectileLoop && LoopingSoundAttenuation)
		{
			ProjectileLoopComponent = UGameplayStatics::SpawnSoundAttached(ProjectileLoop, GetRootComponent(), FName(), GetActorLocation(), EAttachLocation::KeepWorldPosition, false, 1.f, 1.f, 0.f, LoopingSoundAttenuation, (USoundConcurrency*)nullptr, false);
		}

		RocketMovementComponent->SetUpdatedComponent(RootComponent);
		RocketMovementComponent->Activate(true);
	}
	else
	{
		RocketMovementComponent->SetUpdatedComponent(nullptr);
		RocketMovementComponent->Activate(false);
	}
}

void AProjectileRocket::BeginPlay()
{
	Super::BeginPlay();

	//if (!HasAuthority()) // 로켓탄두만큼은 클라도 OnHit Event 가 작동하도록 설정.
	//{
	//	CollisionBox->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	//}

	//SpawnTrailSystem();

	//if (ProjectileLoop && LoopingSoundAttenuation)
	//{
	//	ProjectileLoopComponent = UGameplayStatics::SpawnSoundAttached(ProjectileLoop, GetRootComponent(), FName(), GetActorLocation(), EAttachLocation::KeepWorldPosition, false, 1.f, 1.f, 0.f, LoopingSoundAttenuation, (USoundConcurrency*)nullptr, false);
	//}
}

void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Display, TEXT("onhit"));



	if (GetOwner() == OtherActor)
	{
		//UE_LOG(LogTemp, Error, TEXT("Rocket Owner : %s"), *GetOwner()->GetName());
		CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
			}), 0.1f, false);
		return;
	}

	ExplodeDamage();

	StartDestroyTimer();

	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}

	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	if (ProjectileMesh)
	{
		ProjectileMesh->SetVisibility(false);
	}

	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (TrailSystemComponent && TrailSystemComponent->GetSystemInstanceController())
	{
		//TrailSystemComponent->GetSystemInstance()->Deactivate();
		TrailSystemComponent->GetSystemInstanceController()->Deactivate();
		//TrailSystemComponent->Deactivate();
	}

	if (ProjectileLoopComponent && ProjectileLoopComponent->IsPlaying())
	{
		ProjectileLoopComponent->Stop();
	}

	//Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AProjectileRocket::SetProjectileMovementVelocity(const FVector& InVelocity)
{
	RocketMovementComponent->Velocity = InVelocity.GetSafeNormal() * RocketMovementComponent->MaxSpeed;
}
