// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"
#include "Blaster/Blaster.h"

// Components
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BuffComponent.h"
#include "Components/BoxComponent.h"
#include "Components/LagCompensationComponent.h"
#include "Components/MantleVaultComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/MyCharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MotionWarpingComponent.h"
//#include "Components/PawnNoiseEmitterComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/MyAIPerceptionStimuliSource.h"

// Interfaces
#include "Interfaces/InteractWithCrosshairsInterface.h"

// EnhancedInput
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// HUDs
#include "HUD/OverheadWidget.h"
#include "HUD/HpBarWidgetComponent.h"
#include "HUD/HpBarWidget.h"

// Network
#include "Net/UnrealNetwork.h"

// Weapon
#include "Item/Pickable/Weapon/Weapon.h"
#include "Item/Pickable/Weapon/Weapon_Gun.h"
#include "Item/Pickable/Weapon/Weapon_Melee.h"
#include "Item/Pickable/Weapon/Shotgun.h"
#include "Actor/Projectile/ProjectileGrenade.h"

// Math
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Animations
#include "Animations/BlasterAnimInstance.h"

// PlayerController
#include "PlayerController/BlasterPlayerController.h"

// GameMode & GameState & PlayerState
#include "GameMode/BlasterGameMode.h"
#include "GameState/BlasterGameState.h"
#include "PlayerState/BlasterPlayerState.h"

// PlayerStart
#include "PlayerStart/TeamPlayerStart.h"

// Sound
#include "Sound/SoundCue.h"

// Niagara
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Physics
#include "PhysicsEngine/PhysicsAsset.h"

// AI
#include "AIController.h"
#include "AIController/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlasterTypes/BlackboardKeys.h"

// Items
#include "Item/Pickups/SpeedPickup.h"

// DamageType
#include "DamageType/DamageType_Rumbling.h"

//#include "TimerManager.h"
#include "Item/Pickable/Weapon/WeaponTypes.h"

#include "Characters/Enemy/Enemy.h"

#include "BlasterTypes/KeyType.h"

// Set default values
ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{

	UE_LOG(LogTemp, Display, TEXT("Super getname : %s"), *GetName());

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HpBarWidgetComponent = CreateDefaultSubobject<UHpBarWidgetComponent>(TEXT("HpBarWidgetComponent"));
	ensure(HpBarWidgetComponent);
	HpBarWidgetComponent->SetupAttachment(RootComponent);

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	ensure(AttributeComponent);

	LagCompensation = CreateDefaultSubobject<ULagCompensationComponent>(TEXT("LagCompensation"));

	MantleVaultComponent = CreateDefaultSubobject<UMantleVaultComponent>(TEXT("MantleVaultComponent"));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	//PawnNoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitter"));

	AIPerceptionStimuliSource = CreateDefaultSubobject<UMyAIPerceptionStimuliSource>(TEXT("AIPerceptionStimuliSource"));

	//UE_LOG(LogTemp, Display, TEXT("Base Constructor"));

	InitializeCollisionStates();
	InitializeDefaults();


}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//UE_LOG(LogTemp, Warning, TEXT("Base Initialize"));


	if (BuffComponent)
	{
		BuffComponent->Character = this;
		if (GetCharacterMovement())
		{
			BuffComponent->SetInitialSpeeds(GetCharacterMovement()->MaxWalkSpeed, GetCharacterMovement()->MaxWalkSpeedCrouched);
			BuffComponent->SetInitialJumpVelocity(GetCharacterMovement()->JumpZVelocity);
		}

	}
	
	if (LagCompensation)
	{
		LagCompensation->Character = this;
		//if (Controller)
		//{
		//	LagCompensation->Controller = Cast<ABlasterPlayerController>(Controller);
		//}
	}

	if (HasAuthority())
	{
		InitializeCarriedAmmo();
	}

	//CheckUObjectIsValid(GetMesh()->GetAnimInstance());
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Base Beginplay"));

	//InitializeCollisionStates();
	InitializeDelegates();
	InitializeWidgets();




	Grenades = MaxGrenades;


	//if (GetCameraComponent())
	//{
	//	DefaultFOV = GetCameraComponent()->FieldOfView;
	//	CurrentFOV = DefaultFOV;
	//}


}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CombatState);

	DOREPLIFETIME(ThisClass, EquippedWeapon);
	DOREPLIFETIME(ThisClass, SecondaryWeapon);
	DOREPLIFETIME(ThisClass, bIsAiming);
	DOREPLIFETIME(ThisClass, CharacterState);
	DOREPLIFETIME_CONDITION(ThisClass, CarriedAmmo, COND_OwnerOnly);
	DOREPLIFETIME(ThisClass, Grenades);
	DOREPLIFETIME(ThisClass, bHoldingTheFlag);
	DOREPLIFETIME(ThisClass, Flag);
	DOREPLIFETIME(ThisClass, bIsSliding);
	DOREPLIFETIME_CONDITION(ThisClass, bIsDodge, COND_SimulatedOnly);
	DOREPLIFETIME(ThisClass, bIsMorV);
	DOREPLIFETIME(ThisClass, AnimState);
	DOREPLIFETIME(ThisClass, bParryResultCheck);
	DOREPLIFETIME_CONDITION(ThisClass, bDisableGameplay, COND_OwnerOnly);
	DOREPLIFETIME(ThisClass, KeySectionName);
	//DOREPLIFETIME(ThisClass, CharacterHitPoint);


	DOREPLIFETIME_CONDITION(ThisClass, OverlappingWeapon, COND_OwnerOnly);
	//DOREPLIFETIME(ThisClass, MeshLocation);


}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("%s"), *GetFName().ToString());

	PollInit();



	RotateInPlace(DeltaTime);
	UpdateMotionWarpingTransform();

	DashTimeline.TickTimeline(DeltaTime);


	//UE_LOG(LogTemp, Display, TEXT("bRagdollInProgress : %d"), bRagdollInProgress);

	// Set Ragdoll Settings
	if (bRagdollInProgress || bIsElimmed)
	{
		CalculateMeshLocation();
		GetCapsuleComponent()->SetWorldLocation(MeshLocation);
	}
	else
	{
		//MeshLocation = GetMesh()->GetSocketLocation(TEXT("pelvis")) - GetMesh()->GetRelativeLocation();
		MeshLocation = GetCapsuleComponent()->GetComponentLocation();
	}

	// Ragdoll Recover
	if (bRagdollInProgress && IsLocallyControlled())
	{
		CheckCapsuleAndMeshEquals(DeltaTime, 0.1f);
		if (bIsRagdollComplete)
		{
			if (!bIsElimmed) ExecutePhysicsRecover();
		}
	}

	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	if (BlasterGameMode)
	{
		FName MatchState = BlasterGameMode->GetMatchState();
		if (MatchState == MatchState::Cooldown)
		{
			bDisableGameplay = bDisableGameplay == false ? true : bDisableGameplay;
		}
	}
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::IGetHit(const FVector& InHitPoint)
{
	//DrawDebugSphere(GetWorld(), CharacterHitPoint, 10.f, 12, FColor::Cyan, false, 5.f);
	//EnemyAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsHit"), true);

	CharacterHitPoint = InHitPoint;

	CalculateHitDirection(InHitPoint);

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}

	if (BloodEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect, CharacterHitPoint);
	}


	if (bIsElimmed)
		return SetDead();


	if (CombatState == ECombatState::ECS_Dodging || CombatState == ECombatState::ECS_VaultOrMantle) return;

	if (CombatState != ECombatState::ECS_Unoccupied) return;
	PlayHitReactMontage(InHitPoint);



}

void ACharacterBase::IBindOverheadWidget(UUserWidget* InUserWidget)
{
	if (AttributeComponent == nullptr) return;

	//AttributeComponent->OnHpChanged.AddDynamic(InHpBarWidgetComponent, &UHpBarWidgetComponent::SetHpBar);
	//AttributeComponent->OnShieldChanged.AddDynamic(InHpBarWidgetComponent, &UHpBarWidgetComponent::SetShieldBar);


	if (UHpBarWidget* Hp = Cast<UHpBarWidget>(InUserWidget))
	{
		AttributeComponent->OnHpChanged.AddUObject(Hp, &UHpBarWidget::SetHpBar);
		AttributeComponent->OnShieldChanged.AddUObject(Hp, &UHpBarWidget::SetShieldBar);
		AttributeComponent->OnParryGaugeChanged.AddUObject(Hp, &UHpBarWidget::SetParryBar);
		AttributeComponent->OnParryGaugeAnim.AddUObject(Hp, &UHpBarWidget::ParryGaugeAnimStart);
	}






}

bool ACharacterBase::IIsParring()
{
	return bIsParring;
}

bool ACharacterBase::ICheckParry(AActor* OtherActor)
{
	//UE_LOG(LogTemp, Display, TEXT("CheckParry : %s"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString());

	ENetRole Role1 = GetLocalRole();

	AEnemy* AO = Cast<AEnemy>(OtherActor);
	if (AO == nullptr) return false;

	//if (!HasAuthority() && IsLocallyControlled())
	//{
	//	bParryResultCheck = AO->CheckParryFunc(this);
	//}

	if (IsLocallyControlled()) ServerCheckParry(OtherActor);

	return bParryResultCheck;
}

void ACharacterBase::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Display, TEXT("Damage : %f"), Damage);

	//UE_LOG(LogTemp, Display, TEXT("DamageType : bCausedByWorld :%d, bScaleMomentumByMass : %d, bRadialDamageVelChange : %d, DamageImpulse : %f, DestructibleImpulse : %f, DestructibleDamageSpreadScale : %f, DamageFalloff : %f"), DamageType->bCausedByWorld, DamageType->bScaleMomentumByMass, DamageType->bRadialDamageVelChange, DamageType->DamageImpulse, DamageType->DestructibleImpulse, DamageType->DestructibleDamageSpreadScale, DamageType->DamageFalloff);

	LastDamageCauser = DamageCauser;



	if (AttributeComponent == nullptr || BlasterGameMode == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("ReceiveDamage Error!"));
	}

	//UE_LOG(LogTemp, Display, TEXT("ReceiveDamage : %f"), Damage);

	if (bIsElimmed || BlasterGameMode == nullptr) return;
	Damage = BlasterGameMode->CalculateDamage(InstigatorController, Controller, Damage);
	//UE_LOG(LogTemp, Display, TEXT("ReceiveDamage Damage : %f"), Damage);
	float DamageToHealth = Damage;

	float CurrentShield = AttributeComponent->GetCurrentShield();
	float MaxShield = AttributeComponent->GetMaxShield();
	float CurrentHealth = AttributeComponent->GetCurrentHp();
	float MaxHealth = AttributeComponent->GetMaxHp();

	if (CurrentShield > 0.f)
	{
		if (CurrentShield >= Damage)
		{
			CurrentShield = FMath::Clamp(CurrentShield - Damage, 0.f, MaxShield);
			//UE_LOG(LogTemp, Display, TEXT("CurrentShield : %f"), CurrentShield);


			DamageToHealth = 0.f;
		}
		else
		{
			DamageToHealth = FMath::Clamp(Damage - CurrentShield, 0.f, Damage);
			CurrentShield = 0.f;
		}
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageToHealth, 0.f, MaxHealth);

	if (CombatState == ECombatState::ECS_Parried)
	{
		//UE_LOG(LogTemp, Display, TEXT("Not Process ParryGauge"));
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("Not Process ParryGauge : %s"), *UEnum::GetDisplayValueAsText(CombatState).ToString());

		AttributeComponent->SetCurrentParryGauge(FMath::Clamp(AttributeComponent->GetCurrentParryGauge() + AttributeComponent->GetParryGaugeIncrement(), 0.f, AttributeComponent->GetMaxParryGauge()));
	}

	//UE_LOG(LogTemp, Display, TEXT("CurrentHealth : %f, CurrentShield : %f"), CurrentHealth, CurrentShield);

	AttributeComponent->SetCurrentShield(CurrentShield);
	AttributeComponent->SetCurrentHp(CurrentHealth);

	//BlasterPlayerController->UpdateHUDHealth();
	//BlasterPlayerController->UpdateHUDShield();
	//PlayMontage(GetMesh()->GetAnimInstance(), HitReactMontage, CalculateHitDirection());

	//if (CurrentHealth <= 0.f)
	//{
	//	if (BlasterGameMode)
	//	{
	//		BlasterPlayerController = BlasterPlayerController.Get() == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController.Get();

	//		ABlasterPlayerController* AttackerController = Cast<ABlasterPlayerController>(InstigatorController);
	//		BlasterGameMode->PlayerEliminated(this, BlasterPlayerController, AttackerController);
	//	}
	//}

	if (CurrentHealth <= 0.f)
	{
		bIsElimmed = true;
		SetDead();
	}

	//HpBarWidgetComponent->SetHpBar(CurrentHealth / AttributeComponent->GetMaxHp());
	AttributeComponent->OnHpChanged.Broadcast(CurrentHealth / MaxHealth);
	AttributeComponent->OnShieldChanged.Broadcast(CurrentShield / MaxShield);
	AttributeComponent->OnParryGaugeChanged.Broadcast(AttributeComponent->GetCurrentParryGauge(), AttributeComponent->GetMaxParryGauge());


	if (Cast<UDamageType_Rumbling>(DamageType))
	{
		//UE_LOG(LogTemp, Display, TEXT("Rumbling!"));
		ExecuteRagdoll();
	}
}

//float ACharacterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	float ResultDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
//
//	UE_LOG(LogTemp, Display, TEXT("TakeDamage!"));
//
//	float CurrentHp = FMath::Clamp(AttributeComponent->GetCurrentHp() - Damage, 0.f, AttributeComponent->GetMaxHp());
//	AttributeComponent->SetCurrentHp(CurrentHp);
//
//	if (CurrentHp <= 0.f)
//	{
//		bIsElimmed = true;
//	}
//
//	HpBarWidgetComponent->SetHpBar(CurrentHp / AttributeComponent->GetMaxHp());
//
//	return ResultDamage;
//}

