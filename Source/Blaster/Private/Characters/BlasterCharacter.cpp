// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BlasterCharacter.h"

// Components
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BuffComponent.h"
#include "Components/BoxComponent.h"
#include "Components/LagCompensationComponent.h"
#include "Components/MantleVaultComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/SkillComponent.h"

// EnhancedInput
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Widget
#include "HUD/OverheadWidget.h"
#include "HUD/CharacterOverlay.h"
#include "HUD/HpBarWidget.h"
#include "HUD/HpBarWidgetComponent.h"
#include "HUD/MyWidgetComponent.h"

// Network
#include "Net/UnrealNetwork.h"

// Weapon
#include "Item/Pickable/Weapon/Weapon_Gun.h"

// Math
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Animations
#include "Animations/BlasterAnimInstance.h"

// PlayerController
#include "PlayerController/BlasterPlayerController.h"

#include "GameMode/BlasterGameMode.h"
#include "GameState/BlasterGameState.h"

// PlayerState
#include "PlayerState/BlasterPlayerState.h"

// PlayerStart
#include "PlayerStart/TeamPlayerStart.h"

// sound
#include "Sound/SoundCue.h"

// Niagara
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

//#include "TimerManager.h"
#include "Item/Pickable/Weapon/WeaponTypes.h"

#include "PhysicsEngine/PhysicsAsset.h"
#include "Blaster/Blaster.h"

// Sets default values
ABlasterCharacter::ABlasterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.TickInterval = 0.5f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	BuffComponent = CreateDefaultSubobject<UBuffComponent>(TEXT("BuffComponent"));
	DissolveTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComponent"));
	AttachedGrenade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttachedGrenade"));

	InitializeDefaults();

	//UE_LOG(LogTemp, Display, TEXT("BlasterCharacter Constructor"));
}

// Called every frame
void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//GetMesh()->SetComponentTickInterval(0.001);
	GetCharacterMovement()->SetComponentTickInterval(0.001f);

	//UE_LOG(LogTemp, Display, TEXT("Team : %s"), *UEnum::GetDisplayValueAsText(Team).ToString());


	//if (IsLocallyControlled())
	//UE_LOG(LogTemp, Display, TEXT("CombatState : %s"), *UEnum::GetDisplayValueAsText(CombatState).ToString());

	HideCameraIfCharacterClose();


	//BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	//if (BlasterGameMode)
	//{
	//	FName MatchState = BlasterGameMode->GetMatchState();
	//	if (MatchState == MatchState::Cooldown)
	//	{
	//		bDisableGameplay = bDisableGameplay == false ? true : bDisableGameplay;
	//	}
	//}

	//BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController;


	//UE_LOG(LogTemp, Display, TEXT("bDisableGameplay : %d"), bDisableGameplay);
	//if (HasAuthority() && IsLocallyControlled())
	//{
	//UE_LOG(LogTemp, Display, TEXT("MaxWalkSpeed : %f"), GetCharacterMovement()->MaxWalkSpeed);

	//}


	if (IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;
		SetHUDCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);


		if (CombatState != ECombatState::ECS_Attacking)
		{
			bIsFirebuttonPressed = false;
		}
	}

	//if (HasAuthority() && IsLocallyControlled())
	//UE_LOG(LogTemp, Display, TEXT("bisfirebuttobpressed : %d"), bIsFirebuttonPressed);

	//UE_LOG(LogTemp, Display, TEXT("CrosshairColor : %s"), *HUDPackage.CrosshairsColor.ToString());

	//HUDPackage.CrosshairsColor = FLinearColor::Red;

	//LagCompensation->Draw(bDrawPhysAssets);

	//if (HasAuthority())
	//UE_LOG(LogTemp, Display, TEXT("OverlappingItem : %x"), OverlappingWeapon.Get());

	//if (HasAuthority() && IsLocallyControlled())
	//{
		//UE_LOG(LogTemp, Display, TEXT("CombatState : %s"), *UEnum::GetDisplayValueAsText(CombatState).ToString());
	//}


}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ABlasterCharacter, OverlappingWeapon);
	//DOREPLIFETIME(ABlasterCharacter, CurrentHealth);
	//DOREPLIFETIME_CONDITION(ABlasterCharacter, bDisableGameplay, COND_OwnerOnly);
	//DOREPLIFETIME(ABlasterCharacter, bDisableGameplay);
	//DOREPLIFETIME(ABlasterCharacter, CurrentShield);
	DOREPLIFETIME(ABlasterCharacter, KeyType);


}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//if ()
	//{
	//	->Character = this;
	//}

	//if (BuffComponent)
	//{
	//	BuffComponent->Character = this;
	//	BuffComponent->SetInitialSpeeds(GetCharacterMovement()->MaxWalkSpeed, GetCharacterMovement()->MaxWalkSpeedCrouched);
	//	BuffComponent->SetInitialJumpVelocity(GetCharacterMovement()->JumpZVelocity);
	//}

	//if (LagCompensation)
	//{
	//	LagCompensation->Character = this;
	//	if (Controller)
	//	{
	//		LagCompensation->Controller = Cast<ABlasterPlayerController>(Controller);
	//	}
	//}
}

// Called when the game starts or when spawned
void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("ABlasterCharacter::BeginPlay()"));

	SetActorTickInterval(0.01f);

	SpawnDefaultWeapon();

	//FTimerHandle H;
	//GetWorldTimerManager().SetTimer(H, FTimerDelegate::CreateLambda([&]()
	//	{
	//		if (OverheadWidget && GetPlayerState())
	//		{
	//			OverheadWidget->ShowPlayerName(GetPlayerState());
	//		}
	//	}), 0.2f, false);



	//UE_LOG(LogTemp, Display, TEXT("BeginPlay"));
	//BlasterPlayerController = BlasterPlayerController.Get() == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController.Get();
	//if (BlasterPlayerController)
	//{
	//	ULocalPlayer* LocalPlayer = BlasterPlayerController->GetLocalPlayer();

	//	if (LocalPlayer)
	//	{
	//		UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	//		if (Subsystem)
	//		{
	//			Subsystem->AddMappingContext(IMC_Shoulder, 0);
	//		}
	//	}
	//}

	if (const ULocalPlayer* Player = (GEngine && GetWorld()) ? GEngine->GetFirstGamePlayer(GetWorld()) : nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player);
		if (IMC_Shoulder)
		{
			Subsystem->AddMappingContext(IMC_Shoulder, 0);
		}
	}


	//UE_LOG(LogTemp, Display, TEXT("CurrentHealth : %f, MaxHealth : %f"), CurrentHealth, MaxHealth);

	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController;

	if (BlasterPlayerController)
	{
		BlasterPlayerController->UpdateHUDHealth();
		BlasterPlayerController->UpdateHUDShield();
		BlasterPlayerController->UpdateHUDAmmo();

		BlasterPlayerController->SetHUDHealth(AttributeComponent->GetMaxHp(), AttributeComponent->GetMaxHp());
		BlasterPlayerController->SetHUDShield(AttributeComponent->GetMaxShield(), AttributeComponent->GetMaxShield());

		BlasterPlayerController->PollInit(this);
	}

	AttributeComponent->OnHpChanged.Broadcast(AttributeComponent->GetCurrentHp(), AttributeComponent->GetMaxHp());


	if (AttachedGrenade)
	{
		AttachedGrenade->SetVisibility(false);
	}

	//LagCompensation->HitCapsuleConstruction(GetMesh());

	if (GetCameraComponent())
	{
		DefaultFOV = GetCameraComponent()->FieldOfView;
		CurrentFOV = DefaultFOV;
	}

	if (HasAuthority())
	{
		InitializeCarriedAmmo();
	}

	Grenades = MaxGrenades;


	PollInit();

}

