// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/ProjectileBullet.h"

// GameFrameworks
#include "GameFramework/ProjectileMovementComponent.h"

// Components
#include "Components/LagCompensationComponent.h"

// Kismet
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"

#include "PlayerController/BlasterPlayerController.h"
#include "Characters/BlasterCharacter.h"
#include "DamageType/DamageType_Projectile.h"
#include "Blaster/Blaster.h"

AProjectileBullet::AProjectileBullet()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Mesh"));
	ProjectileMesh->SetupAttachment(GetRootComponent());
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true; // 총알이 속도에 맞춰 회전을 유지함 중력으로 인한 FallOff 를 추가하면 경로 궤적에 따라 회전함.
	ProjectileMovementComponent->SetIsReplicated(false);
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = InitialSpeed;


	ProjectileMovementComponent->SetAutoActivate(false);
	// 
	//ProjectileMovementComponent->BounceVelocityStopSimulatingThreshold = 0.f;

}

void AProjectileBullet::Destroyed()
{
	Super::Destroyed();

	ProjectileMovementComponent->SetUpdatedComponent(nullptr);
	ProjectileMovementComponent->Activate(false);

}

void AProjectileBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("ProjectileBullet tick"));
}

void AProjectileBullet::SetIsActive(bool InIsActive)
{
	Super::SetIsActive(InIsActive);

	if (InIsActive)
	{

		ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
		ProjectileMovementComponent->Activate(true);
	}
	else
	{
		ProjectileMovementComponent->SetUpdatedComponent(nullptr);
		ProjectileMovementComponent->Activate(false);
	}


}

void AProjectileBullet::SetProjectileMovementVelocity(const FVector& InVelocity)
{
	ProjectileMovementComponent->Velocity = InVelocity.GetSafeNormal() * ProjectileMovementComponent->MaxSpeed;
}

#if WITH_EDITOR
void AProjectileBullet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	//UE_LOG(LogTemp, Display, TEXT("PostEditChangeProperty"));
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, InitialSpeed))
	{
		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->InitialSpeed = InitialSpeed;
			ProjectileMovementComponent->MaxSpeed = InitialSpeed;
		}
	}
}
#endif //WITH_EDITOR


void AProjectileBullet::BeginPlay()
{
	Super::BeginPlay();

	//FPredictProjectilePathParams PathParams;
	//PathParams.bTraceWithChannel = true;
	//PathParams.bTraceWithCollision = true;
	//PathParams.DrawDebugTime = 5.f;
	//PathParams.DrawDebugType = EDrawDebugTrace::ForDuration;
	//PathParams.LaunchVelocity = GetActorForwardVector() * InitialSpeed;
	//PathParams.MaxSimTime = 4.f;
	//PathParams.ProjectileRadius = 5.f;
	//PathParams.SimFrequency = 60.f;
	//PathParams.StartLocation = GetActorLocation();
	//PathParams.TraceChannel = ECollisionChannel::ECC_Visibility;
	//PathParams.ActorsToIgnore.Add(this);

	//FPredictProjectilePathResult PathResult;

	//UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);
}

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Display, TEXT("GetOwner : %s"), *GetOwner()->GetName());

	ITeamInterface* TI = Cast<ITeamInterface>(OtherActor);

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	ITeamInterface* T2 = Cast<ITeamInterface>(GetOwner());
	if (TI && T2 && TI->IGetTeam() == T2->IGetTeam())
	{
		if (bIsNotPoolable)
		{
			Super::Destroy();
		}
		else
		{
			ProjectileMovementComponent->SetUpdatedComponent(nullptr);
			ProjectileMovementComponent->Activate(false);
			Deactivate();
		}

		return;
	}

	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);


	if (OwnerCharacter)
	{

		if (OwnerCharacter->HasAuthority() && !bUseServerSideRewind)
		{

			float DamageToCause = Hit.BoneName.ToString() == TEXT("head") ? RandomHeadShotDamage : RandomDamage;

			UGameplayStatics::ApplyDamage(OtherActor, DamageToCause, GetInstigatorController(), this, UDamageType_Projectile::StaticClass());

			return;
		}


		ABlasterPlayerController* OwnerController = Cast<ABlasterPlayerController>(OwnerCharacter->GetController());
		if (OwnerController)
		{
			ACharacterBase* HitCharacter = Cast<ACharacterBase>(OtherActor);
			if (bUseServerSideRewind && OwnerCharacter->GetComponentByClass<ULagCompensationComponent>() && OwnerCharacter->IsLocallyControlled() && HitCharacter)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Autonomous : ProjectileBullet OnHit"));

				OwnerCharacter->GetComponentByClass<ULagCompensationComponent>()->ProjectileServerScoreRequest(HitCharacter, TraceStart, InitialVelocity, OwnerController->GetServerTime() - OwnerController->SingleTripTime);
			}
		}
	}





}