void ACharacterBase::SetDead()
{
	//UE_LOG(LogTemp, Display, TEXT("Dead"));
	CombatState = ECombatState::ECS_Unoccupied;

	//HpBarWidgetComponent->SetVisibility(false);
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//DisableInput(Cast<APlayerController>(GetController()));

	if (Tags.Num() > 0 && Tags[0] == TEXT("Hovering"))
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetSimulatePhysics(true);
	}

	//if (DeadMontage)
	//{
	//	PlayRandomMontage(GetMesh()->GetAnimInstance(), DeadMontage, TEXT("Dead"));
	//}

	Elim(false);
}

void ACharacterBase::OnPlayMontageNotifyBeginFunc(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	//UE_LOG(LogTemp, Display, TEXT("CharacterBase : OnPlayMontageNotifyBeginFunc"));
	//UE_LOG(LogTemp, Display, TEXT("NotifyName : %s"), *NotifyName.ToString());

#define ENABLE_BOXCOLLISION TEXT("EnableBoxCollision")
#define DISABLE_BOXCOLLISION TEXT("DisableBoxCollision")
#define DEADMONTAGE_END_CHECK TEXT("End")
#define ENABLE_COLLISION TEXT("EnableCollision")
#define DISABLE_COLLISION TEXT("DisableCollision")
#define AURA_RIGHT TEXT("AuraRight")
#define AURA_LEFT TEXT("AuraLeft")
#define SPAWN_ENEMY_RIGHT TEXT("SpawnEnemyRight")
#define SPAWN_ENEMY_LEFT TEXT("SpawnEnemyLeft")
#define PARRY_START TEXT("ParryStart")
#define PARRY_END TEXT("ParryEnd")
#define RELOAD_FINISHED TEXT("ReloadFinished")
#define SHELL TEXT("Shell")
#define THROW_GRENADE_FINISHED TEXT("ThrowGrenadeFinished")
#define GRENADE_LAUNCH TEXT("GrenadeLaunch")
#define SWAP_ATTACHED_WEAPON TEXT("SwapAttachWeapon")
#define SLIDE_END TEXT("SlideEnd")
#define DODGE_END TEXT("DodgeEnd")
#define VAULT_OR_MANTLE_END TEXT("VaultOrMantleEnd")
#define BOLT TEXT("Bolt")
#define GROUND_ATTACK TEXT("GroundAttack")
#define SLOMO_START TEXT("SlomoStart")
#define SLOMO_END TEXT("SlomoEnd")
#define STANDING TEXT("Standing")


	if (ENABLE_BOXCOLLISION == NotifyName)
	{
		if (Weapon)
		{
			//UE_LOG(LogTemp, Display, TEXT("ENABLE_BOXCOLLISION"));
			Weapon->GetBoxComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			//UE_LOG(LogTemp, Display, TEXT("ENABLE_BOXCOLLISION"));
			EnableHitCapsulesCollision();


			//FBodyInstance* BodyInstance = GetMesh()->GetBodyInstance(TEXT("weapon_r"));
			//if (BodyInstance)
			//{
			//	UE_LOG(LogTemp, Display, TEXT("%s : Enable Physics Collision"), *BodyInstance->GetBodyDebugName());




			//	BodyInstance->SetShapeCollisionEnabled(0, ECollisionEnabled::QueryAndPhysics);
			//	FCollisionResponseContainer FRC;
			//	FRC.SetResponse(ECC_SkeletalMesh, ECollisionResponse::ECR_Ignore);

			//	BodyInstance->SetShapeResponseToChannels(0, FRC);
			//	BodyInstance->SetResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Ignore);


			//	ECollisionEnabled::Type T1 = BodyInstance->GetCollisionEnabled();
			//	ECollisionEnabled::Type T2 = BodyInstance->GetShapeCollisionEnabled(0);
			//	FCollisionResponseContainer FRC2 = BodyInstance->GetShapeResponseToChannels(0);
			//	FCollisionResponse R = BodyInstance->GetCollisionResponse();

			//	int a = 1;
			//}
			//else
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("No BodyInstance"));
			//}

		}
	}
	else if (DISABLE_BOXCOLLISION == NotifyName)
	{
		if (Weapon)
		{
			Weapon->GetBoxComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Weapon->ClearActorsToIgnore();
		}
		else
		{
			//UE_LOG(LogTemp, Display, TEXT("DISABLE_BOXCOLLISION"));

			DisableHitCapsulesCollision();

			//GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

			//FBodyInstance* BodyInstance = GetMesh()->GetBodyInstance(TEXT("weapon_r"));
			//if (BodyInstance)
			//{
			//	BodyInstance->SetShapeCollisionEnabled(0, ECollisionEnabled::NoCollision);
			//	FCollisionResponseContainer FRC;
			//	FRC.SetResponse(ECC_SkeletalMesh, ECollisionResponse::ECR_Ignore);
			//	BodyInstance->SetShapeResponseToChannels(0, FRC);
			//}
			//else
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("No BodyInstance"));
			//}
		}

	}
	else if (NotifyName == DEADMONTAGE_END_CHECK)
	{
		GetMesh()->GetAnimInstance()->Montage_Pause(DeadMontage);
		SetLifeSpan(5.f);
		return;
	}
	else if (NotifyName == ENABLE_COLLISION)
	{
		//UE_LOG(LogTemp, Display, TEXT("EnableCollision"));
		//SetActorEnableCollision(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		return;
	}
	else if (NotifyName == DISABLE_COLLISION)
	{
		//SetActorEnableCollision(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		return;
	}



	else if (RELOAD_FINISHED == NotifyName)
	{
		//UE_LOG(LogTemp, Display, TEXT("ReloadFinished"));
		FinishReloading();
		return;
	}
	else if (SHELL == NotifyName)
	{
		ShotgunShellReload();
		return;
	}
	else if (THROW_GRENADE_FINISHED == NotifyName)
	{
		//UE_LOG(LogTemp, Display, TEXT("ThrowGrenadeFinished"));

		ThrowGrenadeFinished();
		return;
	}
	else if (GRENADE_LAUNCH == NotifyName)
	{
		//UE_LOG(LogTemp, Display, TEXT("GrenadeLaunch"));
		DisableAttachedGrenade();
	}
	else if (SWAP_ATTACHED_WEAPON == NotifyName)
	{
		FinishSwapAttachWeapons();
	}
	else if (SLIDE_END == NotifyName)
	{
		UnCrouch();
	}
	else if (DODGE_END == NotifyName)
	{
		//bIsDodge = false;
		//CombatState = ECombatState::ECS_Unoccupied;
		UnCrouch();
	}
	else if (VAULT_OR_MANTLE_END == NotifyName)
	{
		bIsMorV = false;
		SetCombatState(ECombatState::ECS_Unoccupied);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//bUseControllerRotationYaw = true;
	}
	else if (AURA_LEFT == NotifyName)
	{
		//UE_LOG(LogTemp, Display, TEXT("AURA_LEFT"));

		SpawnMagic(TEXT("Socket_LeftTrailEnd"));
	}
	else if (AURA_RIGHT == NotifyName)
	{
		SpawnMagic(TEXT("Socket_RightTrailEnd"));
	}
	else if (SPAWN_ENEMY_RIGHT == NotifyName)
	{
		SpawnCharacter(GetActorLocation() + GetActorRightVector() * 100.f);
	}
	else if (SPAWN_ENEMY_LEFT == NotifyName)
	{
		SpawnCharacter(GetActorLocation() - GetActorRightVector() * 100.f);
	}
	else if (PARRY_START == NotifyName)
	{
		bIsParring = true;
	}
	else if (PARRY_END == NotifyName)
	{
		bIsParring = false;
	}
	else if (BOLT == NotifyName)
	{
		//Bolt();
		if (IsLocallyControlled()) ServerBolt();
	}
	else if (GROUND_ATTACK == NotifyName)
	{
		GroundAttack(TEXT("Player"));
	}
	else if (SLOMO_START == NotifyName)
	{
		CustomTimeDilation = 0.1f;
	}
	else if (SLOMO_END == NotifyName)
	{
		CustomTimeDilation = 1.f;
	}
	else if (STANDING == NotifyName)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		GetMesh()->GetAnimInstance()->Montage_Stop(0.25f);
		CombatState = ECombatState::ECS_Unoccupied;
		bDisableGameplay = false;
		GetMesh()->SetCollisionResponseToChannel(ECC_CanDamagedByWeapon, ECR_Block);

	}
}

void ACharacterBase::OnMontageEndedFunc(UAnimMontage* Montage, bool bInterrupted)
{
	//UE_LOG(LogTemp, Warning, TEXT("Owner : %s, OnMontageEnded : %s, Interrupt : %d"), *GetName(), *Montage->GetName(), bInterrupted);



	if (bInterrupted)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Interrupt! : %s"), *Montage->GetName());

		//if (Montage == AttackMontage || Montage == HitReactMontage)
		//{
		//	CombatState = ECombatState::ECS_Unoccupied;
		//}

		if (Montage == ReloadMontage)
		{
			bLocallyReloading = false;
		}
	}

	if (!bInterrupted)
	{
		if (CombatState == ECombatState::ECS_Parried) return;

		//AAIController* AIController = Cast<AAIController>(GetController());
		//if (AIController && AIController->GetBlackboardComponent())
		//{
		//	AIController->GetBlackboardComponent()->SetValueAsEnum(E_COMBAT_STATE, (int)CombatState);
		//}

		CombatState = ECombatState::ECS_Unoccupied;



		if (Montage == HitReactMontage)
		{
			//UE_LOG(LogTemp, Display, TEXT("HitEnded"));
			CombatState = ECombatState::ECS_Unoccupied;
		}
		else if (Montage == DodgeMontage)
		{
			//UnCrouch();
			bIsDodge = false;
			CombatState = ECombatState::ECS_Unoccupied;

		}
		else if (Montage == SlideMontage)
		{
			bIsSliding = false;
			CombatState = ECombatState::ECS_Unoccupied;

		}
		else if (Montage == AttackMontage)
		{
			CombatState = ECombatState::ECS_Unoccupied;
			//CrosshairShootingFactor = 0.75f;
		}
		else if (CombatState == ECombatState::ECS_SwappingWeapon)
		{
			UE_LOG(LogTemp, Display, TEXT("ECS_SwappingWeapon finished"));
			//CombatComponent->CombatState = ECombatState::ECS_Unoccupied;
			bFinishedSwapping = true;
			if (SecondaryWeapon) SecondaryWeapon->EnableCustomDepth(true);
			CombatState = ECombatState::ECS_Unoccupied;
		}
		else if (Montage == VaultMontage || Montage == MantleMontage)
		{
			bUseControllerRotationYaw = true;

			//UE_LOG(LogTemp, Display, TEXT("OnMontageEnded : ECS_VaultOrMantle"));
			//CombatComponent->bIsDodge = false;
			//CombatComponent->CombatState = ECombatState::ECS_Unoccupied;
			//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else if (Montage == BoltActionMontage)
		{
			AttachActorToRightHand(EquippedWeapon);
		}


	}


	//UE_LOG(LogTemp, Display, TEXT("bInterrupted : %d"), bInterrupted);



}

void ACharacterBase::PlayHitReactMontage(const FVector& InHitPoint)
{
	if (HitReactMontage == nullptr || CombatState != ECombatState::ECS_Unoccupied) return;
	//UE_LOG(LogTemp, Display, TEXT("CharacterHitPoint : %f, %f, %f"), CharacterHitPoint.X, CharacterHitPoint.Y, CharacterHitPoint.Z);
	//UE_LOG(LogTemp, Display, TEXT("Combatstate : %s"), *UEnum::GetDisplayValueAsText(CombatState).ToString());
	FName Section = CalculateHitDirection(InHitPoint);
	//UE_LOG(LogTemp, Display, TEXT("PlayHitReactMontage Point : %s"), *Section.ToString());

	PlayMontage(GetMesh()->GetAnimInstance(), HitReactMontage, Section, -1);
	SetState(CharacterState, ECombatState::ECS_HitReact);

	//UE_LOG(LogTemp, Display, TEXT("HitStarted"));
}

void ACharacterBase::PlayMontage(UAnimInstance* AnimInstance, UAnimMontage* Montage, FName SectionName, int32 Rand)
{
	if (Montage)
	{
		AnimInstance->Montage_Play(Montage);
		if (Rand == -1)
		{
			AnimInstance->Montage_JumpToSection(SectionName);
		}
		else
		{
			AnimInstance->Montage_JumpToSection(*FString::Printf(TEXT("%s%d"), *SectionName.ToString(), Rand), Montage);
		}


	}
}

void ACharacterBase::PlayRandomMontage(UAnimInstance* AnimInstance, UAnimMontage* Montage, FString Prefix)
{
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		int32 Rand = FMath::RandRange(0, Montage->CompositeSections.Num() - 1);
		AnimInstance->Montage_JumpToSection(*FString::Printf(TEXT("%s%d"), *Prefix, Rand), Montage);
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("No AnimInstance or AttackMontage"));
	}
}