void ABlasterCharacter::Destroyed()
{
	Super::Destroyed();

	BlasterGameMode = BlasterGameMode == nullptr ? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
	bool bMatchNotInProgress = BlasterGameMode && BlasterGameMode->GetMatchState() != MatchState::InProgress;

	if (EquippedWeapon && bMatchNotInProgress)
	{
		EquippedWeapon->Destroy();
	}
}

//bool ABlasterCharacter::IsSprint() const
//{
//
//	if ()
//	{
//		->bIsSprint;
//	}
//
//	return false;
//}

//ECombatState ABlasterCharacter::GetCombatState() const
//{
//	return  == nullptr ? ECombatState::ECS_MAX : ->CombatState;
//}

//AWeapon_Gun* ABlasterCharacter::GetEquippedWeapon() const
//{
//	return  == nullptr ? nullptr : ->EquippedWeapon;
//}
//
//EAnimState ABlasterCharacter::GetAnimState() const
//{
//	return  == nullptr ? EAnimState::EAS_MAX : ->AnimState;
//}
//
//FVector ABlasterCharacter::GetHitTarget() const
//{
//	return  == nullptr ? FVector() : ->HitTarget;
//}
//
//ETeam ABlasterCharacter::GetTeam()
//{
//	BlasterPlayerState = BlasterPlayerState == nullptr ? GetPlayerState<ABlasterPlayerState>() : BlasterPlayerState;
//	if (BlasterPlayerState == nullptr) return ETeam::ET_NoTeam;
//
//	return BlasterPlayerState->GetTeam();
//}
//
//void ABlasterCharacter::SetHoldingTheFlag(bool bHolding)
//{
//	if ( == nullptr) return;
//
//	->bHoldingTheFlag = bHolding;
//}
//
//bool ABlasterCharacter::IsWeaponEquipped()
//{
//	return ( && ->EquippedWeapon);
//}
//
//bool ABlasterCharacter::IsAiming()
//{
//	return ( && ->bIsAiming);
//}
//
//bool ABlasterCharacter::IsLocallyReloading()
//{
//	if ( == nullptr) return false;
//	return ->bLocallyReloading;
//}
//
//bool ABlasterCharacter::IsHoldingTheFlag() const
//{
//	if ( == nullptr) return false;
//	return ->bHoldingTheFlag;
//}

//void ABlasterCharacter::UpdateHUDHealth()
//{
//	//UE_LOG(LogTemp, Display, TEXT("UpdateHUDHealth"));
//	BlasterPlayerController = BlasterPlayerController.Get() == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController.Get();
//
//	// 아래걸로 설정하면 안됨. GetFirstPlayerController() => 서버의 컨트롤러를 말하는거임.
//	//BlasterPlayerController = BlasterPlayerController.Get() == nullptr ? Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController()) : BlasterPlayerController.Get();
//
//	if (BlasterPlayerController)
//	{
//		BlasterPlayerController->SetHUDHealth(AttributeComponent->GetCurrentHp(), AttributeComponent->GetMaxHp());
//	}
//}
//
//void ABlasterCharacter::UpdateHUDShield()
//{
//	BlasterPlayerController = BlasterPlayerController.Get() == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController.Get();
//	if (BlasterPlayerController)
//		BlasterPlayerController->SetHUDShield(AttributeComponent->GetCurrentShield(), AttributeComponent->GetMaxShield());
//
//}
//
//void ABlasterCharacter::UpdateHUDAmmo()
//{
//	BlasterPlayerController = BlasterPlayerController.Get() == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController.Get();
//	if (BlasterPlayerController &&  && ->EquippedWeapon)
//	{
//		BlasterPlayerController->SetHUDCarriedAmmo(->CarriedAmmo);
//		BlasterPlayerController->SetHUDWeaponAmmo(->EquippedWeapon->GetAmmo());
//	}
//}

void ABlasterCharacter::PollInit()
{
	if (BlasterPlayerState == nullptr)
	{
		BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
		if (BlasterPlayerState)
		{
			OnPlayerStateInitialized();

			ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
			if (BlasterGameState && BlasterGameState->TopScoringPlayers.Contains(BlasterPlayerState))
			{
				//MulticastGainedTheLead();
			}
		}
	}

	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController;

	if (IsLocallyControlled())
	{
		AttributeComponent->OnSpChanged.AddUObject(BlasterPlayerController, &ABlasterPlayerController::SetHUDSp);
		AttributeComponent->OnParryGaugeChanged.AddUObject(BlasterPlayerController, &ABlasterPlayerController::SetHUDParryGauge);
		AttributeComponent->OnHpChanged.AddUObject(BlasterPlayerController, &ABlasterPlayerController::SetHUDHealth);

	}

}

void ABlasterCharacter::OnPlayerStateInitialized()
{
	BlasterPlayerState->AddToScore(0.f);
	BlasterPlayerState->AddToDefeats(0);
	SetTeamColor(BlasterPlayerState->IGetTeam());
	SetSpawnPoint();
}

