// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/Projectile.h"

// Components
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Kismets
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Particles
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

// Sound
#include "Sound/SoundCue.h"

// Interfaces
#include "Interfaces/HitInterface.h"
#include "Interfaces/TeamInterface.h"

// FieldSystem
#include "Field/FieldSystemComponent.h"

// Niagara
#include "NiagaraFunctionLibrary.h"

#include "Blaster/Blaster.h"

#include "PlayerController/BlasterPlayerController.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bReplicates = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);

	int a = 1;

	//CollisionBox->SetGenerateOverlapEvents(true);
	//CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	//CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	//CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);

	//CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);


	//CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	//CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	FieldSystemComponent = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("FieldSystemComponent"));
	ensure(FieldSystemComponent);
	FieldSystemComponent->SetupAttachment(RootComponent);

	CollisionBox->SetCollisionProfileName(PROFILE_Projectile);
	CollisionBox->SetCollisionObjectType(ECC_Projectile);

}

void AProjectile::Destroyed()
{
	//UE_LOG(LogTemp, Display, TEXT("Destroyed"));
	// 
	if (bIsNotPoolable)
	{
		Super::Destroyed();
	}

	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}

	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	Deactivate();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Display, TEXT("Projectile Tick"));
}


void AProjectile::SetIsActive(bool InIsActive)
{
	Super::SetIsActive(InIsActive);

	if (InIsActive)
	{
		if (Tracer)
		{
			TracerComponent = UGameplayStatics::SpawnEmitterAttached(Tracer, CollisionBox, TEXT(""), GetActorLocation() + GetActorForwardVector() * 50.f, GetActorRotation(), EAttachLocation::KeepWorldPosition);
			//TracerComponent = UGameplayStatics::SpawnEmitterAttached(Tracer, ProjectileMesh, TEXT("TracerSocket"), ProjectileMesh->GetComponentLocation(), GetActorRotation(), EAttachLocation::SnapToTarget);
		}

		if (HasAuthority())
		{
			CollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnHit);
		}
	}

}

void AProjectile::SetProjectileMovementVelocity(const FVector& InVelocity)
{
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickInterval(0.1f);
	if (ProjectileMovementComponent) ProjectileMovementComponent->SetComponentTickInterval(0.01f);

	//ProjectileMovementComponent->GetComponentTickInterval();

	//UE_LOG(LogTemp, Display, TEXT("Getowner : %x"), GetOwner());
	//UE_LOG(LogTemp, Display, TEXT("getinstigator : %x"), GetInstigator());

	CollisionBox->MoveIgnoreActors.Add(GetOwner());
	CollisionBox->IgnoreActorWhenMoving(GetOwner(), true);

	if (bIsNotPoolable)
	{
		SetIsActive(true);


		//UE_LOG(LogTemp, Error, TEXT("GetOwner : %s"), *GetOwner()->GetName());
		//CollisionBox->IgnoreActorWhenMoving(GetOwner(), true);
	}



}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Display, TEXT("OnHit : %s"), *OtherActor->GetName());

	//ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	//if (BlasterCharacter)
	//{
	//	BlasterCharacter->MulticastHit();
	//}

	//UE_LOG(LogTemp, Display, TEXT("%s"), *OtherComp->GetName());



	IHitInterface* HitActor = Cast<IHitInterface>(OtherActor);
	if (HitActor)
	{

		HitActor->IGetHit(GetActorLocation(), Hit);
		ApplyForce(FieldSystemComponent, Hit);

		ABlasterPlayerController* BPC = Cast<ABlasterPlayerController>(GetInstigatorController());

		if (BPC && BPC->IsLocalPlayerController())
		{

			//UE_LOG(LogTemp, Error, TEXT("BoneName : %s"), *Hit.BoneName.ToString());
			//UE_LOG(LogTemp, Error, TEXT("MyBoneName : %s"), *Hit.MyBoneName.ToString());
			//UE_LOG(LogTemp, Error, TEXT("Component : %s"), *Hit.Component->GetName());


			if (Hit.BoneName.ToString() == TEXT("neck_02") || Hit.BoneName.ToString() == TEXT("head"))
			{
				BPC->PlayHitNoticeAnim(TEXT("Head"));
			}
			else
			{
				BPC->PlayHitNoticeAnim(TEXT("Body"));
			}
		}
	}
	else
	{
		if (BulletHole)
		{
			float VisibleDistance = 30.f;
			UGameplayStatics::SpawnDecalAttached(BulletHole, FVector(VisibleDistance, 3.f, 3.f), OtherComp, TEXT(""), Hit.ImpactPoint, Hit.ImpactNormal.ToOrientationRotator(), EAttachLocation::KeepWorldPosition, 10.f);

		}
	}


	if (bIsNotPoolable)
	{
		Destroy();
	}
	else
	{
		Destroyed();
	}

}

void AProjectile::ExplodeDamage()
{
	APawn* FiringPawn = GetInstigator();
	if (FiringPawn && HasAuthority())
	{
		AController* FiringController = FiringPawn->GetController() == nullptr ? FiringPawn->AIControllerClass.GetDefaultObject() : FiringPawn->GetController();
		if (FiringController)
		{
			//UE_LOG(LogTemp, Display, TEXT("%s : ExplodeDamage"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString());

			TArray<AActor*> ActorsToIgnore;
			//ActorsToIgnore.Add(GetInstigator());
			UGameplayStatics::ApplyRadialDamageWithFalloff(this, Damage, 10.f, GetActorLocation(), DamageInnerRadius, DamageOuterRadius, 1.f, UDamageType::StaticClass(), ActorsToIgnore, this, FiringController);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Projectile/ExplodeDamage/FiringController is null"));
		}

	}

	float Magnitude = 10'0000'0000.f;
	FieldSystemComponent->ApplyStrainField(true, GetActorLocation(), DamageOuterRadius, Magnitude, 2);

}

void AProjectile::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ThisClass::DestroyTimerFinished, DestroyTime);
}

void AProjectile::DestroyTimerFinished()
{
	if (bIsNotPoolable)
	{
		Destroy();
	}
	else
	{
		Destroyed();
	}
}

void AProjectile::ApplyForce(UFieldSystemComponent* InFieldSystemComponent, const FHitResult& InHitResult)
{
	if (InFieldSystemComponent)
	{
		//UE_LOG(LogTemp, Display, TEXT("%s : FieldSystem Actived"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString());

		float ForceDamage = 1000'00000.f;
		//float ForceDamage = 100.f;

		float ForceRadius = 50.f;

		//UE_LOG(LogTemp, Display, TEXT("Normal : %f, %f, %f"), -OutHitResult.Normal.X, -OutHitResult.Normal.Y, -OutHitResult.Normal.Z);
		InFieldSystemComponent->ApplyStrainField(true, InHitResult.ImpactPoint, ForceRadius, ForceDamage, 0);
		//UKismetSystemLibrary::DrawDebugArrow(this, OutHitResult.ImpactPoint, OutHitResult.ImpactPoint + OutHitResult.ImpactNormal * 50.f, 10.f, FLinearColor::Red, 2.f);
		//InFieldSystemComponent->ApplyLinearForce(true, -InHitResult.ImpactNormal, ForceDamage);
	}
}

void AProjectile::SpawnTrailSystem()
{

	if (TrailSystem)
	{
		TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailSystem, GetRootComponent(), FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
	}
}