void ACharacterBase::DoRagdollImpulse()
{
	//충돌 방향 설정 (바라보는 방향 뒤로)
		//FVector ImpulseDirection = GetActorRotation().Vector() * -1.0f;
	FVector ImpulseDirection;

	//충돌 세기
	float ImpulseStrength = 1000.f;
	if (CharacterHitPoint != FVector(0.f))
	{
		//UE_LOG(LogTemp, Display, TEXT("Not Radial Hit"));
		FVector ALSize2D(GetActorLocation().X, GetActorLocation().Y, 0.f);
		FVector HPSize2D(CharacterHitPoint.X, CharacterHitPoint.Y, 0.f);
		ImpulseDirection = (ALSize2D - HPSize2D).GetSafeNormal();
		ImpulseStrength = 250.f;
	}
	else if (LastDamageCauser)
	{
		ImpulseDirection = (GetActorLocation() - LastDamageCauser->GetActorLocation()).GetSafeNormal();
	}

	//UE_LOG(LogTemp, Display, TEXT("CharacterHitPoint : %f, %f, %f"), CharacterHitPoint.X, CharacterHitPoint.Y, CharacterHitPoint.Z);

	FVector FinalImpulse = ImpulseDirection * ImpulseStrength;
	//FieldSystemComponent->ApplyLinearForce(true, GetActorLocation(), Magnitude);
	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector); //렉돌에 충격을 가하다보면 기존 애니메이션의 속도가 더해져서 인지 가끔 날라가는 속도가 비정상적으로 빠르게 연출되는 경우가 있다. 이를 보완 하기 위해 충격을 주기전 메시의 현재속도를 0으로 초기화 해주었다.
	GetMesh()->AddImpulseToAllBodiesBelow(FinalImpulse, NAME_None);
}

void ACharacterBase::UpdateMotionWarpingTransform()
{
	FName Default = TEXT("Default");
	FName WarpForward = TEXT("WarpForward");
	FName WarpBackward = TEXT("WarpBackward");
	FName WarpLeft = TEXT("WarpLeft");
	FName WarpRight = TEXT("WarpRight");

	//UE_LOG(LogTemp, Display, TEXT("UpdateMotionWarpingTransform"));


	FTransform DefaultTransform(GetActorLocation());
	FTransform ForwardTransform(GetActorLocation() + GetActorForwardVector() * 100.f);
	FTransform BackwardTransform(GetActorLocation() - GetActorForwardVector() * 200.f);

	if (MotionWarpingComponent)
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(Default, DefaultTransform);
		MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(WarpForward, ForwardTransform);

		MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(WarpBackward, BackwardTransform);
	}

}

void ACharacterBase::RandomAttack(int32 Index, const FString& AttackType)
{
	if (CombatState != ECombatState::ECS_Unoccupied) return;

	//UE_LOG(LogTemp, Display, TEXT("RandomAttack"));

	bool ArmedAndOneHanded =
		CharacterState == ECharacterState::ECS_EquippedOnehandedWeapon ||
		CharacterState == ECharacterState::ECS_EquippedTwoHandedWeapon ||
		CharacterState == ECharacterState::ECS_EquippedCapsuleWeapon;

	if (ArmedAndOneHanded)
	{
		//UE_LOG(LogTemp, Display, TEXT("Attack!"));
		//float BackStepQuantity = 50.f;

		//FVector Start = GetActorLocation();
		//FVector End = GetActorLocation() + GetActorForwardVector() * BackStepQuantity;
		//ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_SkeletalMesh);
		//TArray<AActor*> ActorsToIgnore;
		//ActorsToIgnore.Add(this);
		//ActorsToIgnore.Add(Weapon);
		//FHitResult HitResult;
		//UKismetSystemLibrary::BoxTraceSingle(this, Start, End, FVector(20.f), GetActorRotation(), TraceTypeQuery, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

		//if (HitResult.GetActor())
		//{
		//	SetActorLocation(GetActorLocation() - GetActorForwardVector() * BackStepQuantity);
		//}

		//PlayRandomMontage(GetMesh()->GetAnimInstance(), AttackMontage, TEXT("Attack"));


		FString SectionName = AttackType + TEXT("Attack");

		PlayMontage(GetMesh()->GetAnimInstance(), AttackMontage, *SectionName, Index);
		SetState(CharacterState, ECombatState::ECS_Attacking);
	}
	else if (CharacterState == ECharacterState::ECS_EquippedGun)
	{
		FireButtonPressed(true);
		//PlayMontage(GetMesh()->GetAnimInstance(), AttackMontage, TEXT("RifleAim"), -1);
		SetState(CharacterState, ECombatState::ECS_Attacking);
	}
}

void ACharacterBase::GroundAttack(FName OpponentTag)
{
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	//UGameplayStatics::ApplyRadialDamageWithFalloff(this, BaseDamage, MinimumDamage, GetActorLocation(), DamageInnerRadius, DamageOuterRadius, 1.f, UDamageType::StaticClass(), IgnoreActors, this, GetInstigatorController(), ECC_CanDamagedByWeapon);

	//FTransform SpawnT = GetActorTransform();
	//SpawnT.SetLocation(GetActorLocation() + GetActorLocation() * GetActorForwardVector() * 50.f);
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GroundEffect, GetActorTransform());

	//GroundEffect->Activate();
	//GroundEffect->Deactivate();

	UGameplayStatics::PlayWorldCameraShake(this, GroundCameraShakeClass, GetActorLocation(), 0.f, 4000.f);
	UGameplayStatics::PlaySoundAtLocation(this, GroundSound, GetActorLocation());

	//ETraceTypeQuery CanDamagedByWeapon = UEngineTypes::ConvertToTraceType(ECC_CanDamagedByWeapon);
	ETraceTypeQuery CanDamagedByWeapon = UEngineTypes::ConvertToTraceType(ECC_CanDamagedByWeapon);


	TArray<FHitResult> OutHitResults;
	FVector Range(2000.f);
	Range.Z = 20.f;

	FVector Start = GetMesh()->GetSocketLocation(TEXT("root"));
	FVector End = GetMesh()->GetSocketLocation(TEXT("pelvis"));

	UKismetSystemLibrary::BoxTraceMulti(this, Start, End, Range, GetActorRotation(), CanDamagedByWeapon, false, IgnoreActors, EDrawDebugTrace::ForDuration, OutHitResults, true);

	TSet<AActor*> CheckActors;
	CheckActors.Reserve(OutHitResults.Num());
	
	for (size_t i = 0; i < OutHitResults.Num(); i++)
	{
		//if (OutHitResults[i].bBlockingHit && !OutHitResults[i].GetActor()->Tags.IsEmpty() && OutHitResults[i].GetActor()->Tags[0].IsValid() && OutHitResults[i].GetActor()->Tags[0] == OpponentTag)

		if (OutHitResults[i].bBlockingHit && OutHitResults[i].GetActor()->GetInstigatorController() && !OutHitResults[i].GetActor()->GetInstigatorController()->Tags.IsEmpty() && OutHitResults[i].GetActor()->GetInstigatorController()->Tags[0] == OpponentTag)
		{
			if (CheckActors.Contains(OutHitResults[i].GetActor())) continue;
			CheckActors.Add(OutHitResults[i].GetActor());

			//UE_LOG(LogTemp, Display, TEXT("Hit : %s"), *OutHitResults[i].GetActor()->GetName());
			UGameplayStatics::ApplyDamage(OutHitResults[i].GetActor(), GroundAttackDamage, GetInstigatorController(), this, UDamageType_Rumbling::StaticClass());

		}
	}



	//if (OutHitResult.bBlockingHit && !OutHitResult.GetActor()->Tags.IsEmpty() && OutHitResult.GetActor()->Tags[0].IsValid() && OutHitResult.GetActor()->Tags[0] == TEXT("Player"))
	//{
	//	UE_LOG(LogTemp, Display, TEXT("Hit : %s"), *OutHitResult.GetActor()->GetName());
	//	UGameplayStatics::ApplyDamage(OutHitResult.GetActor(), 10.f, GetInstigatorController(), this, UDamageType::StaticClass());
	//}

}

void ACharacterBase::CalculateMeshLocation()
{
	//UE_LOG(LogTemp, Display, TEXT("CalculateMeshLocation"));
	FVector PelvisLocation = GetMesh()->GetSocketLocation(TEXT("pelvis"));
	FVector End = PelvisLocation + FVector(0.f, 0.f, -1.f) * 100.f;

	FVector MeshZOffset = -GetMesh()->GetRelativeLocation();

	ETraceTypeQuery Visibility = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult OutHitResult;
	UKismetSystemLibrary::LineTraceSingle(this, PelvisLocation, End, Visibility, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHitResult, true);

	if (OutHitResult.bBlockingHit)
	{
		CapsuleLocation = OutHitResult.Location + MeshZOffset;
	}
	else
	{
		CapsuleLocation = PelvisLocation;
	}

	MeshLocation = FMath::VInterpTo(MeshLocation, CapsuleLocation, GetWorld()->GetDeltaSeconds(), 2.5f);
	//MeshLocation = FMath::VInterpTo(MeshLocation, CapsuleLocation, GetWorld()->GetDeltaSeconds(), 25.f);

}

void ACharacterBase::SetRagdollCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetConstraintProfileForAll(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	//GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
}

void ACharacterBase::ServerExecuteRagdoll_Implementation()
{
	ExecuteRagdollFunc();
	SetReplicateMovement(false);
}

void ACharacterBase::ExecuteRagdoll()
{
	//if (!HasAuthority()) ExecuteRagdollFunc();
	ServerExecuteRagdoll();
}

void ACharacterBase::ExecuteRagdollFunc()
{
	FireTimer.Invalidate();
	bDisableGameplay = true;
	bRagdollInProgress = true;

	//bIsRagdollStateStopped = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector, true);
	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
	GetMesh()->SetCollisionResponseToChannel(ECC_CanDamagedByWeapon, ECR_Ignore);

	FTimerHandle T;
	GetWorldTimerManager().SetTimer(T, FTimerDelegate::CreateLambda([&]()
		{
			SetRagdollCollision();
			GetMesh()->SetAllPhysicsPosition(GetActorLocation());
			GetMesh()->SetAllPhysicsRotation(GetActorRotation());
			CombatState = ECombatState::ECS_Ragdoll;

		}), 0.1f, false);
}

void ACharacterBase::ServerExecutePhysicsRecover_Implementation()
{
	//UE_LOG(LogTemp, Display, TEXT("ServerExecutePhysicsRecover_Implementation"));
	SetReplicateMovement(true);
	ExecutePhysicsRecoverFunc();
}

void ACharacterBase::ExecutePhysicsRecover()
{
	ServerExecutePhysicsRecover();
}

void ACharacterBase::ExecutePhysicsRecoverFunc()
{
	CombatState = ECombatState::ECS_PhysicsRecover;

	bRagdollInProgress = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	CachePose();

	FTimerHandle T;
	GetWorldTimerManager().SetTimer(T, FTimerDelegate::CreateLambda([&]()
		{
			GetMesh()->SetSimulatePhysics(false);
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			GetMesh()->GetAnimInstance()->Montage_Play(GetUpMontage);
			bIsRagdollStateStopped = false;
			//SetReplicateMovement(true);
		}), 0.2, false);
}

void ACharacterBase::CalculateFacingDirection()
{
	// 부가설명: 왜 RightVector 를 구하느냐? SkeletalMesh 에서 보통 pelvis 의 X축은 위를 향하고 Y축은 앞을 향해있음. 그러므로 우리가 구하고자 하는 통상적인 ForwardVector 를 구하기 위해선 Y방향을 가져와야 하기 때문에 일반적으로 Y 방향을 가르키는 RightVector 를 사용해야 전방벡터를 가져올 수가 있다. 즉, 여기서의 RightVector 는 말만 RightVector 이지 SkeletalMesh 입장에서는 ForwardVector와 같은 의미가 된다.

	FVector MeshForwardVec = UKismetMathLibrary::GetRightVector(GetMesh()->GetSocketRotation(TEXT("pelvis")));

	FVector SocketLocation = GetMesh()->GetSocketLocation(TEXT("pelvis"));
	//DrawDebugLine(GetWorld(), SocketLocation, SocketLocation + MeshForwardVec * 100.f, FColor::Cyan, false, 10.f);


	//bIsFacingUp = GetMesh()->GetSocketTransform(TEXT("pelvis")).GetTranslation().RightVector.Dot(FVector(0.f, 0.f, 1.f)) >= 0.f;
	bIsFacingUp = MeshForwardVec.Dot(FVector(0.f, 0.f, 1.f)) >= 0.f;
}

void ACharacterBase::SetupGetupOrientation()
{
	// 이 함수의 핵심은 회전을 재설정해주는 것

	FVector NeckLocation = GetMesh()->GetSocketLocation(TEXT("neck_01"));
	FVector PelvisLocation = GetMesh()->GetSocketLocation(TEXT("pelvis"));

	FVector Result;
	if (bIsFacingUp)
	{
		Result = PelvisLocation - NeckLocation;
	}
	else
	{
		Result = NeckLocation - PelvisLocation;
	}

	FRotator Result2 = UKismetMathLibrary::MakeRotFromZX(FVector(0.f, 0.f, 1.f), Result);
	//FRotator Result2 = FRotationMatrix::MakeFromZX(FVector(0.f, 0.f, 1.f), Result).Rotator();

	//FTransform T(Result2, GetMesh()->GetComponentLocation(), FVector(1.f)); // 이것도 잘작동하기는 하는데 미세한 끊김이 발생함
	FTransform T(Result2, MeshLocation, FVector(1.f));

	SetActorTransform(T);
}

void ACharacterBase::SetupGetupMontage()
{
	if (bIsFacingUp)
	{
		GetUpMontage = UpMontage;
	}
	else
	{
		GetUpMontage = DownMontage;
	}
}

void ACharacterBase::CachePose()
{
	CalculateFacingDirection();
	SetupGetupOrientation();
	SetupGetupMontage();

	FTimerHandle T;
	GetWorldTimerManager().SetTimer(T, FTimerDelegate::CreateLambda([&]()
		{
			GetMesh()->GetAnimInstance()->SavePoseSnapshot(SnapshotName);
			bIsRagdollStateStopped = true;
			GetWorldTimerManager().SetTimer(T, FTimerDelegate::CreateLambda([&]()
				{

				}), 0.1f, false);


		}), 0.1f, false);


}