void ABlasterCharacter::InitializeDefaults()
{

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	CameraBoom->AddRelativeLocation(FVector(0.f, 0.f, 80.f));
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetupAttachment(GetCapsuleComponent());

	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 420.f);
	bUseControllerRotationYaw = false;



	BuffComponent->SetIsReplicated(true);

	AttachedGrenade->SetupAttachment(GetMesh(), TEXT("GrenadeSocket"));
	AttachedGrenade->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Hit boxes for server-side rewind
	//head = CreateDefaultSubobject<UBoxComponent>(TEXT("head"));
	//head->SetupAttachment(GetMesh(), TEXT("head"));
	//head->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("head"), head);

	//pelvis = CreateDefaultSubobject<UBoxComponent>(TEXT("pelvis"));
	//pelvis->SetupAttachment(GetMesh(), TEXT("pelvis"));
	//pelvis->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("pelvis"), pelvis);

	//spine_02 = CreateDefaultSubobject<UBoxComponent>(TEXT("spine_02"));
	//spine_02->SetupAttachment(GetMesh(), TEXT("spine_02"));
	//spine_02->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("spine_02"), spine_02);

	//spine_03 = CreateDefaultSubobject<UBoxComponent>(TEXT("spine_03"));
	//spine_03->SetupAttachment(GetMesh(), TEXT("spine_03"));
	//spine_03->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("spine_03"), spine_03);

	//upperarm_l = CreateDefaultSubobject<UBoxComponent>(TEXT("upperarm_l"));
	//upperarm_l->SetupAttachment(GetMesh(), TEXT("upperarm_l"));
	//upperarm_l->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("upperarm_l"), upperarm_l);

	//upperarm_r = CreateDefaultSubobject<UBoxComponent>(TEXT("upperarm_r"));
	//upperarm_r->SetupAttachment(GetMesh(), TEXT("upperarm_r"));
	//upperarm_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("upperarm_r"), upperarm_r);

	//lowerarm_l = CreateDefaultSubobject<UBoxComponent>(TEXT("lowerarm_l"));
	//lowerarm_l->SetupAttachment(GetMesh(), TEXT("lowerarm_l"));
	//lowerarm_l->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("lowerarm_l"), lowerarm_l);

	//lowerarm_r = CreateDefaultSubobject<UBoxComponent>(TEXT("lowerarm_r"));
	//lowerarm_r->SetupAttachment(GetMesh(), TEXT("lowerarm_r"));
	//lowerarm_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("lowerarm_r"), lowerarm_r);

	//hand_l = CreateDefaultSubobject<UBoxComponent>(TEXT("hand_l"));
	//hand_l->SetupAttachment(GetMesh(), TEXT("hand_l"));
	//hand_l->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("hand_l"), hand_l);

	//hand_r = CreateDefaultSubobject<UBoxComponent>(TEXT("hand_r"));
	//hand_r->SetupAttachment(GetMesh(), TEXT("hand_r"));
	//hand_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("hand_r"), hand_r);

	//thigh_l = CreateDefaultSubobject<UBoxComponent>(TEXT("thigh_l"));
	//thigh_l->SetupAttachment(GetMesh(), TEXT("thigh_l"));
	//thigh_l->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("thigh_l"), thigh_l);

	//thigh_r = CreateDefaultSubobject<UBoxComponent>(TEXT("thigh_r"));
	//thigh_r->SetupAttachment(GetMesh(), TEXT("thigh_r"));
	//thigh_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("thigh_r"), thigh_r);

	//calf_l = CreateDefaultSubobject<UBoxComponent>(TEXT("calf_l"));
	//calf_l->SetupAttachment(GetMesh(), TEXT("calf_l"));
	//calf_l->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("calf_l"), calf_l);

	//calf_r = CreateDefaultSubobject<UBoxComponent>(TEXT("calf_r"));
	//calf_r->SetupAttachment(GetMesh(), TEXT("calf_r"));
	//calf_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("calf_r"), calf_r);

	//foot_l = CreateDefaultSubobject<UBoxComponent>(TEXT("foot_l"));
	//foot_l->SetupAttachment(GetMesh(), TEXT("foot_l"));
	//foot_l->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("foot_l"), foot_l);

	//foot_r = CreateDefaultSubobject<UBoxComponent>(TEXT("foot_r"));
	//foot_r->SetupAttachment(GetMesh(), TEXT("foot_r"));
	//foot_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitCollisionBoxes.Add(TEXT("foot_r"), foot_r);
}

void ABlasterCharacter::InitializeCollisionStates()
{
	Super::InitializeCollisionStates();

	//GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
}

void ABlasterCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	Super::ReceiveDamage(DamagedActor, Damage, DamageType, InstigatorController, DamageCauser);

	UE_LOG(LogTemp, Display, TEXT("Blaster ReceiveDamage"));

	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController;

	if (AttributeComponent == nullptr || BlasterPlayerController == nullptr || BlasterGameMode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ReceiveDamage Error!"));
		return;
	}

	BlasterPlayerController->UpdateHUDHealth();
	BlasterPlayerController->UpdateHUDShield();

	if (AttributeComponent->GetCurrentHp() <= 0.f)
	{
		ABlasterPlayerController* AttackerController = Cast<ABlasterPlayerController>(InstigatorController);
		BlasterGameMode->PlayerEliminated(this, BlasterPlayerController, AttackerController);
	}

}

//float ABlasterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//
//	UE_LOG(LogTemp, Display, TEXT("TakeDamage"));
//
//	return 0.0f;
//}

//bool ABlasterCharacter::IIsPlayer()
//{
//	return true;
//}

void ABlasterCharacter::IGetItem(AItem* InWeapon)
{
	//UE_LOG(LogTemp, Display, TEXT("IGetItem : x") , InWeapon);
	// For Server
	if (IsLocallyControlled())
	{
		if (InWeapon)
		{
			//UE_LOG(LogTemp, Display, TEXT("ShowPickupWidget"));
			if (OverlappingWeapon)
			{
				OverlappingWeapon->ShowPickupWidget(false);
			}
			InWeapon->ShowPickupWidget(true);
			//OverlappingWeaponBackup = InWeapon;
		}
		else
		{
			//OverlappingWeaponBackup->ShowPickupWidget(false);
			if (OverlappingWeapon)
			{
				OverlappingWeapon->ShowPickupWidget(false);
			}
		}
	}

	OverlappingWeapon = Cast<AWeapon>(InWeapon);
}

// Called to bind functionality to input
void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//UE_LOG(LogTemp, Display, TEXT("SetupPlayerInputComponent"));
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{

		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Triggered, this, &ThisClass::_Crouch);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(IA_Equip, ETriggerEvent::Triggered, this, &ThisClass::Interact);
		//EnhancedInputComponent->BindAction(IA_Aim, ETriggerEvent::Triggered, this, &ThisClass::AimButtonToggle);
		EnhancedInputComponent->BindAction(IA_Aim, ETriggerEvent::Started, this, &ThisClass::AimButtonPressed);
		EnhancedInputComponent->BindAction(IA_Aim, ETriggerEvent::Completed, this, &ThisClass::AimButtonReleased);

		// Semimode
		//EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &ThisClass::FireButtonPressed);
		// Automode
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Ongoing, this, &ThisClass::FireButtonPressed);

		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Completed, this, &ThisClass::FireButtonReleased);
		EnhancedInputComponent->BindAction(IA_Reload, ETriggerEvent::Triggered, this, &ThisClass::ReloadButtonPressed);
		EnhancedInputComponent->BindAction(IA_ThrowGrenade, ETriggerEvent::Triggered, this, &ThisClass::ThrowGrenadeButtonPressed);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &ThisClass::Sprint);
		EnhancedInputComponent->BindAction(IA_Slide, ETriggerEvent::Triggered, this, &ThisClass::SlideButtonPressed);
		EnhancedInputComponent->BindAction(IA_Dodge, ETriggerEvent::Triggered, this, &ThisClass::DodgeButtonPressed);
		EnhancedInputComponent->BindAction(IA_VaultOrMantle, ETriggerEvent::Triggered, this, &ThisClass::VaultOrMantleButtonPressed);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Started, this, &ThisClass::DashButtonPressed);
		EnhancedInputComponent->BindAction(IA_Melee, ETriggerEvent::Triggered, this, &ThisClass::MeleeButtonPressed);
		EnhancedInputComponent->BindAction(IA_TestingKey, ETriggerEvent::Triggered, this, &ThisClass::TestingButtonPressed);

		EnhancedInputComponent->BindAction(IA_SkillKey[0], ETriggerEvent::Triggered, this, &ThisClass::SkillButtonPressed0);
		EnhancedInputComponent->BindAction(IA_SkillKey[1], ETriggerEvent::Triggered, this, &ThisClass::SkillButtonPressed1);
		EnhancedInputComponent->BindAction(IA_SkillKey[2], ETriggerEvent::Triggered, this, &ThisClass::SkillButtonPressed2);
		EnhancedInputComponent->BindAction(IA_SkillKey[3], ETriggerEvent::Triggered, this, &ThisClass::SkillButtonPressed3);
		EnhancedInputComponent->BindAction(IA_SkillKey[4], ETriggerEvent::Triggered, this, &ThisClass::SkillButtonPressed4);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EnhancedInputComponent is null"));
	}
}

