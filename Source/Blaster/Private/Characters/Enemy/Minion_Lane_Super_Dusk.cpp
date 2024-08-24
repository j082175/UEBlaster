// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/Minion_Lane_Super_Dusk.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
//#include "BodySetupCore.h"
#include "PhysicsEngine/BodySetup.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/AttributeComponent.h"

// FieldSystem
#include "Field/FieldSystemComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Sound/SoundCue.h"

#include "Actor/Projectile/Projectile.h"

#include "Actor/ParticleSpawner.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController/EnemyAIController.h"
#include "BlasterTypes/BlackboardKeys.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/Blaster.h"

#include "Components/ObjectPoolComponent.h"
#include "GameState/BlasterGameState.h"

//#include "Interfaces/HitInterface.h"

AMinion_Lane_Super_Dusk::AMinion_Lane_Super_Dusk()
{
	FieldSystemComponent = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("FieldSystemComponent"));
	FieldSystemComponent->SetupAttachment(RootComponent);

	Super::InitializeCollisionStates();
	InitializeDefaults();
}

void AMinion_Lane_Super_Dusk::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AMinion_Lane_Super_Dusk::BeginPlay()
{
	Super::BeginPlay();

	InitializeCollisionStates();
	InitializeDelegates();

	//GetController()->SetActorTickInterval(0.01f);

	FTimerHandle H;
	GetWorldTimerManager().SetTimer(H, FTimerDelegate::CreateLambda([&]()
		{
			if (GetController()) GetController()->SetActorTickInterval(0.01f);
		}), 1.f, false);
}

void AMinion_Lane_Super_Dusk::OnMontageEndedFunc(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnMontageEndedFunc(Montage, bInterrupted);

	if (!bInterrupted)
	{
		//if (CombatState == ECombatState::ECS_Unoccupied)
		//{
		//	bCheckLeftRightAttack = false;
		//}
	}


}

void AMinion_Lane_Super_Dusk::InitializeCollisionStates()
{
	HitCollisionCapsules[TEXT("weapon_r")]->BodyInstance.SetInstanceNotifyRBCollision(true);
	HitCollisionCapsules[TEXT("weapon_l")]->BodyInstance.SetInstanceNotifyRBCollision(true);

	if (HitCollisionCapsules[TEXT("weapon_r")]) HitCollisionCapsules[TEXT("weapon_r")]->SetCollisionResponseToChannel(ECC_CanMorV, ECR_Ignore);
	if (HitCollisionCapsules[TEXT("weapon_l")]) HitCollisionCapsules[TEXT("weapon_l")]->SetCollisionResponseToChannel(ECC_CanMorV, ECR_Ignore);
}

void AMinion_Lane_Super_Dusk::InitializeDefaults()
{
	Super::InitializeDefaults();

	CharacterState = ECharacterState::ECS_EquippedCapsuleWeapon;
}

void AMinion_Lane_Super_Dusk::InitializeDelegates()
{
	Super::InitializeDelegates();

	HitCollisionCapsules[TEXT("weapon_r")]->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnRightWeaponCapsuleBeginOverlap);
	HitCollisionCapsules[TEXT("weapon_l")]->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnLeftWeaponCapsuleBeginOverlap);

	HitCollisionCapsules[TEXT("weapon_r")]->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnWeaponCapsuleHit);
	HitCollisionCapsules[TEXT("weapon_l")]->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnWeaponCapsuleHit);
}

void AMinion_Lane_Super_Dusk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CombatState == ECombatState::ECS_Unoccupied) bCheckLeftRightAttack = false;

	//UE_LOG(LogTemp, Display, TEXT("bCheckLeftRightAttack : %d"), bCheckLeftRightAttack);
	//UE_LOG(LogTemp, Display, TEXT("CombatState : %s"), *UEnum::GetDisplayValueAsText(CombatState).ToString());
	//ECollisionResponse R = HitCollisionCapsules[TEXT("weapon_r")]->GetCollisionResponseToChannel(ECC_SkeletalMesh);
	//ECollisionEnabled::Type T = HitCollisionCapsules[TEXT("weapon_r")]->GetCollisionEnabled();

	//UE_LOG(LogTemp, Display, TEXT("Response : %s, Type : %s"), *UEnum::GetDisplayValueAsText(R).ToString(), *UEnum::GetDisplayValueAsText(T).ToString());
	//UE_LOG(LogTemp, Display, TEXT("bCheckLeftRightAttack : %d"), bCheckLeftRightAttack);
}