void ACharacterBase::CheckCapsuleAndMeshEquals(float DeltaTime, float InAccuracy)
{
	CheckCapsuleAndMeshTimer += DeltaTime;

	if (LastCapsuleLocation.Equals(GetCapsuleComponent()->GetComponentLocation(), InAccuracy))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Equals"));
		bIsRagdollComplete = true;
		//return true;
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("Not Equals"));

		bIsRagdollComplete = false;
		//return false;
	}

	if (CheckCapsuleAndMeshTimer <= CheckCapsuleAndMeshThreshold) return;

	CheckCapsuleAndMeshTimer = 0.f;

	LastCapsuleLocation = GetCapsuleComponent()->GetComponentLocation();

	return;
}

void ACharacterBase::MulticastRandomAttack_Implementation(int32 Index, const FString& AttackType)
{
	RandomAttack(Index, AttackType);
}

void ACharacterBase::SetState(ECharacterState InCharacterState, ECombatState InActionState)
{
	// State Setting
	CharacterState = InCharacterState;
	CombatState = InActionState;
}

void ACharacterBase::OnRep_CombatState()
{
	FireButtonPressed(false);
	bIsFiring = false;

	switch (CombatState)
	{
	case ECombatState::ECS_Unoccupied:
		if (bFireButtonPressed)
		{
			FireButtonPressed(true);
		}
		break;
	case ECombatState::ECS_Reloading:
		//if (!IsLocallyControlled()) HandleReload();
		break;
	case ECombatState::ECS_ThrowingGrenade:
		//if (Character && !IsLocallyControlled()) // Simulate Proxy 만 재생하게 혹시 모르니 조건문 걸은거임
		//{
		//	PlayThrowGrenadeMontage();
		//}
		PlayThrowGrenadeMontage();
		ShowAttachedGrenade(true);

		break;
	case ECombatState::ECS_SwappingWeapon:
		PlaySwapMontage();
		break;
	case ECombatState::ECS_Dodging:
		if (!HasAuthority() && !IsLocallyControlled())
		{
			DodgeFunc(KeySectionName);
		}
		break;
	case ECombatState::ECS_VaultOrMantle:
		if (!HasAuthority() && !IsLocallyControlled())
		{
			VaultOrMantleFunc();
		}
		break;
	case ECombatState::ECS_Ragdoll:
		//UE_LOG(LogTemp, Display, TEXT("Role : %s, OnRep ECS_Ragdoll"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString());
		//if (!HasAuthority() && !IsLocallyControlled())
		ExecuteRagdollFunc();
		break;
	case ECombatState::ECS_PhysicsRecover:
		//if (!HasAuthority() && !IsLocallyControlled())
		ExecutePhysicsRecoverFunc();
		break;
	case ECombatState::ECS_MAX:
		break;
	default:
		break;
	}
}

void ACharacterBase::SetWeaponCollision(ECollisionEnabled::Type Type)
{
	if (Weapon)
	{
		Weapon->GetBoxComponent()->SetCollisionEnabled(Type);
	}
}

void ACharacterBase::EnableHitCapsulesCollision()
{
	HitCollisionCapsules[TEXT("weapon_r")]->SetGenerateOverlapEvents(true);
	HitCollisionCapsules[TEXT("weapon_r")]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HitCollisionCapsules[TEXT("weapon_r")]->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	HitCollisionCapsules[TEXT("weapon_r")]->SetCollisionResponseToChannel(ECC_Destructible, ECollisionResponse::ECR_Overlap);

	HitCollisionCapsules[TEXT("weapon_l")]->SetGenerateOverlapEvents(true);
	HitCollisionCapsules[TEXT("weapon_l")]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HitCollisionCapsules[TEXT("weapon_l")]->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	HitCollisionCapsules[TEXT("weapon_l")]->SetCollisionResponseToChannel(ECC_Destructible, ECollisionResponse::ECR_Overlap);
}

void ACharacterBase::DisableHitCapsulesCollision()
{
	HitCollisionCapsules[TEXT("weapon_r")]->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
	HitCollisionCapsules[TEXT("weapon_l")]->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);

	HitCollisionCapsules[TEXT("weapon_r")]->SetCollisionResponseToChannel(ECC_Destructible, ECollisionResponse::ECR_Ignore);
	HitCollisionCapsules[TEXT("weapon_l")]->SetCollisionResponseToChannel(ECC_Destructible, ECollisionResponse::ECR_Ignore);
}

void ACharacterBase::AttachToSocket(AItem* Item, USkeletalMeshComponent* SkeletalMesh, FName SocketName)
{
	// if Item is Weapon
	AWeapon_Melee* Weapon1 = Cast<AWeapon_Melee>(Item);

	if (Weapon1)
	{
		//Weapon1->SetOwner(this);
		//Weapon1->SetInstigator(this);

		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
		Weapon1->ItemAttachToComponent(SkeletalMesh, Rules, SocketName);
	}
}

void ACharacterBase::InitializeCollisionStates()
{
	GetCapsuleComponent()->SetCollisionProfileName(PROFILE_Pawn);
	GetMesh()->SetCollisionProfileName(PROFILE_CharacterMesh);

	GetMesh()->SetGenerateOverlapEvents(true);
	//GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_CanDamagedByWeapon, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_CanMorV, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_HitCapsule, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_CanDamagedByWeapon, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_CanMorV, ECollisionResponse::ECR_Ignore);

}

void ACharacterBase::InitializeWidgets()
{
}

void ACharacterBase::InitializeDelegates()
{
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ACharacterBase::OnPlayMontageNotifyBeginFunc);
		GetMesh()->GetAnimInstance()->OnMontageEnded.AddUniqueDynamic(this, &ACharacterBase::OnMontageEndedFunc);
	}

	if (HasAuthority())
	{
		OnTakeAnyDamage.AddUniqueDynamic(this, &ThisClass::ReceiveDamage);
	}

}

void ACharacterBase::InitializeDefaults()
{
	InitDashCurve();

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CombatComponent->SetIsReplicated(true);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	if (GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	// Ragdoll
	CheckCapsuleAndMeshTimer = CheckCapsuleAndMeshThreshold;

}

FName ACharacterBase::CalculateHitDirection(const FVector& InHitPoint)
{
	const FVector ForwardVec = GetActorForwardVector();
	//const FVector HitVector = (CharacterHitPoint - GetActorLocation()).GetSafeNormal();
	const FVector H = FVector(InHitPoint.X, InHitPoint.Y, GetActorLocation().Z);
	const FVector HitVector = (H - GetActorLocation()).GetSafeNormal();

	float cos = FVector::DotProduct(ForwardVec, HitVector);
	float acos = FMath::Acos(cos);
	float theta = FMath::RadiansToDegrees(acos);

	const FVector Cross = FVector::CrossProduct(ForwardVec, HitVector).GetSafeNormal();

	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ForwardVec * 60.f, 5.f, FLinearColor::Blue, 5.f, 3.f);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + HitVector * 60.f, 5.f, FLinearColor::Green, 5.f, 3.f);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Cross * 120.f, 5.f, FLinearColor::Red, 5.f, 3.f);

	//UE_LOG(LogTemp, Display, TEXT("Cross : %f, %f, %f"), Cross.X, Cross.Y, Cross.Z);

	if (Cross.Z < 0.f)
	{
		theta *= -1.f;
	}

	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, FString::Printf(TEXT("Theta : %f"), theta));
	}

	FName Section;

	if (theta >= -45.f && theta < 45.f)
	{
		Section = TEXT("FromFront");
	}
	else if (theta >= 45.f && theta < 135.f)
	{
		Section = TEXT("FromRight");
	}
	else if (theta >= -135.f && theta < -45.f)
	{
		Section = TEXT("FromLeft");
	}
	else
	{
		Section = TEXT("FromBack");
	}

	return Section;
}

void ACharacterBase::Destroyed()
{
	Super::Destroyed();

	//UE_LOG(LogTemp, Display, TEXT("Destroyed"));

	if (Weapon)
	{
		Weapon->Destroy();
	}
}

#if WITH_EDITOR
void ACharacterBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 변경된 프로퍼티의 이름을 가져온다.
	FName PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// 변경된 프로퍼티에 따라 작업을 수행
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, StartingARAmmo))
	{

		//if (ProjectileMovementComponent)
		//{
		//	// InitialSpeed를 에디터에서 수정하면 자동으로 ProjectileMovementComponent->InitialSpeed, MaxSpeed 값 둘다 변경이 된다.
		//	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
		//	ProjectileMovementComponent->MaxSpeed = InitialSpeed;
		//}
	}
}
#endif // WITH_EDITOR

void ACharacterBase::PollInit()
{
	BlasterGameMode = BlasterGameMode == nullptr ? Cast<ABlasterGameMode>(GetWorld()->GetAuthGameMode()) : BlasterGameMode;
}

void ACharacterBase::MulticastGainedTheLead_Implementation()
{
	if (CrownNiagaraSystem == nullptr) return;
	if (CrownNiagaraComponent == nullptr)
	{
		CrownNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(CrownNiagaraSystem, GetMesh(), FName(), GetActorLocation() + FVector(0.f, 0.f, 110.f), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
	}

	if (CrownNiagaraComponent)
	{
		CrownNiagaraComponent->Activate();
	}
}

void ACharacterBase::MulticastLostTheLead_Implementation()
{
	if (CrownNiagaraComponent)
	{
		CrownNiagaraComponent->DestroyComponent();
	}
}

ECombatState ACharacterBase::GetCombatState() const
{
	return CombatState;
}

ECharacterState ACharacterBase::GetCharacterState() const
{
	return CharacterState;
}

FVector ACharacterBase::GetHitTarget() const
{
	return HitTarget;
}

ETeam ACharacterBase::GetTeam()
{
	BlasterPlayerState = BlasterPlayerState == nullptr ? GetPlayerState<ABlasterPlayerState>() : BlasterPlayerState;
	if (BlasterPlayerState == nullptr) return ETeam::ET_NoTeam;

	return BlasterPlayerState->GetTeam();
}

void ACharacterBase::SetHoldingTheFlag(bool bHolding)
{
	if (CombatComponent == nullptr) return;

	bHoldingTheFlag = bHolding;
}

bool ACharacterBase::IsWeaponEquipped()
{
	return EquippedWeapon == nullptr ? false : true;
}

bool ACharacterBase::IsAiming()
{
	return (bIsAiming);
}

bool ACharacterBase::IsLocallyReloading()
{
	return bLocallyReloading;
}

bool ACharacterBase::IsHoldingTheFlag() const
{
	return bHoldingTheFlag;
}

void ACharacterBase::PlayFireMontage(bool InbIsAiming)
{
	if (!EquippedWeapon) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		FName SectionName;
		SectionName = InbIsAiming ? TEXT("RifleAim") : TEXT("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void ACharacterBase::PlayDeadMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr) return;
	if (DeadMontage)
	{
		//UE_LOG(LogTemp, Display, TEXT("PlayDeadMontage"));
		AnimInstance->Montage_Play(DeadMontage);
		AnimInstance->Montage_JumpToSection(TEXT("Forward"), DeadMontage);

		//DeadMontage->CompositeSections.Num();
	}
}

//void ACharacterBase::PlayCombatHitReactMontage()
//{
//	if (!CombatComponent || !CombatComponent->EquippedWeapon) return;
//
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance && HitReactMontage)
//	{
//
//		AnimInstance->Montage_Play(HitReactMontage);
//		FName SectionName(TEXT("FromFront"));
//		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
//
//
//	}
//}

void ACharacterBase::PlayReloadMontage()
{
	if (!EquippedWeapon) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
		FName SectionName;
		switch (EquippedWeapon->GetWeaponType())
		{
		case EWeaponType::EWT_AssaultRifle:
			//UE_LOG(LogTemp, Display, TEXT("%s"), *UEnum::GetDisplayValueAsText(EWeaponType::EWT_AssaultRifle).ToString());
			//SectionName = TEXT("Rifle");
			SectionName = *UEnum::GetDisplayValueAsText(EWeaponType::EWT_AssaultRifle).ToString();
			break;
		case EWeaponType::EWT_RocketLauncher:
			//SectionName = TEXT("RocketLauncher");
			SectionName = *UEnum::GetDisplayValueAsText(EWeaponType::EWT_RocketLauncher).ToString();

			break;
		case EWeaponType::EWT_Pistol:
			//SectionName = TEXT("Pistol");
			SectionName = *UEnum::GetDisplayValueAsText(EWeaponType::EWT_Pistol).ToString();

			break;
		case EWeaponType::EWT_SMG:
			//SectionName = TEXT("SMG");
			SectionName = *UEnum::GetDisplayValueAsText(EWeaponType::EWT_Pistol).ToString();

			break;
		case EWeaponType::EWT_Shotgun:
			//SectionName = TEXT("Shotgun");
			SectionName = *UEnum::GetDisplayValueAsText(EWeaponType::EWT_Shotgun).ToString();

			break;
		case EWeaponType::EWT_SniperRifle:
			//SectionName = TEXT("SniperRifle");
			SectionName = *UEnum::GetDisplayValueAsText(EWeaponType::EWT_SniperRifle).ToString();

			break;
		case EWeaponType::EWT_GrenadeLauncher:
			//SectionName = TEXT("GrenadeLauncher");
			SectionName = *UEnum::GetDisplayValueAsText(EWeaponType::EWT_GrenadeLauncher).ToString();

			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, ReloadMontage);
	}
}

void ACharacterBase::PlayThrowGrenadeMontage()
{

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ThrowGrenadeMontage)
	{
		AnimInstance->Montage_Play(ThrowGrenadeMontage);
	}
}

void ACharacterBase::PlaySwapMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SwapMontage)
	{
		AnimInstance->Montage_Play(SwapMontage);
	}
}

void ACharacterBase::PlaySlideMontage()
{
	//UE_LOG(LogTemp, Display, TEXT("PlaySlideMontage"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SlideMontage)
	{
		AnimInstance->Montage_Play(SlideMontage);
	}
}