void ABlasterCharacter::_Crouch()
{
	//if (!HasAuthority() && IsLocallyControlled()) UE_LOG(LogTemp, Display, TEXT("BlasterCharacter : bHoldingTheFlag : %d"), ->bHoldingTheFlag);
	if (bDisableGameplay) return;
	if (bIsCrouched)
	{
		if (bHoldingTheFlag) return;

		Super::UnCrouch();
		return;
	}

	if (!GetCharacterMovement()->IsFalling())
	{
		Super::Crouch();
	}
}

void ABlasterCharacter::Jump()
{
	//UE_LOG(LogTemp, Display, TEXT("Jump"));
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

void ABlasterCharacter::Look(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Display, TEXT("Look"));

	FVector2D Vec = Value.Get<FVector2d>();

	AddControllerYawInput(Vec.X);
	AddControllerPitchInput(Vec.Y);
}

void ABlasterCharacter::Move(const FInputActionValue& Value)
{
	if (bDisableGameplay) return;
	if (CombatState == ECombatState::ECS_Ragdoll) return;
	//UE_LOG(LogTemp, Display, TEXT("Move"));
	FVector2D Vec = Value.Get<FVector2d>();

	FRotator Rotator = GetController()->GetControlRotation();
	Rotator = FRotator(0.f, Rotator.Yaw, 0.f);

	FVector ForwardVec = FRotationMatrix(Rotator).GetUnitAxis(EAxis::Y);
	FVector RightVec = FRotationMatrix(Rotator).GetUnitAxis(EAxis::X);


	if (Vec.X > 0.5f)
	{
		KeyType = EKeyType::EKT_Fwd;
		KeySectionName = TEXT("Forward");
	}
	else if (Vec.X < -0.5f)
	{
		KeyType = EKeyType::EKT_Bwd;
		KeySectionName = TEXT("Backward");
	}
	else if (Vec.Y > 0.5f)
	{
		KeyType = EKeyType::EKT_Right;
		KeySectionName = TEXT("Right");
	}
	else if (Vec.Y < 0.5f)
	{
		KeyType = EKeyType::EKT_Left;
		KeySectionName = TEXT("Left");
	}

	//UE_LOG(LogTemp, Display, TEXT("KeyType : %s"), *UEnum::GetDisplayValueAsText(KeyType).ToString());

	AddMovementInput(ForwardVec, Vec.Y);
	AddMovementInput(RightVec, Vec.X);


}

void ABlasterCharacter::ServerEquipButtonPressed_Implementation(AWeapon* InItem)
{
	//UE_LOG(LogTemp, Display, TEXT("ServerEquipButtonPressed_Implementation"));
	//UE_LOG(LogTemp, Display, TEXT("%s : OverlappingItem : %x"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString(),  OverlappingWeapon.Get());

	EquipButtonFunc(InItem);

}

void ABlasterCharacter::Interact()
{
	//UE_LOG(LogTemp, Display, TEXT("Interact"));
	if (bDisableGameplay) return;
	if (bHoldingTheFlag) return;
	//if (HasAuthority())
	//{
	//	UE_LOG(LogTemp, Display, TEXT("HasAuthority"));
	//}

	//if (GetLocalRole() == ENetRole::ROLE_Authority)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("ROLE_Authority"));
	//}

	//if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("ROLE_AutonomousProxy"));
	//}

	//if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("ROLE_SimulatedProxy"));
	//}


	if (IsLocallyControlled())
	{
		//if (!HasAuthority())
		//{
		//	EquipButtonFunc(OverlappingWeapon);
		//}
		ServerEquipButtonPressed(OverlappingWeapon);
	}


	//if (CombatState == ECombatState::ECS_Unoccupied) ServerEquipButtonPressed();
	//bool bSwap = ShouldSwapWeapons() && !HasAuthority() && CombatState == ECombatState::ECS_Unoccupied && OverlappingWeapon == nullptr;
	//if (bSwap)
	//{
	//	//UE_LOG(LogTemp, Display, TEXT("Swap"));
	//	PlaySwapMontage();
	//	CombatState = ECombatState::ECS_SwappingWeapon;
	//	bFinishedSwapping = false;
	//}
}

void ABlasterCharacter::AimButtonToggle()
{
	if (bDisableGameplay) return;
	if (EquippedWeapon)
	{
		if (bIsAiming)
		{
			//UE_LOG(LogTemp, Display, TEXT("Not Aiming"));
			SetAiming(false);

		}
		else
		{
			//UE_LOG(LogTemp, Display, TEXT("Aiming"));
			SetAiming(true);
		}

	}
}

void ABlasterCharacter::AimButtonPressed(const FInputActionValue& isPressed)
{
	//UE_LOG(LogTemp, Display, TEXT("Pressed : %d"), isPressed.Get<bool>());
	if (bDisableGameplay) return;
	if (bHoldingTheFlag) return;

	if (EquippedWeapon)
	{
		//UE_LOG(LogTemp, Display, TEXT("Aiming"));
		SetAiming(true);
	}
}

void ABlasterCharacter::AimButtonReleased(const FInputActionValue& isPressed)
{
	//UE_LOG(LogTemp, Display, TEXT("Released : %d"), isPressed.Get<bool>());
	if (bDisableGameplay) return;
	if (bHoldingTheFlag) return;
	if (EquippedWeapon)
	{
		//UE_LOG(LogTemp, Display, TEXT("Not Aiming"));
		SetAiming(false);
	}
}

void ABlasterCharacter::FireButtonPressed(const FInputActionValue& isPressed)
{
	//UE_LOG(LogTemp, Display, TEXT("Pressed : %d"), isPressed.Get<bool>());
	if (bDisableGameplay) return;
	if (bHoldingTheFlag) return;

	if (IsAiming() && !bIsFirebuttonPressed && !bCheckIsSemi && !bIsFiring)
	{

		//UE_LOG(LogTemp, Display, TEXT("Fire"));
		bIsFirebuttonPressed = true;
		NotifyFireButtonPressed(true);

		Fire(true);

		AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);

		if (Gun && !Gun->IsAutomatic()) bCheckIsSemi = true;
	}

	//if (IsAiming())
	//{
	//	Super::FireButtonPressed(true);
	//}
}

void ABlasterCharacter::FireButtonReleased(const FInputActionValue& isPressed)
{
	//UE_LOG(LogTemp, Display, TEXT("Released : %d"), isPressed.Get<bool>());
	if (bDisableGameplay) return;
	if (bHoldingTheFlag) return;
	if (IsAiming())
	{
		bIsFirebuttonPressed = false;
		NotifyFireButtonPressed(false);
		Super::Fire(false);
		bCheckIsSemi = false;
	} 
}

