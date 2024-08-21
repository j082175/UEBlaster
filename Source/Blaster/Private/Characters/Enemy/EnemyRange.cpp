// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyRange.h"
#include "Kismet/GameplayStatics.h"
#include "Item/Pickable/Weapon/Weapon_Gun.h"
#include "Components/CombatComponent.h"
#include "AIController/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlasterTypes/BlackboardKeys.h"
#include "Item/Pickable/Weapon/Shotgun.h"
#include "Components/MyCharacterMovementComponent.h"

AEnemyRange::AEnemyRange()
{
	InitializeDefaults();

	StartingARAmmo = 10000;
	StartingRocketAmmo = 10000;
	StartingPistolAmmo = 10000;
	StartingSMGAmmo = 10000;
	StartingShotgunAmmo = 10000;
	StartingSniperAmmo = 10000;
	StartingGrenadeLauncherAmmo = 10000;

	BaseWalkSpeed = 300.f;
	AimWalkSpeed = 200.f;

}

void AEnemyRange::OnMontageEndedFunc(UAnimMontage* Montage, bool bInterrupted)
{
	ACharacterBase::OnMontageEndedFunc(Montage, bInterrupted);
}

void AEnemyRange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EquippedGun = EquippedGun == nullptr ? Cast<AWeapon_Gun>(EquippedWeapon) : EquippedGun;

	AIController = AIController == nullptr ? Cast<AEnemyAIController>(GetController()) : AIController;

	if (AIController && AIController->GetBlackboardComponent())
	{
		AActor* TargetActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TARGET_ACTOR));
		if (TargetActor)
		{
			AIController->GetBlackboardComponent()->ClearValue(TARGET_ACTOR_LAST_POSITION);

			TargetPoint = TargetActor->GetActorLocation();
		}
		else
		{
			if (TargetPoint != FVector(0.f))
			{
				if (TargetPointCheckCount > TargetPointCheckThreshold)
				{
					AIController->GetBlackboardComponent()->ClearValue(TARGET_ACTOR_LAST_POSITION);
					TargetPointCheckCount = 0.f;
					TargetPoint = FVector(0.f);
				}
				else
				{
					//UE_LOG(LogTemp, Display, TEXT("TargetActor Set"));
					AIController->GetBlackboardComponent()->SetValueAsVector(TARGET_ACTOR_LAST_POSITION, TargetPoint);
					SetAiming(false);
					FireButtonPressed(false);
					OnAttackEnded.ExecuteIfBound();

					TargetPointCheckCount += DeltaTime;
				}
			}
		}


	}

	if (EquippedGun)
	{
		FTransform MuzzleTipTransform = EquippedGun->GetWeaponMesh()->GetSocketTransform(TEXT("MuzzleFlash"));
		DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), TargetPoint, FColor::Red, false);
	}


	//UE_LOG(LogTemp, Display, TEXT("CombatState : %s"), *UEnum::GetDisplayValueAsText(CombatState).ToString());
	//UE_LOG(LogTemp, Display, TEXT("AimWalkSpeed : %f"), AimWalkSpeed);

}

void AEnemyRange::InitializeDefaults()
{
	Super::InitializeDefaults();

	CharacterState = ECharacterState::ECS_EquippedGun;

	bIsAiming = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	UMyCharacterMovementComponent* MC = Cast<UMyCharacterMovementComponent>(GetCharacterMovement());
	if (MC)
	{
		MC->SetUseAccelerationForPaths(true);
	}
}

void AEnemyRange::FireButtonPressed(bool bPressed)
{
	//UE_LOG(LogTemp, Display, TEXT("AI Firing"));
	//UE_LOG(LogTemp, Display, TEXT("CombatState : %s"), *UEnum::GetDisplayValueAsText(CombatState).ToString());

	if (CombatState != ECombatState::ECS_Unoccupied) return;

	if (EquippedGun == nullptr) return;
	bFireButtonPressed = bPressed;
	if (!EquippedGun->IsAutomatic() && bIsFiring) return;

	if (bFireButtonPressed)
	{
		//UE_LOG(LogTemp, Display, TEXT("EE"));
		bIsFiring = true;

		if (EquippedGun && bPressed)
		{
			CrosshairShootingFactor = 0.75f;

			switch (EquippedGun->GetFireType())
			{
			case EFireType::EFT_HitScan:
				FireHitScanWeapon(bPressed);
				break;
			case EFireType::EFT_Projectile:
				FireProjectileWeapon(bPressed);
				break;
			case EFireType::EFT_Shotgun:
				FireShotgun(bPressed);
				break;
			default:
				break;
			}
		}

		//UE_LOG(LogTemp, Display, TEXT("FireTimer : %d"), FireTimer.IsValid());

		//FireTimer.Invalidate();
		StartFireTimer();
	}
}

void AEnemyRange::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultWeapon();
	
	if (AIController && AIController->GetBlackboardComponent())
	AIController->GetBlackboardComponent()->ClearValue(TARGET_ACTOR_LAST_POSITION);


}

void AEnemyRange::IAttack(FAttackEndedDelegate Delegate, const FString& AttackType)
{
	OnAttackEnded = Delegate;

	AttackFunc();
}

void AEnemyRange::ISetAIState(EAIState InAIState)
{
	Super::ISetAIState(InAIState);

	AIController->GetBlackboardComponent()->SetValueAsBool(CAN_SEE, false);
}

void AEnemyRange::FinishReloading()
{
	Super::FinishReloading();

	AttackFunc();
}

void AEnemyRange::FireProjectileWeapon(bool bPressed)
{
	if (EquippedGun)
	{
		TargetPoint = EquippedGun->bUseScatter ? EquippedGun->TraceEndWithScatter(TargetPoint) : TargetPoint;

		MulticastFire(bPressed, TargetPoint);
	}
}

void AEnemyRange::FireHitScanWeapon(bool bPressed)
{
	if (EquippedGun)
	{
		TargetPoint = EquippedGun->bUseScatter ? EquippedGun->TraceEndWithScatter(TargetPoint) : TargetPoint;
		MulticastFire(bPressed, TargetPoint);
	}
}

void AEnemyRange::FireShotgun(bool bPressed)
{
	AShotgun* Shotgun = Cast<AShotgun>(EquippedGun);
	if (Shotgun)
	{
		TArray<FVector_NetQuantize> HitTargets;
		Shotgun->ShotgunTraceEndWithScatter(TargetPoint, HitTargets);
		MulticastShotgunFire(HitTargets);
	}
}

void AEnemyRange::AttackFunc()
{
	if (!IsValid(AttackMontage)) return;
	if (!EquippedGun) return;

	//UE_LOG(LogTemp, Display, TEXT("IAttack"));

	TargetPoint = EquippedGun->bUseScatter ? EquippedGun->TraceEndWithScatter(TargetPoint) : TargetPoint;
	//MulticastFire(true, TargetPoint);
	FireButtonPressed(true);
	SetAiming(true);
	EquippedGun->SetIsAutomatic(true);



	//FTimerHandle FireTimer1;
	//GetWorldTimerManager().SetTimer(FireTimer1, FTimerDelegate::CreateLambda([&]()
	//	{
	//		FireButtonPressed(false);
	//		SetAiming(false);
	//		OnAttackEnded.ExecuteIfBound();
	//	}), 3.f, false);
}