void ACharacterBase::PlayDodgeMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DodgeMontage)
	{
		AnimInstance->Montage_Play(DodgeMontage);
		AnimInstance->Montage_JumpToSection(SectionName, DodgeMontage);
	}
}

void ACharacterBase::PlayVaultMontage(FName SectionName)
{
	//UE_LOG(LogTemp, Display, TEXT("CombatState : %s"), *UEnum::GetDisplayValueAsText(CombatState).ToString());

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && VaultMontage)
	{
		AnimInstance->Montage_Play(VaultMontage);
		if (SectionName.ToString() == TEXT("")) return;
		AnimInstance->Montage_JumpToSection(SectionName, VaultMontage);
	}
}

void ACharacterBase::PlayMantleMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && MantleMontage)
	{
		AnimInstance->Montage_Play(MantleMontage);
		if (SectionName.ToString() == TEXT("")) return;
		AnimInstance->Montage_JumpToSection(SectionName, MantleMontage);
	}
}

void ACharacterBase::PlayBoltActionMontage(FName SectionName)
{

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && BoltActionMontage)
	{
		CombatState = ECombatState::ECS_Reloading;
		AttachActorToLeftHand(EquippedWeapon);

		AnimInstance->Montage_Play(BoltActionMontage);
		if (SectionName.ToString() == TEXT("")) return;
		AnimInstance->Montage_JumpToSection(SectionName, BoltActionMontage);
	}
}

void ACharacterBase::UpdateDissolveMaterial(float DissolveValue)
{
	if (DynamicDissolveMaterialInstance)
	{
		//UE_LOG(LogTemp, Display, TEXT("UpdateDissolveMaterial"));
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), DissolveValue);
	}
}

void ACharacterBase::StartDissolve()
{
	DissolveTrack.BindDynamic(this, &ThisClass::UpdateDissolveMaterial);
	if (DissolveCurve && DissolveTimelineComponent)
	{
		DissolveTimelineComponent->AddInterpFloat(DissolveCurve, DissolveTrack);
		DissolveTimelineComponent->Play();
	}
}

void ACharacterBase::SetTeamColor(ETeam InTeam)
{
	if (!IsValid(GetMesh()) || !IsValid(OriginalMaterial)) return;
	switch (InTeam)
	{
	case ETeam::ET_NoTeam:
		GetMesh()->SetMaterial(1, OriginalMaterial);
		DissolveMaterialInstance = BlueDissolveMatInst;
		break;
	case ETeam::ET_RedTeam:
		GetMesh()->SetMaterial(1, RedMaterial);
		DissolveMaterialInstance = RedDissolveMatInst;
		break;
	case ETeam::ET_BlueTeam:
		GetMesh()->SetMaterial(1, BlueMaterial);
		DissolveMaterialInstance = BlueDissolveMatInst;
		break;
	case ETeam::ET_MAX:
		break;
	default:
		break;
	}
}

void ACharacterBase::Elim(bool bPlayerLeftGame)
{
	if (EquippedWeapon)
	{
		DropOrDestroyWeapon(EquippedWeapon);
	}

	if (SecondaryWeapon)
	{
		DropOrDestroyWeapon(SecondaryWeapon);
	}

	if (Flag)
	{
		bHoldingTheFlag = false;
		Flag->Dropped();
	}

	MulticastElim(bPlayerLeftGame);
}

void ACharacterBase::DropOrDestroyWeapon(AWeapon* InWeapon)
{
	//UE_LOG(LogTemp, Display, TEXT("DropOrDestroyWeapon"));
	if (InWeapon == nullptr) return;

	if (InWeapon->bDestroyWeapon)
	{
		//UE_LOG(LogTemp, Display, TEXT("DropOrDestroyWeapon : Destroy"));
		InWeapon->Destroy();
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("DropOrDestroyWeapon : Dropped"));
		InWeapon->Dropped();

	}
}

void ACharacterBase::MulticastElim_Implementation(bool bPlayerLeftGame)
{
	//UE_LOG(LogTemp, Display, TEXT("MulticastElim_Implementation"));
	//bLeftGame = bPlayerLeftGame;
	//if (BlasterPlayerController && CombatComponent)
	//{
	//	BlasterPlayerController->SetHUDWeaponAmmo(0);
	//}

	bIsElimmed = true;

	if (DissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicDissolveMaterialInstance);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), 0.55f);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Glow"), 200.f);
	}

	StartDissolve();

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	//if (BlasterPlayerController && IsLocallyControlled())
	//{
	//	DisableInput(BlasterPlayerController);
	//}
	bDisableGameplay = true;
	FireButtonPressed(false);

	if (DeadMontage)
	{
		PlayDeadMontage();

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		SetRagdollCollision();
		DoRagdollImpulse();
		SetReplicateMovement(false);

	}

	FTimerHandle Timerhandle;
	GetWorldTimerManager().SetTimer(Timerhandle, FTimerDelegate::CreateLambda([&]()
		{
			PlayDeadMontage();
		}), 0.01f, false);

	if (ElimBotEffect)
	{
		FVector ElimBotSpawnPoint(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 200.f);
		ElimBotEffectComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ElimBotEffect, ElimBotSpawnPoint, GetActorRotation());
	}

	if (ElimBotSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, ElimBotSound, GetActorLocation());
	}

	//bool bHideSniperScope = IsLocallyControlled() && CombatComponent && CombatComponent->bIsAiming && CombatComponent->EquippedWeapon && CombatComponent->EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle;

	//if (bHideSniperScope)
	//{
	//	ShowSniperScopeWidget(false);
	//}

	if (CrownNiagaraComponent)
	{
		CrownNiagaraComponent->DestroyComponent();
	}

	GetWorldTimerManager().SetTimer(ElimTimer, this, &ThisClass::ElimTimerFinished, ElimDelay);

}

void ACharacterBase::SetSpawnPoint()
{
	if (HasAuthority() && BlasterPlayerState->GetTeam() != ETeam::ET_NoTeam)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, ATeamPlayerStart::StaticClass(), PlayerStarts);
		TArray<ATeamPlayerStart*> TeamPlayerStarts;

		for (const auto& Start : PlayerStarts)
		{
			ATeamPlayerStart* TeamStart = Cast<ATeamPlayerStart>(Start);
			if (TeamStart && TeamStart->Team == BlasterPlayerState->GetTeam())
			{
				TeamPlayerStarts.Add(TeamStart);
			}
		}

		if (TeamPlayerStarts.Num() > 0)
		{
			ATeamPlayerStart* ChosenPlayerStart = TeamPlayerStarts[FMath::RandRange(0, TeamPlayerStarts.Num() - 1)];
			SetActorLocationAndRotation(ChosenPlayerStart->GetActorLocation(), ChosenPlayerStart->GetActorRotation());
		}
	}
}

void ACharacterBase::ElimTimerFinished()
{
	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	if (BlasterGameMode)
	{
		BlasterGameMode->RequestRespawn(this, GetController());
	}
}

void ACharacterBase::SpawnDefaultWeapon()
{
	BlasterGameMode = BlasterGameMode == nullptr ? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
	UWorld* World = GetWorld();
	if (BlasterGameMode && World && !bIsElimmed && DefaultWeaponClass)
	{
		AWeapon* StartingWeapon = World->SpawnActor<AWeapon>(DefaultWeaponClass);

		StartingWeapon->bDestroyWeapon = true;
		EquipWeapon(StartingWeapon);
	}
}



bool ACharacterBase::ShouldSwapWeapons()
{
	return EquippedWeapon != nullptr && SecondaryWeapon != nullptr;
}

void ACharacterBase::FinishSwapAttachWeapons()
{
	//UE_LOG(LogTemp, Display, TEXT("%s : FinishSwapAttachWeapons"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString());

	if (HasAuthority())
	{
		AWeapon* Temp = EquippedWeapon;
		EquippedWeapon = SecondaryWeapon;
		SecondaryWeapon = Temp;

		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		AttachActorToRightHand(EquippedWeapon);

		if (AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon)) Gun->SetHUDAmmo();
		UpdateCarriedAmmo();
		PlayEquipWeaponSound();

		SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
		AttachActorToBackpack(SecondaryWeapon);
	}





	//AWeapon* Temp = EquippedWeapon;
	//EquippedWeapon = SecondaryWeapon;
	////EquipWeaponFunc();

	//SecondaryWeapon = Temp;
	////EquipSecondaryFunc();

	/*EquippedWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
	AttachActorToBackpack(EquippedWeapon);

	SecondaryWeapon->SetHUDAmmo();
	UpdateCarriedAmmo();
	PlaySecondaryWeaponSound();

	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	AttachActorToRightHand(SecondaryWeapon);

	AWeapon* Temp = EquippedWeapon;
	EquippedWeapon = SecondaryWeapon;
	SecondaryWeapon = Temp;



	if (SecondaryWeapon) SecondaryWeapon->EnableCustomDepth(false);*/
}

void ACharacterBase::EquipWeaponFunc()
{
	if (EquippedWeapon)
	{
		bUseControllerRotationYaw = true;
		//GetCharacterMovement()->bOrientRotationToMovement = false;
		//bUseControllerRotationYaw = true;
		//UE_LOG(LogTemp, Display, TEXT("Equipped!"));
		//EquippedWeapon->ItemAttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, );

		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		//CharacterState = ECharacterState::EAS_Equipped;

		AttachActorToRightHand(EquippedWeapon);
		PlayEquipWeaponSound();

		EquippedWeapon->ShowPickupWidget(false);
		EquippedWeapon->EnableCustomDepth(false);

		EquippedWeapon->SetOwner(this);
		if (AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon)) Gun->SetHUDAmmo();
		UpdateCarriedAmmo();
		ReloadEmptyWeapon();
	}

}

void ACharacterBase::EquipSecondaryFunc()
{
	if (SecondaryWeapon)
	{
		//GetCharacterMovement()->bOrientRotationToMovement = false;
		//bUseControllerRotationYaw = true;
		//UE_LOG(LogTemp, Display, TEXT("Equipped!"));

		SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
		//CharacterState = ECharacterState::;

		AttachActorToBackpack(SecondaryWeapon);
		PlaySecondaryWeaponSound();
		SecondaryWeapon->SetOwner(this);
		SecondaryWeapon->ShowPickupWidget(false);
		SecondaryWeapon->EnableCustomDepth(false);

		//Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(GetController()) : Controller;
		//if (Controller)
		//{
		//	Controller->SetHUDWeaponType(SecondaryWeapon->GetWeaponType());
		//}


	}
}

