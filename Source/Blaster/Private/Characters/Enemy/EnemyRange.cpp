// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyRange.h"
#include "Item/Pickable/Weapon/Weapon_Gun.h"
#include "Components/CombatComponent.h"
#include "AIController/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlasterTypes/BlackboardKeys.h"
#include "Item/Pickable/Weapon/Shotgun.h"
#include "Components/MyCharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "HUD/OverheadWidget.h"
#include "PlayerState/BlasterPlayerState.h"
#include "Components/InventoryComponent.h"
#include "Blaster/Blaster.h"

AEnemyRange::AEnemyRange()
{
	InitializeDefaults();

	//BaseWalkSpeed = 300.f;
	//AimWalkSpeed = 200.f;

}

void AEnemyRange::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void AEnemyRange::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultWeapon();

	if (BaseAIController.IsValid() && BaseAIController->GetBlackboardComponent())
		BaseAIController->GetBlackboardComponent()->ClearValue(TARGET_ACTOR_LAST_POSITION);


	FTimerHandle H;
	GetWorldTimerManager().SetTimer(H, this, &ThisClass::InitEnemyOverhead, 0.1f, false);


	ACharacterBase* CharacterOwner = Cast<ACharacterBase>(GetOwner());
	if (CharacterOwner)
	{
		SpawnDefaultController();
		ISetTeam(CharacterOwner->IGetTeam());
		SetTeamColor(IGetTeam());

		if (AAIController* AIController = Cast<AAIController>(GetController()))
		{
			AIController->GetBlackboardComponent()->SetValueAsObject(OWING_ACTOR, GetInstigator());
		}
	}

	//SetActorTickInterval(1.f);
}

void AEnemyRange::OnMontageEndedFunc(UAnimMontage* Montage, bool bInterrupted)
{
	ACharacterBase::OnMontageEndedFunc(Montage, bInterrupted);

	if (Montage == MantleMontage || Montage == VaultMontage)
	{
		AttackFunc();
	}
}

void AEnemyRange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EquippedGun = EquippedGun == nullptr ? Cast<AWeapon_Gun>(InventoryComponent->EquippedWeapon) : EquippedGun;

	BaseAIController = BaseAIController == nullptr ? Cast<ABaseAIController>(GetController()) : BaseAIController.Get();

	if (BaseAIController.IsValid() && BaseAIController->GetBlackboardComponent())
	{
		AActor* TargetActor = Cast<AActor>(BaseAIController->GetBlackboardComponent()->GetValueAsObject(TARGET_ACTOR));
		if (TargetActor)
		{
			BaseAIController->GetBlackboardComponent()->ClearValue(TARGET_ACTOR_LAST_POSITION);

			TargetPoint = TargetActor->GetActorLocation();
		}
		else
		{
			if (TargetPoint != FVector(0.f))
			{
				if (TargetPointCheckCount > TargetPointCheckThreshold)
				{
					BaseAIController->GetBlackboardComponent()->ClearValue(TARGET_ACTOR_LAST_POSITION);
					TargetPointCheckCount = 0.f;
					TargetPoint = FVector(0.f);
				}
				else
				{
					//UE_LOG(LogTemp, Display, TEXT("TargetActor Set"));
					BaseAIController->GetBlackboardComponent()->SetValueAsVector(TARGET_ACTOR_LAST_POSITION, TargetPoint);
					SetAiming(false);
					Fire(false);
					OnAttackEnded.ExecuteIfBound();

					TargetPointCheckCount += DeltaTime;
				}
			}
		}


	}

#if WITH_EDITOR
	if (EquippedGun)
	{
		FTransform MuzzleTipTransform = EquippedGun->GetWeaponMesh()->GetSocketTransform(SOCKET_MUZZLE_FLASH);
		DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), TargetPoint, FColor::Red, false);
	}
#endif

	//UE_LOG(LogTemp, Display, TEXT("EnemyRange Tick"));

	//UE_LOG(LogTemp, Display, TEXT("CombatState : %s"), *UEnum::GetDisplayValueAsText(CombatState).ToString());
	//UE_LOG(LogTemp, Display, TEXT("AimWalkSpeed : %f"), AimWalkSpeed);

}

void AEnemyRange::InitializeDefaults()
{
	Super::InitializeDefaults();

	CharacterState = ECharacterState::EquippedGun;

	bIsAiming = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	UMyCharacterMovementComponent* MC = Cast<UMyCharacterMovementComponent>(GetCharacterMovement());
	if (MC)
	{
		MC->SetUseAccelerationForPaths(true);
	}
}

