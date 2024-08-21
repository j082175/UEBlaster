// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickable/Weapon/HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Characters/BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Item/Pickable/Weapon/WeaponTypes.h"
#include "Components/LagCompensationComponent.h"
#include "PlayerController/BlasterPlayerController.h"
#include "Blaster/Blaster.h"
#include "Interfaces/HitInterface.h"

#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"


void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	//UE_LOG(LogTemp, Display, TEXT("InstigatorController : %x"), InstigatorController);

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(TEXT("MuzzleFlash"));
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();

		FHitResult FireHit;
		WeaponTraceHit(Start, HitTarget, FireHit);

		if (FireHit.bBlockingHit)
		{

			IHitInterface* HitActor = Cast<IHitInterface>(FireHit.GetActor());
			if (HitActor)
			{
				//if (HasAuthority()) UE_LOG(LogTemp, Error, TEXT("HitActor : %s"), *FireHit.GetActor()->GetName());
				HitActor->IGetHit(HitTarget);
				//ApplyForce(FieldSystemComponent, FireHit);
				//UAISense_Damage::ReportDamageEvent(this, FireHit.GetActor(), GetInstigator(), Damage, GetActorLocation(), HitTarget);
			}


			if (HitActor && InstigatorController) // 오로지 데미지를 처리하는 부분만 서버가 처리.
			{
				bool bCauseAuthDamage = !bUseServerSideRewind || OwnerPawn->IsLocallyControlled();
				if (HasAuthority() && bCauseAuthDamage)
				{

					const float DamageToCause = FireHit.BoneName.ToString() == TEXT("head") ? HeadShotDamage : Damage;

					//UE_LOG(LogTemp, Display, TEXT("HitScanWeapon : HasAuthority, %s, Damage : %f"), *FireHit.GetActor()->GetName(), DamageToCause);

					UGameplayStatics::ApplyDamage(FireHit.GetActor(), DamageToCause, InstigatorController, this, UDamageType::StaticClass());


				}
				else if (!HasAuthority() && bUseServerSideRewind)
				{
					//UE_LOG(LogTemp, Display, TEXT("HitActor : %s"), *BlasterCharacter->GetName());
					BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(OwnerPawn) : BlasterOwnerCharacter;
					BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(InstigatorController) : BlasterOwnerController;
					if (BlasterOwnerController && BlasterOwnerCharacter && BlasterOwnerCharacter->GetLagCompensationComponent() && BlasterOwnerCharacter->IsLocallyControlled())
					{
						BlasterOwnerCharacter->GetLagCompensationComponent()->ServerScoreRequest(FireHit.GetActor(), Start, HitTarget, BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime);
					}
				}


			}

			if (ImpactParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
			}

			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, FireHit.ImpactPoint);
			}

			UAISense_Hearing::ReportNoiseEvent(this, FireHit.ImpactPoint, 1.f, GetInstigator());
		}

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}

		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}
}

void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
	//UE_LOG(LogTemp, Display, TEXT("WeaponTraceHit"));
	FHitResult FireHit;
	UWorld* World = GetWorld();
	if (World)
	{
		//FVector End = bUseScatter ? TraceEndWithScatter(TraceStart, HitTarget) : TraceStart + (HitTarget - TraceStart) * 1.25f;
		FVector End = TraceStart + (HitTarget - TraceStart) * 1.25f;

		//ETraceTypeQuery CanDamagedByWeapon = UEngineTypes::ConvertToTraceType(ECC_CanDamagedByWeapon);
		ETraceTypeQuery Visibility = UEngineTypes::ConvertToTraceType(ECC_Visibility);

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner());

		UKismetSystemLibrary::LineTraceSingle(this, TraceStart, End, Visibility, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);
		//World->LineTraceSingleByChannel(OutHit, TraceStart, End, ECC_CanDamagedByWeapon);
		FVector BeamEnd = End;
		if (OutHit.bBlockingHit)
		{
			BeamEnd = OutHit.ImpactPoint;
		}
		else
		{
			OutHit.ImpactPoint = End; 
		}

		DrawDebugSphere(GetWorld(), BeamEnd, 5.f, 12, FColor::Orange, false, 2.f);

		if (BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World, BeamParticles, TraceStart, FRotator::ZeroRotator, true);

			if (Beam)
			{
				Beam->SetVectorParameter(TEXT("Target"), BeamEnd);
			}

			//APooledObject* PO = Cast<UObjectPoolSubsystem>(GetGameInstance())->GetSpawnedObject(FTransform(GetActorRotation(), OutHit.ImpactPoint), PoolTest);


		}
	}
}