void ACharacterBase::EquipWeapon(AWeapon* InWeapon)
{

	if (InWeapon == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;

	//UE_LOG(LogTemp, Display, TEXT("LocalRole : %s"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString());

	if (InWeapon->GetWeaponType() == EWeaponType::EWT_Flag)
	{

		Crouch();
		bHoldingTheFlag = true;
		InWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		InWeapon->SetOwner(this);
		AttachFlagToLeftHand(InWeapon);
		Flag = InWeapon;
		//GetCharacterMovement()->bOrientRotationToMovement = true;
		//bUseControllerRotationYaw = false;
	}
	else
	{
		if (EquippedWeapon != nullptr && SecondaryWeapon == nullptr) // Only Equipped One Weapon
		{
			SecondaryWeapon = InWeapon;
			EquipSecondaryFunc();
		}
		else // First Slot and Secondary Slot is all full
		{
			//UE_LOG(LogTemp, Display, TEXT("EquipWeapon"));
			DropEquippedWeapon();
			EquippedWeapon = InWeapon;
			EquipWeaponFunc();
		}

		if (GetCharacterMovement()) GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
	}

	AnimState = EAnimState::EAS_Equipped;
	bIsAiming = false;
}

void ACharacterBase::SwapWeapons()
{
	if (CombatState != ECombatState::ECS_Unoccupied) return;

	CombatState = ECombatState::ECS_SwappingWeapon;
	PlaySwapMontage();
	bFinishedSwapping = false;
	bIsFiring = false;
	//CombatState = ECombatState::ECS_SwappingWeapon;

	//AWeapon* TempWeapon = EquippedWeapon;
	//EquippedWeapon = SecondaryWeapon;
	//SecondaryWeapon = TempWeapon;


}

void ACharacterBase::OnRep_EquippedWeapon()
{
	EquipWeaponFunc();
}

void ACharacterBase::OnRep_SecondaryWeapon()
{
	EquipSecondaryFunc();

}

void ACharacterBase::FireButtonPressed(bool bPressed)
{
	if (EquippedWeapon == nullptr) return;
	bFireButtonPressed = bPressed;

	if (CanFire())
	{
		//UE_LOG(LogTemp, Display, TEXT("Canfire : %d Fire"), CanFire());

		bIsFiring = true;

		//FHitResult HitResult;
		//TraceUnderCrosshairs(HitResult);


		if (EquippedWeapon && bPressed)
		{

			CrosshairShootingFactor += 0.75;
			CameraShake(FireCameraShakeClass, true);

			AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
			if (!Gun) return;

			AddControllerPitchInput(Gun->GetRandomRecoilPitch());
			AddControllerYawInput(Gun->GetRandomRecoilYaw());

			switch (Gun->GetFireType())
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

void ACharacterBase::FireProjectileWeapon(bool bPressed)
{
	if (EquippedWeapon)
	{
		AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
		if (!Gun) return;

		HitTarget = Gun->bUseScatter ? Gun->TraceEndWithScatter(HitTarget) : HitTarget;

		if (!HasAuthority() && IsLocallyControlled()) // Autonomous
		{
			LocalFire(bPressed, HitTarget);
		}
		ServerFire(bPressed, HitTarget, Gun->GetFireDelay());

	}
}

void ACharacterBase::FireHitScanWeapon(bool bPressed)
{
	if (EquippedWeapon)
	{
		AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
		if (!Gun) return;

		HitTarget = Gun->bUseScatter ? Gun->TraceEndWithScatter(HitTarget) : HitTarget;

		ServerFire(bPressed, HitTarget, Gun->GetFireDelay());

		bool IsAutonomous = !HasAuthority() && IsLocallyControlled();
		if (IsAutonomous)
		{
			LocalFire(bPressed, HitTarget);
		}
	}
}

void ACharacterBase::FireShotgun(bool bPressed)
{
	AShotgun* Shotgun = Cast<AShotgun>(EquippedWeapon);
	if (Shotgun)
	{
		TArray<FVector_NetQuantize> HitTargets;
		Shotgun->ShotgunTraceEndWithScatter(HitTarget, HitTargets);
		if (!HasAuthority()) ShotgunLocalFire(HitTargets);
		ServerShotgunFire(HitTargets, Shotgun->GetFireDelay());
	}


}

void ACharacterBase::ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	AShotgun* Shotgun = Cast<AShotgun>(EquippedWeapon);
	if (Shotgun == nullptr) return;
	if (CombatState == ECombatState::ECS_Reloading || CombatState == ECombatState::ECS_Unoccupied)
	{
		//UE_LOG(LogTemp, Display, TEXT("ShotgunLocalFire"));
		bLocallyReloading = false;
		PlayFireMontage(bIsAiming);
		Shotgun->FireShotgun(TraceHitTargets);
		CombatState = ECombatState::ECS_Unoccupied;
	}

}

void ACharacterBase::StartFireTimer()
{
	//UE_LOG(LogTemp, Display, TEXT("StartFireTimer"));

	if (EquippedWeapon == nullptr) return;
	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
	if (!Gun) return;

	GetWorldTimerManager().SetTimer(FireTimer, this, &ThisClass::FireTimerFinished, Gun->GetFireDelay());
	//UE_LOG(LogTemp, Display, TEXT("FireTimer : %d"), FireTimer.IsValid());

}

void ACharacterBase::FireTimerFinished()
{
	if (EquippedWeapon == nullptr) return;
	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
	if (!Gun) return;
	//UE_LOG(LogTemp, Display, TEXT("FireTimerFinished"));

	//UE_LOG(LogTemp, Display, TEXT("FireTimer : %d"), FireTimer.IsValid());


	//bCanFire = true;

	ReloadEmptyWeapon();
	if (bFireButtonPressed && Gun->IsAutomatic())
	{
		FireButtonPressed(true);
	}
	bIsFiring = false;


	FireTimer.Invalidate();
}

void ACharacterBase::ServerFire_Implementation(bool bPressed, const FVector_NetQuantize& TraceHitTarget, float FireDelay)
{
	MulticastFire(bPressed, TraceHitTarget);
}

bool ACharacterBase::ServerFire_Validate(bool bPressed, const FVector_NetQuantize& TraceHitTarget, float FireDelay)
{
	if (EquippedWeapon)
	{
		AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
		if (!Gun) return false;

		bool bNearlyEqual = FMath::IsNearlyEqual(Gun->GetFireDelay(), FireDelay, 0.001f);
		return bNearlyEqual;
	}

	return true;
}

void ACharacterBase::MulticastFire_Implementation(bool bPressed, const FVector_NetQuantize& TraceHitTarget)
{
	//UE_LOG(LogTemp, Warning, TEXT("MulticastFire_Implementation"));
	//if (Character && IsLocallyControlled() && !HasAuthority())
	//{

	//	FText Str = UEnum::GetDisplayValueAsText(GetLocalRole());
	//	UE_LOG(LogTemp, Error, TEXT("Should Autonomous : %s"), *Str.ToString());
	//	return;
	//}
	//else if (Character && IsLocallyControlled() && HasAuthority())
	//{
	//	FText Str = UEnum::GetDisplayValueAsText(GetLocalRole());
	//	UE_LOG(LogTemp, Error, TEXT("Should Authority : %s"), *Str.ToString());
	//	return;
	//}
	//else if (Character && !IsLocallyControlled() && HasAuthority())
	//{
	//	FText Str = UEnum::GetDisplayValueAsText(GetLocalRole());
	//	UE_LOG(LogTemp, Error, TEXT("Not Local, Should Authority : %s"), *Str.ToString());
	//	return;
	//}
	//else if (Character && !IsLocallyControlled() && !HasAuthority())
	//{
	//	FText Str = UEnum::GetDisplayValueAsText(GetLocalRole());
	//	UE_LOG(LogTemp, Error, TEXT("Not Local, Should Proxy : %s"), *Str.ToString());
	//	return;
	//}

	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		return;
	}

	LocalFire(bPressed, TraceHitTarget);


}

void ACharacterBase::LocalFire(bool bPressed, const FVector_NetQuantize& TraceHitTarget)
{

	FString RoleStr = UEnum::GetDisplayValueAsText(GetLocalRole()).ToString();
	//UE_LOG(LogTemp, Display, TEXT("TraceHitTarget : %f, %f, %f, %s"), TraceHitTarget.X, TraceHitTarget.Y, TraceHitTarget.Z, *RoleStr);
	//UE_LOG(LogTemp, Warning, TEXT("%s : LocalFire"), *RoleStr);

	if (EquippedWeapon == nullptr) return;
	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
	if (!Gun) return;
	//if (Character && CombatState == ECombatState::ECS_Reloading && EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Shotgun)
	//{
	//	PlayFireMontage(bIsAiming);
	//	EquippedWeapon->Fire(TraceHitTarget);
	//	CombatState = ECombatState::ECS_Unoccupied;
	//	return;
	//}

	if (bPressed)
	{
		CombatState = ECombatState::ECS_Attacking;
		PlayFireMontage(bIsAiming);
		Gun->Fire(TraceHitTarget);
	}
}

void ACharacterBase::ServerShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	MulticastShotgunFire(TraceHitTargets);
}

bool ACharacterBase::ServerShotgunFire_Validate(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	if (EquippedWeapon)
	{
		AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
		if (!Gun) return false;

		bool bNearlyEqual = FMath::IsNearlyEqual(Gun->GetFireDelay(), FireDelay, 0.001f);
		return bNearlyEqual;
	}

	return true;
}

void ACharacterBase::MulticastShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	if (IsLocallyControlled() && !HasAuthority())
	{
		return;
	}

	ShotgunLocalFire(TraceHitTargets);
}

bool ACharacterBase::CanFire()
{
	//UE_LOG(LogTemp, Display, TEXT("bLocallyReloading : %d"), bLocallyReloading);
	if (EquippedWeapon == nullptr) return false;
	if (bLocallyReloading) return false;
	if (!IsAiming()) return false;
	if (!bFireButtonPressed) return false;
	if (!IsLocallyControlled()) return false;
	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
	if (!Gun) return false;

	if (Gun->IsEmpty()) Reload();

	return !Gun->IsEmpty() && (CombatState == ECombatState::ECS_Unoccupied || CombatState == ECombatState::ECS_HitReact || CombatState == ECombatState::ECS_Attacking);
}

void ACharacterBase::PickupAmmo(EWeaponType InWeaponType, int32 AmmoAmount)
{
	if (CarriedAmmoMap.Contains(InWeaponType))
	{
		CarriedAmmoMap[InWeaponType] = FMath::Clamp(CarriedAmmoMap[InWeaponType] + AmmoAmount, 0, INT32_MAX);

		UpdateCarriedAmmo();
	}

	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
	if (!Gun) return;

	if (EquippedWeapon && Gun->IsEmpty() && EquippedWeapon->GetWeaponType() == InWeaponType)
	{
		Reload();
	}
}

void ACharacterBase::InitializeCarriedAmmo()
{
	CarriedAmmoMap.Emplace(EWeaponType::EWT_AssaultRifle, StartingARAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_RocketLauncher, StartingRocketAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Pistol, StartingPistolAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SMG, StartingSMGAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Shotgun, StartingShotgunAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SniperRifle, StartingSniperAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_GrenadeLauncher, StartingGrenadeLauncherAmmo);

}

void ACharacterBase::OnRep_CarriedAmmo()
{
	UE_LOG(LogTemp, Display, TEXT("OnRep_CarriedAmmo"));

	bool bJumpToShoutgunEnd = CombatState == ECombatState::ECS_Reloading && EquippedWeapon != nullptr && EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Shotgun && CarriedAmmo == 0;
	if (bJumpToShoutgunEnd)
	{
		JumpToShotgunEnd();
	}

	UpdateCarriedAmmo();
}

void ACharacterBase::UpdateAmmoValues()
{
	if (EquippedWeapon == nullptr) return;
	if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Shotgun) return;
	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
	if (!Gun) return;

	int32 ReloadAmount = AmountToReload();

	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmoMap[EquippedWeapon->GetWeaponType()] -= ReloadAmount;
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
		//UE_LOG(LogTemp, Display, TEXT("CarriedAmmo : %d"), CarriedAmmo);
	}
	Gun->AddAmmo(ReloadAmount);


	if (IsLocallyControlled())
	{
		UE_LOG(LogTemp, Display, TEXT("%s: UpdateAmmoValues's CarriedAmmo : %d"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString(), CarriedAmmo);
	}
}

void ACharacterBase::UpdateShotgunAmmoValues()
{
	UE_LOG(LogTemp, Display, TEXT("UpdateShotgunAmmoValues : %d"), bReloadStopCheck);
	if (EquippedWeapon == nullptr) return;
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmoMap[EquippedWeapon->GetWeaponType()] -= 1;

		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
	if (!Gun) return;

	Gun->AddAmmo(1);

	if (Gun->IsFull() || CarriedAmmo == 0 || bReloadStopCheck)
	{
		// Jump to ShotgunEnd section
		JumpToShotgunEnd();
	}
}

void ACharacterBase::Reload()
{
	//UE_LOG(LogTemp, Display, TEXT("CombatState : %d"), CombatState);
	if ((int)CombatState > 1) return;
	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
	if (!Gun) return;

	if (CarriedAmmo > 0 && !Gun->IsFull() && !bLocallyReloading)
	{
		if (!HasAuthority() && IsLocallyControlled()) HandleReload();
		ServerReload();
		bLocallyReloading = true;
	}
}

void ACharacterBase::HandleReload()
{
	CombatState = ECombatState::ECS_Reloading;
	PlayReloadMontage();
}

void ACharacterBase::ShotgunShellReload()
{
	if (HasAuthority())
	{
		UpdateShotgunAmmoValues();
	}
}

void ACharacterBase::JumpToShotgunEnd_Implementation()
{

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && GetReloadMontage())
	{
		AnimInstance->Montage_JumpToSection(TEXT("ShotgunEnd"), GetReloadMontage());
	}
}

void ACharacterBase::ServerReload_Implementation()
{
	//UE_LOG(LogTemp, Display, TEXT("ServerReload_Implementation"));
	if (EquippedWeapon == nullptr) return;
	//UpdateAmmoValues();

	//if (!IsLocallyControlled()) HandleReload();
	MulticastReload();
}

void ACharacterBase::MulticastReload_Implementation()
{
	if (!HasAuthority() && IsLocallyControlled()) return;
	HandleReload();
}

void ACharacterBase::FinishReloading()
{

	//FString Str = UEnum::GetDisplayValueAsText(GetLocalRole()).ToString();
	//UE_LOG(LogTemp, Display, TEXT("FinishReloading %s : %d"), *Str, bLocallyReloading);



	bLocallyReloading = false;
	if (true)
	{
		CombatState = ECombatState::ECS_Unoccupied;
		UpdateAmmoValues();
	}
}

int32 ACharacterBase::AmountToReload()
{
	if (EquippedWeapon == nullptr) return 0;
	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
	if (!Gun) return 0;

	int32 RoomInMag = Gun->GetMagCapacity() - Gun->GetAmmo();

	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		int32 AmountCarried = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
		int32 Least = FMath::Min(RoomInMag, AmountCarried);
		return FMath::Clamp(RoomInMag, 0, Least); // 사실 클램프는 없어도 되는데 GetAmmo 가 MagCapacity 보다 클 경우를 대비한 거임. 즉 실수가 생겼을 시 예방책
	}
	return 0;
}

void ACharacterBase::ServerBolt_Implementation()
{
	MulticastBolt();
}

void ACharacterBase::MulticastBolt_Implementation()
{
	Bolt();
}

void ACharacterBase::Bolt()
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
	{
		AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
		if (!Gun) return;

		Gun->EjectCasing();
	}
}



void ACharacterBase::Melee()
{
	if (CombatState != ECombatState::ECS_Unoccupied)
	{
		if (CombatState != ECombatState::ECS_Reloading) return;
	}

	UE_LOG(LogTemp, Display, TEXT("Melee"));

	if (IsLocallyControlled())
	{
		if (!HasAuthority()) MeleeFunc();
		ServerMelee();
	}
}

void ACharacterBase::MeleeFunc()
{
	CombatState = ECombatState::ECS_MeleeAttack;
	if (MeleeMontage) PlayMontage(GetMesh()->GetAnimInstance(), MeleeMontage, TEXT("MeleeAttack"), -1);
}

void ACharacterBase::ServerMelee_Implementation()
{
	MulticastMelee();
}

void ACharacterBase::MulticastMelee_Implementation()
{
	if (!HasAuthority() && IsLocallyControlled()) return;
	MeleeFunc();
}

void ACharacterBase::SetMovementSpeed(float WalkSpeed, float CrouchSpeed)
{

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
}

//void ACharacterBase::OnRep_CombatState()
//{
//	switch (CombatState)
//	{
//	case ECombatState::ECS_Unoccupied:
//		if (bFireButtonPressed)
//		{
//			FireButtonPressed(true);
//		}
//		break;
//	case ECombatState::ECS_Reloading:
//		if (Character && !IsLocallyControlled()) HandleReload();
//		break;
//	case ECombatState::ECS_ThrowingGrenade:
//		//if (Character && !IsLocallyControlled()) // Simulate Proxy 만 재생하게 혹시 모르니 조건문 걸은거임
//		//{
//		//	PlayThrowGrenadeMontage();
//		//}
//		PlayThrowGrenadeMontage();
//		ShowAttachedGrenade(true);
//
//		break;
//	case ECombatState::ECS_SwappingWeapon:
//		if (Character && !IsLocallyControlled())
//		{
//			PlaySwapMontage();
//		}
//		break;
//	case ECombatState::ECS_MAX:
//		break;
//	default:
//		break;
//	}
//}