void ABlasterCharacter::NotifyFireButtonPressed_Implementation(bool IsFired)
{
	bReloadStopCheck = IsFired;
}

void ABlasterCharacter::ReloadButtonPressed()
{
	//UE_LOG(LogTemp, Display, TEXT("ReloadButtonPressed"));
	if (bDisableGameplay) return;
	if (bHoldingTheFlag) return;
	Reload();
}

void ABlasterCharacter::ThrowGrenadeButtonPressed()
{
	if (bDisableGameplay) return;
	if (bHoldingTheFlag) return;
	ThrowGrenade();
}

void ABlasterCharacter::Sprint()
{
	//UE_LOG(LogTemp, Display, TEXT("Sprint"));
	if (bDisableGameplay) return;

	bIsSprint = bIsSprint == false ? true : false;
}

void ABlasterCharacter::SlideButtonPressed()
{
	if (bDisableGameplay) return;
	Slide();
}

void ABlasterCharacter::DodgeButtonPressed()
{
	if (bDisableGameplay) return;

	//if (bCanDodge)
	//{
	//	if (!Dodge(KeySectionName)) return;
	//	bCanDodge = false;
	//	OnSkillStarted.Broadcast(TEXT("Active"), 1, DodgeCoolTime);
	//}

	FCoolTimeCheckStruct* S = SkillComponent->CoolTimeMap.Find(TEXT("Dodge"));

	if (S->bCanExecute)
	{
		if (!Dodge(KeySectionName)) return;
		S->bCanExecute = false;
		SkillComponent->OnSkillCoolTimeStarted.Broadcast(TEXT("Active"), 1, S->CoolTime);
	}

}

void ABlasterCharacter::VaultOrMantleButtonPressed()
{
	if (bDisableGameplay) return;
	VaultOrMantle();
}

void ABlasterCharacter::DashButtonPressed()
{
	if (bDisableGameplay) return;
	++MoveButtonPressedCount;

	MoveButtonPressedCount = FMath::Clamp(MoveButtonPressedCount, 0, 2);
	if (MoveButtonPressedCount == 2)
	{
		//if (bCanDash)
		//{
		//	if (!Dash(KeySectionName)) return;
		//	bCanDash = false;
		//	OnSkillStarted.Broadcast(TEXT("Active"), 2, DashCoolTime);
		//}

		FCoolTimeCheckStruct* S = SkillComponent->CoolTimeMap.Find(TEXT("Dash"));

		if (S->bCanExecute)
		{
			if (!Dash(KeySectionName)) return;
			S->bCanExecute = false;
			SkillComponent->OnSkillCoolTimeStarted.Broadcast(TEXT("Active"), 2, S->CoolTime);
		}
	}

	FTimerHandle DashHandle;
	GetWorldTimerManager().SetTimer(DashHandle, FTimerDelegate::CreateLambda([&]()
		{
			MoveButtonPressedCount = 0;
		}), 0.3f, false);

}

void ABlasterCharacter::MeleeButtonPressed()
{
	if (bDisableGameplay) return;
	Melee();
}

void ABlasterCharacter::TestingButtonPressed()
{
	//SetReplicateMovement(false);

	//ServerTest();
	Test();
}

void ABlasterCharacter::SkillButtonPressed0()
{
	SkillComponent->SkillButtonPressed(0);
}

void ABlasterCharacter::SkillButtonPressed1()
{
	SkillComponent->SkillButtonPressed(1);
}

void ABlasterCharacter::SkillButtonPressed2()
{
	SkillComponent->SkillButtonPressed(2);
}

void ABlasterCharacter::SkillButtonPressed3()
{
	SkillComponent->SkillButtonPressed(3);
}

void ABlasterCharacter::SkillButtonPressed4()
{
	SkillComponent->SkillButtonPressed(4);
}



//void ABlasterCharacter::HitCapsuleConstruction()
//{
//	if (GetMesh() == nullptr) return;
//
//	for (const auto& x : GetMesh()->GetPhysicsAsset()->SkeletalBodySetups)
//	{
//		FName BName = x->BoneName;
//		FTransform BoneWorldTransform = GetMesh()->GetBoneTransform(GetMesh()->GetBoneIndex(BName));
//		for (const auto& y : x->AggGeom.SphylElems)
//		{
//			FTransform LocalTransform = y.GetTransform();
//			FTransform WorldTransform = LocalTransform * BoneWorldTransform;
//
//			FPhysAssetInformation AssetInfo;
//			AssetInfo.BoneName = BName;
//			AssetInfo.HalfHeight = y.Length / 2 + y.Radius;
//			AssetInfo.Radius = y.Radius;
//			AssetInfo.BoneWorldTransform = WorldTransform;
//
//			PhysAssetInfo.Add(AssetInfo);
//		}
//	}
//
//	for (size_t i = 0; i < PhysAssetInfo.Num(); i++)
//	{
//		SetupHitCapsule(PhysAssetInfo[i]);
//	}
//}
//
//void ABlasterCharacter::SetupHitCapsule(FPhysAssetInformation PhysicsAssetInfo)
//{
//	UCapsuleComponent* HitCapsule;
//	HitCapsule = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), PhysicsAssetInfo.BoneName, RF_Transient);
//	//HitCapsule = CreateDefaultSubobject<UCapsuleComponent>(PhysicsAssetInfo.BoneName);
//
//	HitCapsule->SetupAttachment(GetMesh(), PhysicsAssetInfo.BoneName);
//	HitCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	HitCapsule->SetWorldTransform(PhysicsAssetInfo.BoneWorldTransform);
//	HitCapsule->SetCapsuleHalfHeight(PhysicsAssetInfo.HalfHeight);
//	HitCapsule->SetCapsuleRadius(PhysicsAssetInfo.Radius);
//
//	HitCapsule->RegisterComponent();
//	HitCapsuleBones.Add(HitCapsule);
//	HitCollisionCapsules.Add(PhysicsAssetInfo.BoneName, HitCapsule);
//}



