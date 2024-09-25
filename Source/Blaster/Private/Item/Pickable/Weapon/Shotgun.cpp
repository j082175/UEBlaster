// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickable/Weapon/Shotgun.h"

// Interfaces
#include "Interfaces/HitInterface.h"

// Components
#include "Components/LagCompensationComponent.h"

// Engine
#include "Engine/SkeletalMeshSocket.h"

// Kismet
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Particles
#include "Particles/ParticleSystemComponent.h"

// Sounds
#include "Sound/SoundCue.h"


#include "Characters/BlasterCharacter.h"
#include "Characters/CharacterBase.h"
#include "PlayerController/BlasterPlayerController.h"

#include "Blaster.h"

//void AShotgun::Fire(const FVector& HitTarget)
//{
//	//Super::Fire(HitTarget);
//	AWeapon_Gun::Fire(HitTarget);
//
//	APawn* OwnerPawn = Cast<APawn>(GetOwner());
//	if (OwnerPawn == nullptr) return;
//	AController* InstigatorController = OwnerPawn->GetController();
//
//	//UE_LOG(LogTemp, Display, TEXT("InstigatorController : %x"), InstigatorController);
//
//	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(TEXT("MuzzleFlash"));
//	if (MuzzleFlashSocket)
//	{
//		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
//		FVector Start = SocketTransform.GetLocation();
//
//		TMap<ACharacterBase*, uint32> HitMap;
//
//		for (size_t i = 0; i < NumberOfPellets; i++)
//		{
//			//FVector End = TraceEndWithScatter(Start, HitTarget);
//			FHitResult FireHit;
//			WeaponTraceHit(Start, HitTarget, FireHit);
//
//			ACharacterBase* BlasterCharacter = Cast<ACharacterBase>(FireHit.GetActor());
//			if (BlasterCharacter && HasAuthority() && InstigatorController) // 오로지 데미지를 처리하는 부분만 서버가 처리.
//			{
//				if (HitMap.Contains(BlasterCharacter))
//				{
//					HitMap[BlasterCharacter]++;
//				}
//				else
//				{
//					HitMap.Emplace(BlasterCharacter, 1);
//				}
//			}
//
//			if (ImpactParticle)
//			{
//				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
//			}
//
//			if (HitSound)
//			{
//				UGameplayStatics::PlaySoundAtLocation(this, HitSound, FireHit.ImpactPoint, .5f, FMath::RandRange(-.5f, .5f));
//			}
//		}
//
//
//		for (const auto& HitPair : HitMap)
//		{
//			if (HitPair.Key && HasAuthority() && InstigatorController) // 오로지 데미지를 처리하는 부분만 서버가 처리.
//			{
//
//					UGameplayStatics::ApplyDamage(HitPair.Key , Damage * HitPair.Value, InstigatorController, this, UDamageType::StaticClass());
//
//			}
//		}
//	}
//}