void ACharacterBase::ThrowGrenade()
{
	if (CombatState != ECombatState::ECS_Unoccupied || AnimState == EAnimState::EAS_UnEquipped) return;
	//CombatState = ECombatState::ECS_ThrowingGrenade;
	//if (Character)
	//{
	//	PlayThrowGrenadeMontage();
	//}

	//if (Character && !HasAuthority()) // 그냥 혹시모르니 조건확인용임
	//{
	//	ServerThrowGrenade();
	//}

	//if (Character && HasAuthority())
	//{
	//	Grenades = FMath::Clamp(Grenades - 1, 0, MaxGrenades);
	//	UpdateHUDGrenades();
	//}

	ServerThrowGrenade();
}

void ACharacterBase::ThrowGrenadeFinished()
{
	//UE_LOG(LogTemp, Warning, TEXT("ThrowGrenadeFinished"));
	CombatState = ECombatState::ECS_Unoccupied;
	AttachActorToRightHand(EquippedWeapon);
}

void ACharacterBase::DropEquippedWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Dropped();
	}
}

void ACharacterBase::AttachActorToRightHand(AActor* ActorToAttach)
{
	if (GetMesh() == nullptr || ActorToAttach == nullptr) return;
	//UE_LOG(LogTemp, Display, TEXT("AttachActorToRightHand"));
	const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(TEXT("hand_rSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(ActorToAttach, GetMesh()); // 자동으로 replicated 됨
	}
}

void ACharacterBase::AttachActorToBackpack(AActor* ActorToAttach)
{
	if (GetMesh() == nullptr || ActorToAttach == nullptr) return;
	//UE_LOG(LogTemp, Display, TEXT("AttachActorToRightHand"));
	const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(TEXT("BackpackSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(ActorToAttach, GetMesh()); // 자동으로 replicated 됨
	}
}

void ACharacterBase::AttachActorToLeftHand(AActor* ActorToAttach)
{
	if (GetMesh() == nullptr || ActorToAttach == nullptr || EquippedWeapon == nullptr) return;
	//UE_LOG(LogTemp, Display, TEXT("AttachActorToRightHand"));
	bool bUsePistolSocket = EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Pistol || EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SMG;
	FName SocketName = bUsePistolSocket ? TEXT("hand_lSocket_Pistol") : TEXT("hand_lSocket");

	if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
	{
		SocketName = TEXT("hand_lSocket_Rifle");
	}

	const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(SocketName);
	if (HandSocket)
	{
		HandSocket->AttachActor(ActorToAttach, GetMesh()); // 자동으로 replicated 됨
	}
}

void ACharacterBase::AttachFlagToLeftHand(AWeapon* InFlag)
{
	if (GetMesh() == nullptr || InFlag == nullptr) return;
	//UE_LOG(LogTemp, Display, TEXT("AttachActorToRightHand"));
	const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(TEXT("FlagSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(InFlag, GetMesh()); // 자동으로 replicated 됨
	}
}

void ACharacterBase::ServerThrowGrenade_Implementation()
{
	if (Grenades == 0) return;
	//UE_LOG(LogTemp, Display, TEXT("ServerThrowGrenade_Implementation"));
	CombatState = ECombatState::ECS_ThrowingGrenade;
	ShowAttachedGrenade(true);
	AttachActorToLeftHand(EquippedWeapon);
	PlayThrowGrenadeMontage();

	Grenades = FMath::Clamp(Grenades - 1, 0, MaxGrenades);

}

void ACharacterBase::UpdateCarriedAmmo()
{
	if (EquippedWeapon == nullptr) return;
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}
}

void ACharacterBase::PlayEquipWeaponSound()
{
	if (EquippedWeapon && EquippedWeapon->GetTakeSound())
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquippedWeapon->GetTakeSound(), GetActorLocation());
	}
}

void ACharacterBase::PlaySecondaryWeaponSound()
{
	if (SecondaryWeapon && SecondaryWeapon->GetTakeSound())
	{
		UGameplayStatics::PlaySoundAtLocation(this, SecondaryWeapon->GetTakeSound(), GetActorLocation());
	}
}

void ACharacterBase::ReloadEmptyWeapon()
{
	if (EquippedWeapon && Cast<AWeapon_Gun>(EquippedWeapon)->IsEmpty())
	{
		Reload();
	}
}

void ACharacterBase::ShowAttachedGrenade(bool bShowGrenade)
{
	if (GetAttachedGrenade())
	{
		GetAttachedGrenade()->SetVisibility(bShowGrenade);
	}
}

void ACharacterBase::LaunchGrenade()
{
	if (IsLocallyControlled())
	{
		//UE_LOG(LogTemp, Display, TEXT("LaunchGrenade"));
		ServerLaunchGrenade(HitTarget);

	}
}


void ACharacterBase::OnRep_Grenades()
{


}

void ACharacterBase::ServerLaunchGrenade_Implementation(const FVector_NetQuantize& Target)
{
	if (GrenadeClass && GetAttachedGrenade())
	{
		//UE_LOG(LogTemp, Display, TEXT("ServerLaunchGrenade_Implementation"));
		const FVector StartingLocation = GetAttachedGrenade()->GetComponentLocation();
		FVector ToTarget = Target - StartingLocation;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;
		UWorld* World = GetWorld();
		if (World)
		{
			DrawDebugLine(World, StartingLocation, Target, FColor::Magenta, false, 5.f);
			AProjectileGrenade* Grenade = World->SpawnActor<AProjectileGrenade>(GrenadeClass, StartingLocation, ToTarget.Rotation(), SpawnParams);

		}
	}
}

void ACharacterBase::DisableAttachedGrenade()
{
	ShowAttachedGrenade(false);
	LaunchGrenade();
}

void ACharacterBase::OnRep_HoldingTheFlag()
{
	if (bHoldingTheFlag && IsLocallyControlled())
	{

		//UE_LOG(LogTemp, Display, TEXT("OnRep_HoldingTheFlag : %s"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString());
		Crouch();
	}
}

void ACharacterBase::Slide()
{

	if (GetVelocity().Length() > 500.f && CanJump())
	{
		CombatState = ECombatState::ECS_Sliding;
		if (!HasAuthority()) SlideFunc();
		ServerSlide();
	}
}

void ACharacterBase::SlideFunc()
{
	PlaySlideMontage();
	Crouch();
}

void ACharacterBase::ServerSlide_Implementation()
{
	SlideFunc();
	bIsSliding = true;
}

void ACharacterBase::OnRep_Sliding()
{
	if (bIsSliding)
	{
		SlideFunc();
	}
}

void ACharacterBase::Dodge(FName InSectionName)
{
	//UE_LOG(LogTemp, Display, TEXT("Dodge : CombatState : %s"), *UEnum::GetDisplayValueAsText(CombatState).ToString());
	if (CombatState != ECombatState::ECS_Unoccupied || GetCharacterMovement()->IsFalling())
	{
		if (CombatState != ECombatState::ECS_Reloading)
		{
			if (CombatState != ECombatState::ECS_Attacking) return;
		}
	}

	if (!CheckSpUnder(AttributeComponent->GetCurrentSp(), AttributeComponent->GetDodgeCost())) return;

	//CombatState = ECombatState::ECS_Dodging;

	//bIsDodge = true;

	if (IsLocallyControlled())
	{
		if (!HasAuthority()) DodgeFunc(InSectionName);
		ServerDodge(InSectionName);
	}
}

void ACharacterBase::DodgeFunc(FName InSectionName)
{
	CombatState = ECombatState::ECS_Dodging;
	Crouch();
	PlayDodgeMontage(InSectionName);
}

void ACharacterBase::ServerDodge_Implementation(FName InSectionName)
{
	DodgeFunc(InSectionName);
	KeySectionName = InSectionName;
}

void ACharacterBase::OnRep_Dodge()
{
	if (bIsDodge)
	{
		DodgeFunc(KeySectionName);
	}
}

void ACharacterBase::VaultOrMantle()
{
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	//UE_LOG(LogTemp, Display, TEXT("VaultOrMantle"));


	if (IsLocallyControlled())
	{
		if (!HasAuthority())
		{
			bCanMorV = VaultOrMantleFunc();
		}
		ServerVaultMantle();
	}

}

bool ACharacterBase::VaultOrMantleFunc()
{
	if (MantleVaultComponent->GetInitialObjectLocation_C())
	{
		//if (!HasAuthority()) UE_LOG(LogTemp, Error, TEXT("VaultOrMantleFunc"));
		CombatState = ECombatState::ECS_VaultOrMantle;
		bUseControllerRotationYaw = false;
		return true;
	}

	return false;
}

void ACharacterBase::ServerVaultMantle_Implementation()
{
	bCanMorV = VaultOrMantleFunc();
	bIsMorV = true;
}

void ACharacterBase::OnRep_VaultOrMantle()
{
	VaultOrMantleFunc();
}

void ACharacterBase::SetAimingFunc(bool InbIsAiming)
{
	//FString Str = UEnum::GetDisplayValueAsText(GetLocalRole()).ToString();
	//UE_LOG(LogTemp, Display, TEXT("ServerSetAiming_Implementation : %s"), *Str);

	bIsAiming = InbIsAiming;

	if (bIsAiming)
	{
		//GetCharacterMovement()->bOrientRotationToMovement = false;
		//bUseControllerRotationYaw = true;

		if (BuffComponent && BuffComponent->SpeedBuff && GetBuffComponent()->bIsBuffingSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed + BuffComponent->SpeedBuff->GetBaseSpeedBuff();
			GetCharacterMovement()->MaxWalkSpeedCrouched = AimCrouchSpeed + BuffComponent->SpeedBuff->GetCrouchSpeedBuff();
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
			GetCharacterMovement()->MaxWalkSpeedCrouched = AimCrouchSpeed;
		}


		//SetMovementSpeed(AimWalkSpeed, AimCrouchSpeed);
		AnimState = EAnimState::EAS_Combat;
	}
	else
	{
		//GetCharacterMovement()->bOrientRotationToMovement = true;
		//bUseControllerRotationYaw = false;

		if (BuffComponent && BuffComponent->SpeedBuff && GetBuffComponent()->bIsBuffingSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed + BuffComponent->SpeedBuff->GetBaseSpeedBuff();
			GetCharacterMovement()->MaxWalkSpeedCrouched = BaseCrouchSpeed + BuffComponent->SpeedBuff->GetCrouchSpeedBuff();
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
			GetCharacterMovement()->MaxWalkSpeedCrouched = BaseCrouchSpeed;
		}


		//SetMovementSpeed(BaseWalkSpeed, BaseCrouchSpeed);
		AnimState = EAnimState::EAS_Equipped;
	}
}

void ACharacterBase::SetAiming(bool InbIsAiming)
{
	//bIsAiming = InbIsAiming;
	if (EquippedWeapon == nullptr) return;

	if (IsLocallyControlled()) bAimButtonPressed = InbIsAiming;

	SetAimingFunc(InbIsAiming);

	ServerSetAiming(InbIsAiming);


}

void ACharacterBase::OnRep_Aiming_Finished()
{
	if (IsLocallyControlled())
	{
		bIsAiming = bAimButtonPressed;
		AnimState = EAnimState::EAS_Equipped;
	}
}

void ACharacterBase::ServerSetAiming_Implementation(bool InbIsAiming)
{
	if (!HasAuthority()) return;

	SetAimingFunc(InbIsAiming);
}

bool ACharacterBase::ServerSetAiming_Validate(bool InbIsAiming)
{
	//UE_LOG(LogTemp, Display, TEXT("Validation"));
	return true;
}

void ACharacterBase::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();

	//if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy)
	//{
	//	SimProxiesTurn();
	//}
	SimProxiesTurn();
	TimeSinceLastMovementReplication = 0.f;
}

void ACharacterBase::SimProxiesTurn()
{
	// Turning about Simulated Proxies

	if (EquippedWeapon == nullptr) return;

	bRotateRootBone = false;

	// Turning is only work if speed is zero
	float Speed = CalculateSpeed();
	if (Speed > 0.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}


	//AO_PitchMappingForClient();
	ProxyRotationLastFrame = ProxyRotationCurrentFrame;
	ProxyRotationCurrentFrame = GetActorRotation();

	// 두 Rotator 의 차이를 정규화함. 즉, 기존 Rotator 에서 신규 Rotator 사이의 델타값
	ProxyYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotationCurrentFrame, ProxyRotationLastFrame).Yaw;

	if (FMath::Abs(ProxyYawDelta) > TurnThreshold) // 델타값, 즉 두 로테이터 간의 차이에 유의미한 변화가 있을 시
	{
		if (ProxyYawDelta > TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_TurningRight;
		}
		else if (ProxyYawDelta < -TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_TurningLeft;
		}
		else
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		}
	}
	else
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}
}

void ACharacterBase::TurnInPlace(float DeltaTime)
{
	float TurningThreshold = 45.f;

	if (AO_Yaw > TurningThreshold) // Right Turning Threshold
	{
		TurningInPlace = ETurningInPlace::ETIP_TurningRight;
	}
	else if (AO_Yaw < -TurningThreshold) // Left Turning Threshold
	{
		TurningInPlace = ETurningInPlace::ETIP_TurningLeft;
	}

	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 6.f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}