void AMinion_Lane_Super_Dusk::OnRightWeaponCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;


	//UE_LOG(LogTemp, Error, TEXT("OnRightWeaponCapsuleBeginOverlap : %d"), bCheckLeftRightAttack);


	if (bCheckLeftRightAttack) return;

	ICanParryInterface* ParryActor = Cast<ICanParryInterface>(OtherActor);
	if (ParryActor && ParryActor->IIsParring())
	{
		if (ParryActor->ICheckParry(this))
		{
			bCheckLeftRightAttack = true;
			return;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ICheckParry false"));
		}
	}



	//UE_LOG(LogTemp, Display, TEXT("%f"), OtherActor->GetActorForwardVector().Dot(this->GetActorForwardVector()));

	if (HasAuthority()) UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

	IHitInterface* HitActor = Cast<IHitInterface>(OtherActor);
	if (HitActor)
	{
		//UE_LOG(LogTemp, Display, TEXT("HitActor : %s"), *OtherActor->GetName());
		FVector ImpactPoint = HitCollisionCapsules[TEXT("weapon_r")]->GetComponentLocation();

		HitActor->IGetHit(ImpactPoint);


		//FVector Location = HitCollisionCapsules[TEXT("weapon_r")]->GetComponentLocation();
		//float Radius = HitCollisionCapsules[TEXT("weapon_r")]->GetScaledCapsuleRadius();
		//float HalfHeight = HitCollisionCapsules[TEXT("weapon_r")]->GetScaledCapsuleHalfHeight();
		//FRotator Rot = HitCollisionCapsules[TEXT("weapon_r")]->GetComponentRotation();

		//DrawDebugSphere(GetWorld(), HitCollisionCapsules[TEXT("weapon_r")]->GetComponentLocation(), 10.f, 12, FColor::Green, false, 2.f);
		//DrawDebugSphere(GetWorld(), HitCollisionCapsules[TEXT("weapon_l")]->GetComponentLocation(), 10.f, 12, FColor::Red, false, 2.f);

		//DrawDebugCapsule(GetWorld(), Location, HalfHeight, Radius, Rot.Quaternion(), FColor::White, false, 1.f);

		float ForceDamage = 1000'00000.f;
		float ForceRadius = 50.f;
		FieldSystemComponent->ApplyStrainField(true, ImpactPoint, ForceRadius, ForceDamage, 0);

		if (ImpactParticle)
		{
			//UE_LOG(LogTemp, Display, TEXT("Spawn ImpactParticle"));
			FTransform ImpactT(ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, ImpactT);
		}

		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, ImpactPoint);
		}

	}

}

void AMinion_Lane_Super_Dusk::OnLeftWeaponCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;
	//UE_LOG(LogTemp, Error, TEXT("OnLeftWeaponCapsuleBeginOverlap"));

	if (bCheckLeftRightAttack) return;

	ICanParryInterface* ParryActor = Cast<ICanParryInterface>(OtherActor);
	if (ParryActor && ParryActor->IIsParring())
	{
		if (ParryActor->ICheckParry(this))
		{
			bCheckLeftRightAttack = true;
			return;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ICheckParry false"));
		}
	}


	if (HasAuthority()) UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

	IHitInterface* HitActor = Cast<IHitInterface>(OtherActor);
	if (HitActor)
	{
		//UE_LOG(LogTemp, Display, TEXT("HitActor : %s"), *OtherActor->GetName());
		FVector ImpactPoint = HitCollisionCapsules[TEXT("weapon_l")]->GetComponentLocation();

		HitActor->IGetHit(ImpactPoint);
		float ForceDamage = 1000'00000.f;
		float ForceRadius = 100.f;

		FieldSystemComponent->ApplyStrainField(true, ImpactPoint, ForceRadius, ForceDamage, 0);
		if (ImpactParticle)
		{
			FTransform ImpactT(ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, ImpactT);
		}

		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, ImpactPoint);
		}
	}
}

void AMinion_Lane_Super_Dusk::OnWeaponCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == this) return;

	//UE_LOG(LogTemp, Error, TEXT("OnWeaponCapsuleHit"));

	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.f, 12, FColor::Red);

}

void AMinion_Lane_Super_Dusk::OnPlayMontageNotifyBeginFunc(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::OnPlayMontageNotifyBeginFunc(NotifyName, BranchingPointPayload);

#define CAST_START TEXT("CastStart")
#define SKY_LASER_ATTACK TEXT("SkyLaserAttack")

	if (!HasAuthority()) return;

	if (NotifyName == CAST_START)
	{
		//UE_LOG(LogTemp, Display, SKY_LASER_ATTACK);
		InitLaserSpawner();
	}
	else if (NotifyName == SKY_LASER_ATTACK)
	{
		//UE_LOG(LogTemp, Display, SKY_LASER_ATTACK);
		LaunchLaser();
	}
}