void AEnemyRange::Fire(bool bPressed)
{
	if (!HasAuthority()) return;
	//UE_LOG(LogTemp, Display, TEXT("bPressed : %d"), bPressed);
	//UE_LOG(LogTemp, Display, TEXT("AI Firing"));
	//UE_LOG(LogTemp, Display, TEXT("CombatState : %s"), *UEnum::GetDisplayValueAsText(CombatState).ToString());

	if (CombatState != ECombatState::Unoccupied && CombatState != ECombatState::Attacking)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AEnemyRange::FireButtonPressed : CombatState is not unoccupied : %s"), *UEnum::GetDisplayValueAsText(CombatState).ToString());
		return;
	}

	if (EquippedGun == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AEnemyRange::FireButtonPressed : EquippedGun is null"));
		return;
	}

	bFireButtonPressed = bPressed;
	if (!EquippedGun->IsAutomatic() && bIsFiring)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AEnemyRange::FireButtonPressed : !EquippedGun->IsAutomatic() && bIsFiring"));
		return;
	}

	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BaseAIController->GetBrainComponent());
	if (BehaviorTreeComponent && !BehaviorTreeComponent->IsRunning())
	{
		//UE_LOG(LogTemp, Warning, TEXT("AEnemyRange::FireButtonPressed : Shutdown"));
		return;
	}

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
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("AEnemyRange::FireButtonPressed : bFireButtonPressed is zero"));

	}
}

void AEnemyRange::IAttack(FAttackEndedDelegate Delegate, const FString& AttackType)
{
	OnAttackEnded = Delegate;

	AttackFunc();
}

void AEnemyRange::ISetAIState(EAIState InAIState)
{
	Super::ISetAIState(InAIState);

	if (BaseAIController.IsValid()) BaseAIController->GetBlackboardComponent()->SetValueAsBool(CAN_SEE, false);
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
	


	UE_LOG(LogTemp, Display, TEXT("IAttack"));

	TargetPoint = EquippedGun->bUseScatter ? EquippedGun->TraceEndWithScatter(TargetPoint) : TargetPoint;
	//MulticastFire(true, TargetPoint);
	Fire(true);
	SetAimingFunc(true);
	EquippedGun->SetIsAutomatic(true);



	//FTimerHandle FireTimer1;
	//GetWorldTimerManager().SetTimer(FireTimer1, FTimerDelegate::CreateLambda([&]()
	//	{
	//		FireButtonPressed(false);
	//		SetAiming(false);
	//		OnAttackEnded.ExecuteIfBound();
	//	}), 3.f, false);
}

void AEnemyRange::SetIsActive(bool InIsActive)
{
	Super::SetIsActive(InIsActive);

	if (!InIsActive)
	{
		SetAiming(false);
		Fire(false);
		OnAttackEnded.ExecuteIfBound();
	}
}

void AEnemyRange::StartFireTimer()
{
	if (InventoryComponent->EquippedWeapon == nullptr) return;
	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(InventoryComponent->EquippedWeapon);
	if (!Gun) return;

	GetWorldTimerManager().SetTimer(FireTimer, this, &ThisClass::FireTimerFinished, Gun->GetFireDelay());
}

void AEnemyRange::FireTimerFinished()
{

	//if (InventoryComponent->EquippedWeapon == nullptr) return;
	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(InventoryComponent->EquippedWeapon);
	if (!Gun) return;
	//UE_LOG(LogTemp, Display, TEXT("FireTimerFinished"));

	////UE_LOG(LogTemp, Display, TEXT("FireTimer : %d"), FireTimer.IsValid());

	//ReloadEmptyWeapon();

	if (InventoryComponent->EquippedWeapon && Gun->IsEmpty())
	{
		ServerReload();
		return;
	}

	if (bFireButtonPressed && Gun->IsAutomatic())
	{
		Fire(true);
	}
	bIsFiring = false;
	//GetWorldTimerManager().ClearTimer(FireTimer);
	FireTimer.Invalidate();
}

void AEnemyRange::InitEnemyOverhead()
{
	if (Tags.Num() != 0 && GetInstigatorController())
	{
		ABlasterPlayerState* BPS = GetInstigatorController()->GetPlayerState<ABlasterPlayerState>();
		if (BPS)
		{
			FString PlayerName = BPS->GetPlayerName();
			FString Str = FString::Printf(TEXT("%s's %s"), *PlayerName, *Tags[0].ToString());
			OverheadWidget->ShowPlayerName(Str);
		}
	}
}