// This function must be executed when character is idling. not moving.
void ACharacterBase::AimOffset(float DeltaTime)
{
	if (EquippedWeapon == nullptr) return;

	float Speed = CalculateSpeed();
	bool bIsInAir = GetCharacterMovement()->IsFalling();


	if (AnimState != EAnimState::EAS_UnEquipped)
	{
		// Manage Left and Right
		if (Speed <= 0.f && !bIsInAir)
		{


			bRotateRootBone = true;
			FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
			FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);

			//UE_LOG(LogTemp, Display, TEXT("CurrentAimRotation : %f"), CurrentAimRotation.Yaw);
			//UE_LOG(LogTemp, Display, TEXT("StartingAimRotation : %f"), StartingAimRotation.Yaw);
			//UE_LOG(LogTemp, Display, TEXT("DeltaAimRotation : %f"), DeltaAimRotation.Yaw);

			AO_Yaw = DeltaAimRotation.Yaw;
			//if (IsAiming()) bUseControllerRotationYaw = false;

			if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
			{
				InterpAO_Yaw = AO_Yaw;
			}

			bUseControllerRotationYaw = true;
			TurnInPlace(DeltaTime);
		}

		if (Speed > 0.f || bIsInAir)
		{
			if (AnimState == EAnimState::EAS_Combat)
			{
				bRotateRootBone = false;
			}
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
			AO_Yaw = 0.f;
			//if (IsAiming()) bUseControllerRotationYaw = true;
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		}
		//
	}

	AO_PitchMappingForClient();
}

float ACharacterBase::CalculateSpeed()
{
	return GetVelocity().Size2D();
}

void ACharacterBase::Jump()
{
	if (bDisableGameplay) return;

	if (!GetCharacterMovement()->IsFalling())
	{
		VaultOrMantle();
	}

	if (bHoldingTheFlag) return;

	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Super::Jump();
	}
}

void ACharacterBase::SpawnMagic(FName SocketToSpawn)
{
	// This Class's Derived Class doesn't call Super

	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetMesh()->GetSocketByName(SocketToSpawn);
	UWorld* World = GetWorld();

	if (MuzzleFlashSocket && World)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetMesh());
		FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = InstigatorPawn;

		AProjectile* SpawnedProjectile = nullptr;

		if (HasAuthority() && ProjectileMagicClass)
		{
			SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileMagicClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
			//SpawnedProjectile = World->SpawnActorDeferred<AProjectile>(ProjectileMagic, SocketTransform);

			SpawnedProjectile->bUseServerSideRewind = false;
			SpawnedProjectile->Damage = ProjectileMagicDamage;

		}

	}
}

void ACharacterBase::SpawnCharacter(FVector SpawnLocation)
{
	if (CharacterSpawnEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CharacterSpawnEffect, SpawnLocation);
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(GetActorRotation().Quaternion());
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());

	ACharacterBase* SpawnedActor = GetWorld()->SpawnActor<ACharacterBase>(CharacterToSpawn, SpawnTransform, SpawnParams);
	if (SpawnedActor)
	{
		SpawnedActor->SpawnDefaultController();
	}
}

void ACharacterBase::DashStart(float Output)
{
	//UE_LOG(LogTemp, Display, TEXT("DashStart"));

	if (KeySectionName == FORWARD)
	{
		AddMovementInput(GetCapsuleComponent()->GetForwardVector(), 1.0f, false);
	}
	else if (KeySectionName == BACKWARD)
	{
		AddMovementInput(-GetCapsuleComponent()->GetForwardVector(), 1.0f, false);
	}
	else if (KeySectionName == LEFT)
	{
		AddMovementInput(-GetCapsuleComponent()->GetRightVector(), 1.0f, false);
	}
	else if (KeySectionName == RIGHT)
	{
		AddMovementInput(GetCapsuleComponent()->GetRightVector(), 1.0f, false);
	}
	else
	{
		if (IsLocallyControlled()) UE_LOG(LogTemp, Error, TEXT("KeySectionName Mismatch!!!"));
	}

}

void ACharacterBase::DashEnd()
{
	//UE_LOG(LogTemp, Display, TEXT("DashEnd"));
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxAcceleration = 2048.0f;
	//bUseControllerRotationYaw = true;
}

void ACharacterBase::InitDashCurve()
{
	const ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Script/Engine.CurveFloat'/Game/A_Blaster/Curves/DashCurve.DashCurve'"));

	if (Curve.Succeeded())
	{
		DashCurve = Curve.Object;
	}

	if (DashCurve != nullptr)
	{
		DashTimeLineUpdateDelegate.BindUFunction(this, FName("DashStart"));
		DashTimeLineFinishDelegate.BindUFunction(this, FName("DashEnd"));

		DashTimeline.AddInterpFloat(DashCurve, DashTimeLineUpdateDelegate);
		DashTimeline.SetTimelineFinishedFunc(DashTimeLineFinishDelegate);

		float Min = 0.0f;
		float Max = 0.25f;
		DashCurve->GetTimeRange(Min, Max);
		DashTimeline.SetTimelineLength(Max);
	}
}

void ACharacterBase::ServerDash_Implementation(FName InSectionName)
{
	MulticastDash(InSectionName);
}

void ACharacterBase::MulticastDash_Implementation(FName InSectionName)
{
	if (!HasAuthority() && IsLocallyControlled()) return;
	DashFunc(InSectionName);
}

void ACharacterBase::DashFunc(FName InSectionName)
{
	CombatState = ECombatState::ECS_Dash;

	PlayMontage(GetMesh()->GetAnimInstance(), DashMontage, InSectionName, -1);

	GetCharacterMovement()->MaxWalkSpeed = 2000.0f;
	GetCharacterMovement()->MaxAcceleration = 20000.0f;
	bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bUseControllerDesiredRotation = false;

	DashTimeline.PlayFromStart();
}

void ACharacterBase::Dash(FName InSectionName)
{
	//UE_LOG(LogTemp, Display, TEXT("CombatState : %s"), *UEnum::GetDisplayValueAsText(CombatState).ToString());

	if (CombatState != ECombatState::ECS_Unoccupied)
	{
		if (CombatState != ECombatState::ECS_Reloading)
		{
			if (CombatState != ECombatState::ECS_Attacking) return;
		}
	}

	if (GetCharacterMovement()->IsFalling()) return;


	if (!CheckSpUnder(AttributeComponent->GetCurrentSp(), AttributeComponent->GetDashCost())) return;


	if (IsLocallyControlled())
	{
		if (!HasAuthority()) DashFunc(InSectionName);
		ServerDash(InSectionName);
	}

}

bool ACharacterBase::CheckSpUnder(float InCurrentSp, float InCost)
{
	if (InCurrentSp < InCost) return false;

	float ResultSp = FMath::Clamp(InCurrentSp - InCost, 0.f, AttributeComponent->GetMaxSp());

	if (ResultSp <= 0.f)
	{
		AttributeComponent->SetCurrentSp(0.f);
	}
	else
	{
		AttributeComponent->SetCurrentSp(ResultSp);
	}
	AttributeComponent->OnSpChanged.Broadcast(AttributeComponent->GetCurrentSp(), AttributeComponent->GetMaxSp());

	return true;

}

void ACharacterBase::CameraShake(const TSubclassOf<UCameraShakeBase>& CameraShake, bool bTrue)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC == nullptr) return;

	if (bTrue)
	{
		PC->ClientStartCameraShake(CameraShake);
	}
	else
	{
		PC->ClientStopCameraShake(CameraShake);
	}
}

void ACharacterBase::AO_PitchMappingForClient()
{
	// 서버에서 아래 방향으로의 Pitch 는 음수로 시작하는 반면, 클라에서의 아래 방향으로의 Pitch 는 360도에서부터 내려가는 방식으로 시작하기 때문에, 이를 서버와 같은 방식으로 매핑해줄 필요가 있다.
	// Server's pitch going to negative value when scrolling down, but client's pitch starts 360 degree. so we need to mapping client's value.

	AO_Pitch = GetBaseAimRotation().Pitch;
	//UE_LOG(LogTemp, Display, TEXT("Pitch : %f"), AO_Pitch);
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		FVector2d InRange(270.f, 360.f);
		FVector2d OutRange(-90.f, 0.f);

		// 서버와 똑같게 클라에서 360 -> 270 으로 변화하는 거를 -90 -> 0 으로 변화하게 매핑작업.
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

void ACharacterBase::RotateInPlace(float DeltaTime)
{
	//if (IsLocallyControlled()) UE_LOG(LogTemp, Display, TEXT("Role : %s, CombatState : %s"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString(), *UEnum::GetDisplayValueAsText(CombatState).ToString());
	if (CombatState == ECombatState::ECS_VaultOrMantle) return;
	if (CombatState == ECombatState::ECS_Dash) return;

	if (GetController() == nullptr) return;

	//UE_LOG(LogTemp, Display, TEXT("RotateInPlace"));

	FRotator Rotator = GetController()->GetControlRotation();
	Rotator = FRotator(0.f, Rotator.Yaw, 0.f);
	FVector ForwardVec = FRotationMatrix(Rotator).GetUnitAxis(EAxis::X);

	//UE_LOG(LogTemp, Display, TEXT("GetActorForwardVector : %f, %f, %f"), GetActorForwardVector().X, GetActorForwardVector().Y, GetActorForwardVector().Z);
	//UE_LOG(LogTemp, Display, TEXT("ForwardVec : %f, %f, %f"), ForwardVec.X, ForwardVec.Y, ForwardVec.Z);


	if (!GetActorForwardVector().GetSafeNormal2D().Equals(ForwardVec.GetSafeNormal2D())) return;

	if (bHoldingTheFlag)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}

	if (EquippedWeapon)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
	}

	if (bDisableGameplay)
	{
		bUseControllerRotationYaw = false;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}

	if (GetLocalRole() > ENetRole::ROLE_SimulatedProxy && IsLocallyControlled())
	{
		AimOffset(DeltaTime);
	}
	else // only for the Simulated Proxies, Simulated Proxies's turning rule. 떨림 방지를 위해.
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25f)
		{
			OnRep_ReplicatedMovement();
		}
		AO_PitchMappingForClient();
	}
}

void ACharacterBase::ServerCheckParry_Implementation(AActor* OtherActor)
{
	//UE_LOG(LogTemp, Display, TEXT("ServerCheckParry_Implementation : %s"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString());
	MulticastCheckParry(OtherActor);
}

void ACharacterBase::MulticastCheckParry_Implementation(AActor* OtherActor)
{
	//UE_LOG(LogTemp, Display, TEXT("MulticastCheckParry_Implementation : %s"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString());

	//if (!HasAuthority() && IsLocallyControlled()) return;
	ENetRole Role1 = GetLocalRole();


	AEnemy* PO = Cast<AEnemy>(OtherActor);
	if (PO)
	{
		bParryResultCheck = PO->CheckParryFunc(this);
		if (bParryResultCheck)
		{
			PO->AttributeComponent->OnParryGaugeAnim.Broadcast(false);
			PO->AttributeComponent->bIsParryGaugeAnimPlaying = false;
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ParryOpponent is null"));
	}
}

//bool ACharacterBase::CheckParryFunc(AActor* OtherActor, AAIController* InAIController, float ShortStunTime, float LongStunTime)
//{
//	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(InAIController);
//	if (EnemyAIController == nullptr) return false;
//	A1 = EnemyAIController;
//
//	AEnemy* OtherCharacterBase = Cast<AEnemy>(OtherActor);
//	if (OtherCharacterBase == nullptr) return false;
//
//
//	if (GetLocalRole() > 1)
//	{
//
//	}
//
//
//	if (GetActorForwardVector().Dot(OtherActor->GetActorForwardVector()) < -0.5f) // 전방 기준 45도 이내일때만 발동
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Blocking"));
//		if (OtherCharacterBase->ParriedMontage) PlayMontage(GetMesh()->GetAnimInstance(), OtherCharacterBase->ParriedMontage, TEXT("Parried"), -1);
//		OtherCharacterBase->DisableHitCapsulesCollision();
//		OtherCharacterBase->CombatState = ECombatState::ECS_Parried;
//		//if (EnemyAIController) EnemyAIController->GetBlackboardComponent()->SetValueAsBool(IS_PARRIED, true);
//		if (EnemyAIController) EnemyAIController->StopAI();
//
//		if (ParryEffect)
//		{
//			FTransform ParryEffectTransform(GetActorRotation().Quaternion(), GetActorLocation());
//			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParryEffect, ParryEffectTransform);
//		}
//
//		float StunTime = ShortStunTime;
//
//		if (OtherCharacterBase->GetAttributeComponent()->GetCurrentParryGauge() >= OtherCharacterBase->GetAttributeComponent()->GetMaxParryGauge() - 100.f)
//		{
//			//UE_LOG(LogTemp, Display, TEXT("HardParried"));
//			StunTime = LongStunTime;
//			OtherCharacterBase->GetAttributeComponent()->SetCurrentParryGauge(0.f);
//			OtherCharacterBase->GetAttributeComponent()->OnParryGaugeChanged.Broadcast(0.f, OtherCharacterBase->GetAttributeComponent()->GetMaxParryGauge());
//		}
//
//		FTimerHandle AITimer;
//		GetWorldTimerManager().SetTimer(AITimer, FTimerDelegate::CreateLambda([&]()
//			{
//				bParryResultCheck = 0;
//				if (A1) A1->RunAI();
//			}), StunTime, false);
//
//
//		return true;
//	}
//
//
//
//	return false;
//}

//bool ACharacterBase::CheckParry(AActor* OtherActor)
//{
//	UE_LOG(LogTemp, Display, TEXT("CheckParry : %s"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString());
//
//	//if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
//	{
//		bParryResultCheck = CheckParryFunc(OtherActor);
//	}
//	ServerCheckParry(OtherActor);
//
//	return bParryResultCheck;
//}

void ACharacterBase::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	//if (OverlappingWeapon)
	//{
	//	OverlappingWeapon->showpi
	//}
	//UE_LOG(LogTemp, Display, TEXT("OnRep_OverlappingWeapon : %x"), OverlappingWeapon.Get());
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}

	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}