void AMinion_Lane_Super_Dusk::InitLaserSpawner()
{
	for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; iter++)
	{

		APawn* OwingPawn = iter->Get()->AcknowledgedPawn;

		if (OwingPawn)
		{
			//UE_LOG(LogTemp, Display, TEXT("Spawning"));
			FTransform SkyLaserSpawnPoint = FTransform(FRotator(-90.f, 0.f, 0.f), OwingPawn->GetActorLocation() + OwingPawn->GetActorUpVector() * 1000.f);

			AParticleSpawner* Spawner = GetWorld()->SpawnActor<AParticleSpawner>(ParticleSpawner, SkyLaserSpawnPoint);

			if (Spawner)
			{
				Spawner->SetOwner(OwingPawn);
				Spawner->SetInstigator(this);

				//UGameplayStatics::SpawnEmitterAttached(SkyLaserSpawningParticle, Spawner->GetRootComponent(), TEXT(""), Spawner->GetActorLocation(), Spawner->GetActorRotation(), EAttachLocation::SnapToTarget);

				ParticleSpawnerArr.Add(Spawner);
			}

		}
	}
}

void AMinion_Lane_Super_Dusk::LaunchLaser()
{
	for (size_t i = 0; i < ParticleSpawnerArr.Num(); i++)
	{
		ParticleSpawnerArr[i]->Launch();
	}
	ParticleSpawnerArr.Empty();

	//if (SkyLaserParticle)
	//{

	//	int32 count = 0;
	//	for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; iter++)
	//	{
	//		APawn* OwingPawn = iter->Get()->AcknowledgedPawn;

	//		if (OwingPawn)
	//		{
	//			FTransform SkyLaserSpawnPoint = FTransform(FRotator(-90.f, 0.f, 0.f), OwingPawn->GetActorLocation() + OwingPawn->GetActorUpVector() * 1000.f);

	//			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SkyLaserParticle, SkyLaserSpawnPoint);
	//			AProjectile* Explosive = GetWorld()->SpawnActor<AProjectile>(ProjectileToSpawn, SkyLaserSpawnPoint);
	//			if (Explosive)
	//			{
	//				Explosive->SetOwner(this);
	//				Explosive->SetInstigator(GetInstigator());

	//				ParticleSpawnerArr[count]->Destroy();
	//				++count;
	//			}
	//		}



	//	}

	//}

	//ParticleSpawnerArr.Empty();


}

void AMinion_Lane_Super_Dusk::RandomAttack(int32 Index, const FString& AttackType)
{
	Super::RandomAttack(Index, AttackType);

	if (AttackType == TEXT("Teleport")) // Teleport Attack
	{

		TeleportAttack();




	}
}

void AMinion_Lane_Super_Dusk::TeleportAttack()
{
	TArray<APawn*> PlayerPawnList;

	for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; iter++)
	{
		APawn* OwingPawn = iter->Get()->AcknowledgedPawn;

		if (OwingPawn)
		{
			PlayerPawnList.Add(OwingPawn);
		}
	}

	if (!PlayerPawnList.IsEmpty())
	{
		TeleportTo = PlayerPawnList[0];
	}
	else
	{
		return;
	}

	if (TeleportStartParticle) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportStartParticle, GetActorTransform());
	if (TeleportStartSound) UGameplayStatics::PlaySoundAtLocation(this, TeleportStartSound, GetActorLocation());

	CustomTimeDilation = 0.001f;

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			//TeleportAttack(OwingPawn->GetActorLocation());
			SetActorHiddenInGame(false);
			SetActorEnableCollision(true);
			if (TeleportTo)
			{
				GetRootComponent()->SetWorldLocation(TeleportTo->GetActorLocation());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("TeleportTo is NULL"));
			}

			if (TeleportEndParticle) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEndParticle, GetActorTransform());
			if (TeleportEndSound) UGameplayStatics::PlaySoundAtLocation(this, TeleportEndSound, GetActorLocation());
			UGameplayStatics::PlayWorldCameraShake(this, GroundCameraShakeClass, GetActorLocation(), 0.f, 500.f);

			
			CustomTimeDilation = 1.f;


		}), TeleportDelay, false);
}