//void ABlasterCharacter::PlayFireMontage(bool bIsAiming)
//{
//	if (! || !->EquippedWeapon) return;
//
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance && AttackMontage)
//	{
//		AnimInstance->Montage_Play(AttackMontage);
//		FName SectionName;
//		SectionName = bIsAiming ? TEXT("RifleAim") : TEXT("RifleHip");
//		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
//	}
//}
//
//void ABlasterCharacter::PlayDeadMontage()
//{
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance && DeadMontage)
//	{
//		//UE_LOG(LogTemp, Display, TEXT("PlayDeadMontage"));
//		AnimInstance->Montage_Play(DeadMontage);
//		AnimInstance->Montage_JumpToSection(TEXT("Forward"), DeadMontage);
//	}
//}
//
//void ABlasterCharacter::PlayCombatHitReactMontage()
//{
//	if (! || !->EquippedWeapon) return;
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
//
//void ABlasterCharacter::PlayReloadMontage()
//{
//	if (! || !->EquippedWeapon) return;
//
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance && ReloadMontage)
//	{
//		AnimInstance->Montage_Play(ReloadMontage);
//		FName SectionName;
//		switch (->EquippedWeapon->GetWeaponType())
//		{
//		case EWeaponType::EWT_AssaultRifle:
//			SectionName = TEXT("Rifle");
//			break;
//		case EWeaponType::EWT_RocketLauncher:
//			SectionName = TEXT("RocketLauncher");
//			break;
//		case EWeaponType::EWT_Pistol:
//			SectionName = TEXT("Pistol");
//			break;
//		case EWeaponType::EWT_SMG:
//			SectionName = TEXT("SMG");
//			break;
//		case EWeaponType::EWT_Shotgun:
//			SectionName = TEXT("Shotgun");
//			break;
//		case EWeaponType::EWT_SniperRifle:
//			SectionName = TEXT("SniperRifle");
//			break;
//		case EWeaponType::EWT_GrenadeLauncher:
//			SectionName = TEXT("GrenadeLauncher");
//			break;
//		default:
//			break;
//		}
//		AnimInstance->Montage_JumpToSection(SectionName, ReloadMontage);
//	}
//}
//
//void ABlasterCharacter::PlayThrowGrenadeMontage()
//{
//
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance && ThrowGrenadeMontage)
//	{
//		AnimInstance->Montage_Play(ThrowGrenadeMontage);
//	}
//}
//
//void ABlasterCharacter::PlaySwapMontage()
//{
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance && SwapMontage)
//	{
//		AnimInstance->Montage_Play(SwapMontage);
//	}
//}
//
//void ABlasterCharacter::PlaySlideMontage()
//{
//	//UE_LOG(LogTemp, Display, TEXT("PlaySlideMontage"));
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance && SlideMontage)
//	{
//		AnimInstance->Montage_Play(SlideMontage);
//	}
//}
//
//void ABlasterCharacter::PlayDodgeMontage(FName SectionName)
//{
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance && DodgeMontage)
//	{
//		AnimInstance->Montage_Play(DodgeMontage);
//		AnimInstance->Montage_JumpToSection(SectionName, DodgeMontage);
//	}
//}
//
//void ABlasterCharacter::PlayVaultMontage(FName SectionName)
//{
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance && VaultMontage)
//	{
//		AnimInstance->Montage_Play(VaultMontage);
//		if (SectionName.ToString() == TEXT("")) return;
//		AnimInstance->Montage_JumpToSection(SectionName, VaultMontage);
//	}
//}
//
//void ABlasterCharacter::PlayMantleMontage(FName SectionName)
//{
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance && MantleMontage)
//	{
//		AnimInstance->Montage_Play(MantleMontage);
//		if (SectionName.ToString() == TEXT("")) return;
//		AnimInstance->Montage_JumpToSection(SectionName, MantleMontage);
//	}
//}