void AShotgun::FireShotgun(const TArray<FVector_NetQuantize>& HitTargets)
{
	AWeapon_Gun::Fire(FVector());

	//UE_LOG(LogTemp, Error, TEXT("FireShotgun"));

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(SOCKET_MUZZLE_FLASH);
	if (MuzzleFlashSocket)
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		const FVector Start = SocketTransform.GetLocation();

		// Maps hit character to number of times hit
		TMap<ACharacterBase*, uint32> HitMap;
		TMap<ACharacterBase*, uint32> HeadShotHitMap;

		for (const FVector_NetQuantize& HitTarget : HitTargets)
		{
			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);


			//UE_LOG(LogTemp, Display, TEXT("ShotGun Hit"));
			IHitInterface* HitActor = Cast<IHitInterface>(FireHit.GetActor());
			if (HitActor)
			{
				HitActor->IGetHit(-HitTarget, FireHit);
				ApplyForce(FieldSystemComponent, FireHit);
			}



			ACharacterBase* BlasterCharacter = Cast<ACharacterBase>(FireHit.GetActor());
			if (BlasterCharacter) // 오로지 데미지를 처리하는 부분만 서버가 처리.
			{
				const bool bHeadShot = FireHit.BoneName.ToString() == TEXT("head");

				if (bHeadShot)
				{
					if (HeadShotHitMap.Contains(BlasterCharacter)) HeadShotHitMap[BlasterCharacter]++;
					else HeadShotHitMap.Emplace(BlasterCharacter, 1);
				}
				else
				{
					//UE_LOG(LogTemp, Display, TEXT("Shot"));
					if (HitMap.Contains(BlasterCharacter)) HitMap[BlasterCharacter]++;
					else HitMap.Emplace(BlasterCharacter, 1);
				}



				if (ImpactParticle)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
				}

				if (HitSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, HitSound, FireHit.ImpactPoint, .5f, FMath::RandRange(-.5f, .5f));
				}
			}
		}

		TArray<ACharacterBase*> HitCharacters;
		TMap<ACharacterBase*, float> DamageMap;

		for (const auto& HitPair : HitMap)
		{
			if (HitPair.Key)
			{
				//UE_LOG(LogTemp, Display, TEXT("HitPair"));
				//DamageMap.Emplace(HitPair.Key, HitPair.Value * Damage);

				DamageMap.Emplace(HitPair.Key, HitPair.Value * Damage);
				HitCharacters.AddUnique(HitPair.Key);

			}
		}


		for (const auto& HeadShotHitPair : HeadShotHitMap)
		{
			if (HeadShotHitPair.Key)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Headshot"));
				//DamageMap.Emplace(HeadShotHitPair.Key, HeadShotHitPair.Value * HeadShotDamage);

				DamageMap.Emplace(HeadShotHitPair.Key, HeadShotHitPair.Value * HeadShotDamage);
				HitCharacters.AddUnique(HeadShotHitPair.Key);

			}
		}

		for (const auto& HitCharacter : HitCharacters)
		{
			bool bCauseAuthDamage = !bUseServerSideRewind || OwnerPawn->IsLocallyControlled();
			if (HasAuthority() && bCauseAuthDamage && DamageMap.Contains(HitCharacter))
			{
				//UE_LOG(LogTemp, Display, TEXT("TotalDamage : %f"), TotalDamage);
				UGameplayStatics::ApplyDamage(HitCharacter, DamageMap[HitCharacter], InstigatorController, this, UDamageType::StaticClass());
			}
		}





		if (!HasAuthority() && bUseServerSideRewind)
		{
			//UE_LOG(LogTemp, Display, TEXT("!HasAuthority"));
			BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ACharacterBase>(OwnerPawn) : BlasterOwnerCharacter;
			BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(InstigatorController) : BlasterOwnerController;
			if (BlasterOwnerController && BlasterOwnerCharacter && BlasterOwnerCharacter->GetLagCompensationComponent() && BlasterOwnerCharacter->IsLocallyControlled()) // 오직 로컬플레이만 RPC 를 호출할 수 있도록 설정 그렇지 않으면 반복된 for 문으로 인해 네트워크 성능저하가 너무 심함
			{
				BlasterOwnerCharacter->GetLagCompensationComponent()->ShotgunServerScoreRequest(HitCharacters, Start, HitTargets, BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime);
			}
		}




	}
}

void AShotgun::ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets)
{
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(SOCKET_MUZZLE_FLASH);
	if (MuzzleFlashSocket == nullptr) return;

	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	const FVector TraceStart = SocketTransform.GetLocation();


	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	const FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;


	//DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, FColor::Red, false, 5.f);
	//DrawDebugSphere(GetWorld(), EndLoc, 4.f, 12, FColor::Orange, false, 5.f);
	//DrawDebugLine(GetWorld(), TraceStart, FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size()), FColor::Cyan, false, 5.f);

	for (uint32 i = 0; i < NumberOfPellets; i++)
	{
		const FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
		const FVector EndLoc = SphereCenter + RandVec;
		FVector ToEndLoc = EndLoc - TraceStart;
		ToEndLoc = TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size();

		HitTargets.Add(ToEndLoc);
	}
}
