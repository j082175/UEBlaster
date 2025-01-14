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
#include "Interfaces/TeamInterface.h"
#include "Components/ReceiveDamageHUDComponent.h"

#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Blaster/Blaster.h"

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	//UE_LOG(LogTemp, Display, TEXT("InstigatorController : %x"), InstigatorController);

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(SOCKET_MUZZLE_FLASH);
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();

		FHitResult FireHit;
		WeaponTraceHit(Start, HitTarget, FireHit);

		if (FireHit.bBlockingHit)
		{
			//UE_LOG(LogTemp, Display, TEXT("bBlockingHit"));

			float DamageToCause = FireHit.BoneName.ToString() == TEXT("head") ? HeadShotDamage : Damage;

			DamageToCause = FMath::RandRange(DamageToCause - DamageDeviation, DamageToCause + DamageDeviation);

			IHitInterface* HitActor = Cast<IHitInterface>(FireHit.GetActor());
			if (HitActor)
			{

				ITeamInterface* TI = Cast<ITeamInterface>(FireHit.GetActor());
				ITeamInterface* T2 = Cast<ITeamInterface>(GetOwner());
				if (TI && T2 && TI->IGetTeam() == T2->IGetTeam())
				{
					return;
				}
				


				//if (HasAuthority()) UE_LOG(LogTemp, Error, TEXT("HitActor : %s"), *FireHit.GetActor()->GetName());
				HitActor->IGetHit(HitTarget, FireHit);
				//ApplyForce(FieldSystemComponent, FireHit);
				//UAISense_Damage::ReportDamageEvent(this, FireHit.GetActor(), GetInstigator(), Damage, GetActorLocation(), HitTarget);
				
				ABlasterPlayerController* BPC = Cast<ABlasterPlayerController>(GetInstigatorController());
				UReceiveDamageHUDComponent* DHUD = FireHit.GetActor()->GetComponentByClass<UReceiveDamageHUDComponent>();

				if (BPC && BPC->IsLocalPlayerController())
				{
					//UE_LOG(LogTemp, Error, TEXT("OnHit : %s"), *FireHit.BoneName.ToString());

					if (FireHit.BoneName.ToString() == TEXT("head"))
					{
						BPC->PlayHitNoticeAnim(TEXT("Head"));
						if (DHUD) DHUD->SetDamageInfo(DamageToCause, GetInstigatorController(), FLinearColor::Red);
					}
					else
					{
						BPC->PlayHitNoticeAnim(TEXT("Body"));
						if (DHUD) DHUD->SetDamageInfo(DamageToCause, GetInstigatorController(), FLinearColor(0.f, 1.f, 1.f));
					}
				}
			}


			if (HitActor && InstigatorController) // 오로지 데미지를 처리하는 부분만 서버가 처리.
			{
				//UE_LOG(LogTemp, Display, TEXT("Ahthority : %d, ServerSideRewind : %d"), HasAuthority(), bUseServerSideRewind);
				bool bCauseAuthDamage = !bUseServerSideRewind || OwnerPawn->IsLocallyControlled();

				if (!FireHit.GetActor()->GetComponentByClass<ULagCompensationComponent>())
				{
					UGameplayStatics::ApplyDamage(FireHit.GetActor(), DamageToCause, InstigatorController, this, UDamageType::StaticClass());
				}

				else if (HasAuthority() && bCauseAuthDamage)
				{
					//UE_LOG(LogTemp, Display, TEXT("Not Use ServerSideRewind"));

					//UE_LOG(LogTemp, Display, TEXT("HitScanWeapon : HasAuthority, %s, Damage : %f"), *FireHit.GetActor()->GetName(), DamageToCause);

					UGameplayStatics::ApplyDamage(FireHit.GetActor(), DamageToCause, InstigatorController, this, UDamageType::StaticClass());


				}
				else if (!HasAuthority() && bUseServerSideRewind)
				{
					//UE_LOG(LogTemp, Display, TEXT("Use ServerSideRewind"));

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

		//if (MuzzleFlash)
		//{
		//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		//}

		//if (FireSound)
		//{
		//	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		//}
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
			
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World, BeamParticles, TraceStart, (End - TraceStart).GetSafeNormal().Rotation(), FVector(1.f, 0.2f, 0.2f), true);

			if (Beam)
			{
				Beam->SetVectorParameter(TEXT("Target"), BeamEnd);
			}

			//APooledObject* PO = Cast<UObjectPoolSubsystem>(GetGameInstance())->GetSpawnedObject(FTransform(GetActorRotation(), OutHit.ImpactPoint), PoolTest);


		}
	}
}