void ABlasterCharacter::HideCameraIfCharacterClose()
{
	if (!IsLocallyControlled()) return;
	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
	if (!Gun) return;

	if ((FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CameraThreshold)
	{
		GetMesh()->SetVisibility(false);

		if (Gun->GetWeaponMesh())
		{
			Gun->GetWeaponMesh()->bOwnerNoSee = true;
		}

		if (Gun->GetWeaponMesh())
		{
			Gun->GetWeaponMesh()->bOwnerNoSee = true;
		}
	}
	else
	{
		GetMesh()->SetVisibility(true);
		if (Gun->GetWeaponMesh())
		{
			Gun->GetWeaponMesh()->bOwnerNoSee = false;
		}

		if (Gun->GetWeaponMesh())
		{
			Gun->GetWeaponMesh()->bOwnerNoSee = false;
		}
	}
}



//void ABlasterCharacter::OnRep_Health(float LastHealth)
//{
//	if (AttributeComponent == nullptr) return;
//
//	UpdateHUDHealth();
//	if (AttributeComponent->GetCurrentHp() < LastHealth)
//	{
//		PlayCombatHitReactMontage();
//	}
//}
//
//void ABlasterCharacter::OnRep_Shield(float LastShield)
//{
//	if (AttributeComponent == nullptr) return;
//
//	UpdateHUDShield();
//	if (AttributeComponent->GetCurrentShield() < LastShield)
//	{
//		//UE_LOG(LogTemp, Display, TEXT("OnRep_Shield, LastShield : %f"), LastShield);
//		PlayCombatHitReactMontage();
//	}
//}

//void ABlasterCharacter::UpdateDissolveMaterial(float DissolveValue)
//{
//	if (DynamicDissolveMaterialInstance)
//	{
//		//UE_LOG(LogTemp, Display, TEXT("UpdateDissolveMaterial"));
//		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), DissolveValue);
//	}
//}
//
//void ABlasterCharacter::StartDissolve()
//{
//	DissolveTrack.BindDynamic(this, &ThisClass::UpdateDissolveMaterial);
//	if (DissolveCurve && DissolveTimelineComponent)
//	{
//		DissolveTimelineComponent->AddInterpFloat(DissolveCurve, DissolveTrack);
//		DissolveTimelineComponent->Play();
//	}
//}
//
//void ABlasterCharacter::SetTeamColor(ETeam Team)
//{
//	if (!IsValid(GetMesh()) || !IsValid(OriginalMaterial)) return;
//	switch (Team)
//	{
//	case ETeam::ET_NoTeam:
//		GetMesh()->SetMaterial(1, OriginalMaterial);
//		DissolveMaterialInstance = BlueDissolveMatInst;
//		break;
//	case ETeam::ET_RedTeam:
//		GetMesh()->SetMaterial(1, RedMaterial);
//		DissolveMaterialInstance = RedDissolveMatInst;
//		break;
//	case ETeam::ET_BlueTeam:
//		GetMesh()->SetMaterial(1, BlueMaterial);
//		DissolveMaterialInstance = BlueDissolveMatInst;
//		break;
//	case ETeam::ET_MAX:
//		break;
//	default:
//		break;
//	}
//}
//
//void ABlasterCharacter::Elim(bool bPlayerLeftGame)
//{
//	if ()
//	{
//		if (->EquippedWeapon)
//		{
//			DropOrDestroyWeapon(->EquippedWeapon);
//		}
//
//		if (->SecondaryWeapon)
//		{
//			DropOrDestroyWeapon(->SecondaryWeapon);
//		}
//
//		if (->Flag)
//		{
//			->bHoldingTheFlag = false;
//			->Flag->Dropped();
//		}
//	}
//
//	MulticastElim(bPlayerLeftGame);
//}
//
//void ABlasterCharacter::DropOrDestroyWeapon(AWeapon_Gun* InWeapon)
//{
//	//UE_LOG(LogTemp, Display, TEXT("DropOrDestroyWeapon"));
//	if (InWeapon == nullptr) return;
//
//	if (InWeapon->bDestroyWeapon)
//	{
//		//UE_LOG(LogTemp, Display, TEXT("DropOrDestroyWeapon : Destroy"));
//		InWeapon->Destroy();
//	}
//	else
//	{
//		//UE_LOG(LogTemp, Display, TEXT("DropOrDestroyWeapon : Dropped"));
//		InWeapon->Dropped();
//
//	}
//}
//
void ABlasterCharacter::MulticastElim(bool bPlayerLeftGame)
{
	//UE_LOG(LogTemp, Display, TEXT("MulticastElim_Implementation"));


	bLeftGame = bPlayerLeftGame;

	//if (BlasterPlayerController)
	//{
	//	BlasterPlayerController->SetHUDWeaponAmmo(0);
	//}

	//bIsElimmed = true;

	//if (DissolveMaterialInstance)
	//{
	//	DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
	//	GetMesh()->SetMaterial(0, DynamicDissolveMaterialInstance);
	//	DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), 0.55f);
	//	DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Glow"), 200.f);
	//}

	//StartDissolve();

	//GetCharacterMovement()->DisableMovement();
	//GetCharacterMovement()->StopMovementImmediately();

	//if (BlasterPlayerController && IsLocallyControlled())
	//{
	//	DisableInput(BlasterPlayerController);
	//}
	//bDisableGameplay = true;
	//if ()
	//{
	//	->FireButtonPressed(false);
	//}

	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//PlayDeadMontage();
	//FTimerHandle Timerhandle;
	//GetWorldTimerManager().SetTimer(Timerhandle, FTimerDelegate::CreateLambda([&]()
	//	{
	//		PlayDeadMontage();
	//	}), 0.1f, false);

	//if (ElimBotEffect)
	//{
	//	FVector ElimBotSpawnPoint(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 200.f);
	//	ElimBotEffectComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ElimBotEffect, ElimBotSpawnPoint, GetActorRotation());
	//}

	//if (ElimBotSound)
	//{
	//	UGameplayStatics::SpawnSoundAtLocation(this, ElimBotSound, GetActorLocation());
	//}

	bool bHideSniperScope = IsLocallyControlled() && bIsAiming && EquippedWeapon && EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle;

	if (bHideSniperScope)
	{
		ShowSniperScopeWidget(false);
	}

	//if (CrownNiagaraComponent)
	//{
	//	CrownNiagaraComponent->DestroyComponent();
	//}

	//GetWorldTimerManager().SetTimer(ElimTimer, this, &ThisClass::ElimTimerFinished, ElimDelay);

	Super::MulticastElim(bPlayerLeftGame);
}
//
//void ABlasterCharacter::SetSpawnPoint()
//{
//	if (HasAuthority() && BlasterPlayerState->GetTeam() != ETeam::ET_NoTeam)
//	{
//		TArray<AActor*> PlayerStarts;
//		UGameplayStatics::GetAllActorsOfClass(this, ATeamPlayerStart::StaticClass(), PlayerStarts);
//		TArray<ATeamPlayerStart*> TeamPlayerStarts;
//
//		for (const auto& Start : PlayerStarts)
//		{
//			ATeamPlayerStart* TeamStart = Cast<ATeamPlayerStart>(Start);
//			if (TeamStart && TeamStart->Team == BlasterPlayerState->GetTeam())
//			{
//				TeamPlayerStarts.Add(TeamStart);
//			}
//		}
//
//		if (TeamPlayerStarts.Num() > 0)
//		{
//			ATeamPlayerStart* ChosenPlayerStart = TeamPlayerStarts[FMath::RandRange(0, TeamPlayerStarts.Num() - 1)];
//			SetActorLocationAndRotation(ChosenPlayerStart->GetActorLocation(), ChosenPlayerStart->GetActorRotation());
//		}
//	}
//}
//
void ABlasterCharacter::ElimTimerFinished()
{
	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	if (BlasterGameMode && !bLeftGame)
	{
		BlasterGameMode->RequestRespawn(this, GetController());
	}

	if (bLeftGame && IsLocallyControlled()) // client who is trying to leave the game
	{
		OnLeftGame.Broadcast();
	}

	//if (ElimBotEffectComponent)
	//{
	//	ElimBotEffectComponent->DestroyComponent();
	//} 
}

void ABlasterCharacter::ServerLeaveGame_Implementation()
{
	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	BlasterPlayerState = BlasterPlayerState == nullptr ? GetPlayerState<ABlasterPlayerState>() : BlasterPlayerState;
	if (BlasterGameMode && BlasterPlayerState)
	{
		BlasterGameMode->PlayerLeftGame(BlasterPlayerState);
	}
}

//void ABlasterCharacter::SpawnDefaultWeapon()
//{
//	BlasterGameMode = BlasterGameMode == nullptr ? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
//	UWorld* World = GetWorld();
//	if (BlasterGameMode && World && !bIsElimmed && DefaultWeaponClass)
//	{
//		AWeapon_Gun* StartingWeapon = World->SpawnActor<AWeapon_Gun>(DefaultWeaponClass);
//		StartingWeapon->bDestroyWeapon = true;
//		if ()
//		{
//			->EquipWeapon(StartingWeapon);
//		}
//	}
//}

void ABlasterCharacter::MulticastGainedTheLead()
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

void ABlasterCharacter::MulticastLostTheLead()
{
	if (CrownNiagaraComponent)
	{
		CrownNiagaraComponent->DestroyComponent();
	}
}

void ABlasterCharacter::SetAiming(bool InbIsAiming)
{
	//UE_LOG(LogTemp, Display, TEXT("IsAiming : %d"), InbIsAiming);
	Super::SetAiming(InbIsAiming);

	if (IsLocallyControlled() && EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
	{
		ShowSniperScopeWidget(InbIsAiming);
	}
}

void ABlasterCharacter::FinishSwapAttachWeapons()
{
	Super::FinishSwapAttachWeapons();

	if (IsLocallyControlled())
	{
		//ABlasterPlayerController* PC = Cast<ABlasterPlayerController>(GetController());
		//if (PC)
		//{
		//	PC->SetHUDWeaponType(EquippedWeapon->GetWeaponName());
		//}
	}
}

void ABlasterCharacter::EquipWeaponFunc()
{
	Super::EquipWeaponFunc();

	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController;
	if (BlasterPlayerController)
	{
		BlasterPlayerController->SetHUDWeaponType(EquippedWeapon->GetWeaponName());
	}
}

void ABlasterCharacter::EquipButtonFunc(AWeapon* InWeapon)
{
	OverlappingWeapon = Cast<AWeapon>(InWeapon);

	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
		OverlappingWeapon = nullptr;
	}
	else if (ShouldSwapWeapons())
	{
		SwapWeapons();
	}
}

//void ABlasterCharacter::MulticastHit_Implementation()
//{
//	PlayCombatHitReactMontage();
//}


void ABlasterCharacter::SetHUDCrosshairs(float DeltaTime)
{
	// 이 방법의 장점은 매프레임마다 호출될 때 캐스팅을 할 필요가 없다는 것임
	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController;

	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
	if (!Gun) return;

	if (BlasterPlayerController)
	{
		HUD = HUD == nullptr ? Cast<ABlasterHUD>(BlasterPlayerController->GetHUD()) : HUD;

		if (HUD)
		{

			if (EquippedWeapon)
			{
				HUDPackage.CrosshairCenter = Gun->CrosshairCenter;
				HUDPackage.CrosshairLeft = Gun->CrosshairLeft;
				HUDPackage.CrosshairRight = Gun->CrosshairRight;
				HUDPackage.CrosshairTop = Gun->CrosshairTop;
				HUDPackage.CrosshairBottom = Gun->CrosshairBottom;
			}
			else
			{
				HUDPackage.CrosshairCenter = nullptr;
				HUDPackage.CrosshairLeft = nullptr;
				HUDPackage.CrosshairRight = nullptr;
				HUDPackage.CrosshairTop = nullptr;
				HUDPackage.CrosshairBottom = nullptr;
			}

			// Calculate crosshair spread
			// [0, 600] -> [0, 1]
			FVector2D WalkSpeedRange(0.f, GetCharacterMovement()->MaxWalkSpeed);
			FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = GetVelocity();
			Velocity.Z = 0.f;
			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size2D());

			if (GetCharacterMovement()->IsFalling())
			{
				CrosshairAirFactor = FMath::FInterpTo(CrosshairAirFactor, 2.25f, DeltaTime, 2.25f);
			}
			else
			{
				CrosshairAirFactor = FMath::FInterpTo(CrosshairAirFactor, 0.f, DeltaTime, 30.f);
			}

			if (bIsAiming)
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, -0.58f, DeltaTime, 30.f);
			}
			else
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 30.f);
			}

			if (bIsCrouched)
			{
				CrosshairCrouchFactor = FMath::FInterpTo(CrosshairCrouchFactor, -0.58f, DeltaTime, 30.f);
			}
			else
			{
				CrosshairCrouchFactor = FMath::FInterpTo(CrosshairCrouchFactor, 0.f, DeltaTime, 30.f);
			}


			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 10.f);

			float TotalFactors = 0.5f + CrosshairVelocityFactor + CrosshairAirFactor + CrosshairAimFactor + CrosshairShootingFactor + CrosshairCrouchFactor;

			HUDPackage.CrosshairSpread = TotalFactors;

			if (EquippedWeapon)
			{
				float OriginRadius = Gun->GetSphereRadius();

				TotalFactors *= 30.f;

				Gun->SetAdditionalSphereRadius(OriginRadius + TotalFactors);

				HUDPackage.CrosshairSpread = Gun->GetAdditionalSphereRadius();
			}

			HUD->SetHUDPackage(HUDPackage);
		}
	}
}

void ABlasterCharacter::InterpFOV(float DeltaTime)
{
	if (EquippedWeapon == nullptr) return;
	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(EquippedWeapon);
	if (!Gun) return;

	if (bIsAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, Gun->GetZoomedFOV(), DeltaTime, Gun->GetZoomInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, Gun->GetZoomInterpSpeed());
	}

	if (GetCameraComponent())
	{
		GetCameraComponent()->SetFieldOfView(CurrentFOV);
	}
}

void ABlasterCharacter::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	//UE_LOG(LogTemp, Display, TEXT("bScreenToWorld : %d"), bScreenToWorld);
	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		float DistanceToCharacter = (GetActorLocation() - Start).Size2D();
		Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
		//DrawDebugSphere(GetWorld(), Start, 5.f, 12, FColor::Red);

		FVector End = Start + CrosshairWorldDirection * 80'000.f;
		//FVector End = Start + CrosshairWorldDirection * 2147483647;

		bool bIsHit = GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);
		if (!bIsHit)
		{
			TraceHitResult.ImpactPoint = End;
		}

		if (TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UInteractWithCrosshairsInterface>())
		{

			ITeamInterface* T1 = Cast<ITeamInterface>(TraceHitResult.GetActor());
			if (T1)
			{
				if (T1->IGetTeam() == IGetTeam())
				{
					HUDPackage.CrosshairsColor = FLinearColor::Green;
				}
				else
				{
					HUDPackage.CrosshairsColor = FLinearColor::Red;
				}
			}

			
		}
		else
		{
			HUDPackage.CrosshairsColor = FLinearColor::White;
		}

		ETraceTypeQuery IsPickable = UEngineTypes::ConvertToTraceType(ECC_IsPickable);
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(this);
		FHitResult OutHitResult;
		UKismetSystemLibrary::LineTraceSingle(this, Start, Start + CrosshairWorldDirection * 200.f, IsPickable, false, IgnoreActors, EDrawDebugTrace::None, OutHitResult, true);

		AWeapon* WeaponGun = Cast<AWeapon>(OutHitResult.GetActor());

		if (OutHitResult.bBlockingHit)
		{
			HUDPackage.CrosshairsColor = FLinearColor::Green;

			IGetItem(WeaponGun);
			//WeaponGun->ShowPickupWidget(true);
			//OverlappingWeapon = WeaponGun;
			return;
		}
		else
		{
			//UE_LOG(LogTemp, Display, TEXT("No"));
			IGetItem(nullptr);
			//WeaponGun->ShowPickupWidget(false);
			//OverlappingWeapon = nullptr;
		}
	}
}

void ABlasterCharacter::ThrowGrenadeFinished()
{
	Super::ThrowGrenadeFinished();

	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController;
	if (BlasterPlayerController)
	{
		BlasterPlayerController->SetHUDGrenades(Grenades);
	}
}

void ABlasterCharacter::UpdateAmmoValues()
{
	Super::UpdateAmmoValues();

	if (IsLocallyControlled())
	{
		BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController;
		if (BlasterPlayerController)
		{
			if (EquippedWeapon) BlasterPlayerController->SetHUDCarriedAmmo(CarriedAmmo);
		}
	}

}

void ABlasterCharacter::UpdateCarriedAmmo()
{
	Super::UpdateCarriedAmmo();

	if (IsLocallyControlled())
	{
		BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController;
		if (BlasterPlayerController)
		{
			if (EquippedWeapon) BlasterPlayerController->SetHUDCarriedAmmo(CarriedAmmo);
		}
	}
}

void ABlasterCharacter::Test()
{
	ServerTest();
}

//void ABlasterCharacter::CoolTimeChecker(float DeltaTime)
//{
//
//	/*if (!bCanDodge)
//	{
//		if (DodgeCoolTimeCount > DodgeCoolTime)
//		{
//			bCanDodge = true;
//			DodgeCoolTimeCount = 0.f;
//		}
//		else
//		{
//			DodgeCoolTimeCount += DeltaTime;
//		}
//	}
//
//	if (!bCanDash)
//	{
//		if (DashCoolTimeCount > DashCoolTime)
//		{
//			bCanDash = true;
//			DashCoolTimeCount = 0.f;
//		}
//		else
//		{
//			DashCoolTimeCount += DeltaTime;
//		}
//	}*/
//
//}

//void ABlasterCharacter::InitializeCoolTimeMap()
//{
//	CoolTimeMap.Add(TEXT("Dodge"), FCoolTimeCheckStruct(3.f));
//	CoolTimeMap.Add(TEXT("Dash"), FCoolTimeCheckStruct(2.f));
//
//	CoolTimeMap.Reserve(5);
//	SkillButtonPressedChecker.Reserve(5);
//
//	for (size_t i = 0; i < 5; i++)
//	{
//		FString Str = UEnum::GetDisplayValueAsText(ESkillAssistant::ESA_HealArea).ToString();
//		CoolTimeMap.Add(Str);
//		SkillButtonPressedChecker.Add(false);
//	}
//
//
//}

void ABlasterCharacter::ServerTest_Implementation()
{
	MulticastTest();
}

void ABlasterCharacter::MulticastTest_Implementation()
{
	PlaySlideMontage();

}
