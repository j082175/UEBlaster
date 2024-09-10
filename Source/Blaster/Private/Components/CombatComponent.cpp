// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"
#include "Item/Pickable/Weapon/Weapon_Gun.h"
#include "Characters/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Characters/CharacterBase.h"
#include "Actor/Projectile/Projectile.h"
#include "Actor/Projectile/ProjectileGrenade.h"
#include "Components/BuffComponent.h"
#include "Item/Pickups/SpeedPickup.h"
#include "Item/Pickable/Weapon/Shotgun.h"

#include "PlayerController/BlasterPlayerController.h"
//#include "HUD/BlasterHUD.h"
#include "Camera/CameraComponent.h"

#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "BlasterTypes/KeyType.h"
#include "Components/MantleVaultComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;
	 
	// ...
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	//DOREPLIFETIME(UCombatComponent, SecondaryWeapon);
	//DOREPLIFETIME(UCombatComponent, bIsAiming);
	//DOREPLIFETIME(UCombatComponent, CharacterState);
	//DOREPLIFETIME_CONDITION(UCombatComponent, CarriedAmmo, COND_OwnerOnly);
	//DOREPLIFETIME(UCombatComponent, Grenades);
	//DOREPLIFETIME(UCombatComponent, bHoldingTheFlag);
	//DOREPLIFETIME(UCombatComponent, Flag);
	//DOREPLIFETIME(UCombatComponent, bIsSliding);
	//DOREPLIFETIME(UCombatComponent, bIsDodge);
	//DOREPLIFETIME_CONDITION(UCombatComponent, bIsMorV, COND_SimulatedOnly);

}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...



	//if (Character)
	//{
	//	Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	//	//if (Character->GetCameraComponent())
	//	//{
	//	//	DefaultFOV = Character->GetCameraComponent()->FieldOfView;
	//	//	CurrentFOV = DefaultFOV;
	//	//}

	//	if (Character->HasAuthority())
	//	{
	//		InitializeCarriedAmmo();
	//	}
	//	Character->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &ThisClass::OnPlayMontageNotifyBeginFunc);
	//}

	//Grenades = MaxGrenades;
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//if (!Character->HasAuthority() && Character->IsLocallyControlled())
	//UE_LOG(LogTemp, Display, TEXT("Key Type : %d"), Character->KeyType);

	/*if (Character && Character->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;

		SetHUDCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);

	}*/

	//UE_LOG(LogTemp, Display, TEXT("bFireButtonPressed : %d"), bFireButtonPressed);
	//UE_LOG(LogTemp, Display, TEXT("FireTimer : %d"), FireTimer.IsValid());
	//UE_LOG(LogTemp, Display, TEXT("CharacterState : %d"), CharacterState);

	//if (Character && !Character->HasAuthority() && Character->IsLocallyControlled())
	//{
	//	UE_LOG(LogTemp, Display, TEXT("bLocallyReloading : %d"), bLocallyReloading);
	//}
	//UE_LOG(LogTemp, Display, TEXT("usecontrollerrotationyaw : %d"), Character->bUseControllerRotationYaw);
}

//void UCombatComponent::OnPlayMontageNotifyBeginFunc(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
//{
	//UE_LOG(LogTemp, Display, TEXT("Combat : OnPlayMontageNotifyBeginFunc"));
	//if (TEXT("ReloadFinished") == NotifyName)
	//{
	//	//UE_LOG(LogTemp, Display, TEXT("ReloadFinished"));
	//	FinishReloading();
	//	return;
	//}
	//else if (TEXT("Shell") == NotifyName)
	//{
	//	ShotgunShellReload();
	//	return;
	//}
	//else if (TEXT("ThrowGrenadeFinished") == NotifyName)
	//{
	//	//UE_LOG(LogTemp, Display, TEXT("ThrowGrenadeFinished"));

	//	ThrowGrenadeFinished();
	//	return;
	//}
	//else if (TEXT("GrenadeLaunch") == NotifyName)
	//{
	//	DisableAttachedGrenade();
	//}
	//else if (TEXT("SwapAttachWeapon") == NotifyName)
	//{
	//	FinishSwapAttachWeapons();
	//}
	//else if (TEXT("SlideEnd") == NotifyName)
	//{
	//	Character->UnCrouch();
	//	bIsSliding = false;
	//}
	//else if (TEXT("DodgeEnd") == NotifyName)
	//{
	//	//bIsDodge = false;
	//	//Character->CombatState = ECombatState::Unoccupied;
	//	Character->UnCrouch();
	//}
	//else if (TEXT("VaultOrMantleEnd") == NotifyName)
	//{
	//	bIsMorV = false;
	//	Character->SetCombatState(ECombatState::Unoccupied);
	//	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	//	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//}

//}
//void UCombatComponent::SetAimingFunc(bool InbIsAiming)
//{
//	//FString Str = UEnum::GetDisplayValueAsText(Character->GetLocalRole()).ToString();
//	//UE_LOG(LogTemp, Display, TEXT("ServerSetAiming_Implementation : %s"), *Str);
//
//	bIsAiming = InbIsAiming;
//
//	if (bIsAiming)
//	{
//		//Character->GetCharacterMovement()->bOrientRotationToMovement = false;
//		//Character->bUseControllerRotationYaw = true;
//
//		if (SpeedBuff && Character->GetBuffComponent()->bIsBuffingSpeed)
//		{
//			Character->GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed + SpeedBuff->GetBaseSpeedBuff();
//			Character->GetCharacterMovement()->MaxWalkSpeedCrouched = AimCrouchSpeed + SpeedBuff->GetCrouchSpeedBuff();
//		}
//		else
//		{
//			Character->GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
//			Character->GetCharacterMovement()->MaxWalkSpeedCrouched = AimCrouchSpeed;
//		}
//
//
//		//SetMovementSpeed(AimWalkSpeed, AimCrouchSpeed);
//		CharacterState = ECharacterState::Combat;
//	}
//	else
//	{
//		//Character->GetCharacterMovement()->bOrientRotationToMovement = true;
//		//Character->bUseControllerRotationYaw = false;
//
//		if (SpeedBuff && Character->GetBuffComponent()->bIsBuffingSpeed)
//		{
//			Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed + SpeedBuff->GetBaseSpeedBuff();
//			Character->GetCharacterMovement()->MaxWalkSpeedCrouched = BaseCrouchSpeed + SpeedBuff->GetCrouchSpeedBuff();
//		}
//		else
//		{
//			Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
//			Character->GetCharacterMovement()->MaxWalkSpeedCrouched = BaseCrouchSpeed;
//		}
//
//
//		//SetMovementSpeed(BaseWalkSpeed, BaseCrouchSpeed);
//		CharacterState = ECharacterState::Equipped;
//	}
//}
//
//void UCombatComponent::SetAiming(bool InbIsAiming)
//{
//	//bIsAiming = InbIsAiming;
//	if (Character == nullptr || EquippedWeapon == nullptr) return;
//
//	if (Character->IsLocallyControlled()) bAimButtonPressed = InbIsAiming;
//
//	SetAimingFunc(InbIsAiming);
//
//	ServerSetAiming(InbIsAiming);
//
//	if (Character->IsLocallyControlled() && EquippedWeapon->GetWeaponType() == EWeaponType::SniperRifle)
//	{
//		Character->ShowSniperScopeWidget(InbIsAiming);
//	}
//}
//
//void UCombatComponent::OnRep_Aiming_Finished()
//{
//	if (Character && Character->IsLocallyControlled())
//	{
//		bIsAiming = bAimButtonPressed;
//		CharacterState = ECharacterState::Equipped;
//	}
//}
//
//void UCombatComponent::ServerSetAiming_Implementation(bool InbIsAiming)
//{
//	if (Character && !Character->HasAuthority()) return;
//
//	SetAimingFunc(InbIsAiming);
//}
//
//bool UCombatComponent::ServerSetAiming_Validate(bool InbIsAiming)
//{
//	//UE_LOG(LogTemp, Display, TEXT("Validation"));
//	return true;
//}

//bool UCombatComponent::ShouldSwapWeapons()
//{
//	return EquippedWeapon != nullptr && SecondaryWeapon != nullptr;
//}
//
//void UCombatComponent::FinishSwapAttachWeapons()
//{
//	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
//	AttachActorToRightHand(EquippedWeapon);
//	EquippedWeapon->SetHUDAmmo();
//	UpdateCarriedAmmo();
//	PlayEquipWeaponSound();
//
//	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
//	AttachActorToBackpack(SecondaryWeapon);
//}
//
//void UCombatComponent::EquipWeaponFunc()
//{
//	if (EquippedWeapon && Character)
//	{
//		//Character->GetCharacterMovement()->bOrientRotationToMovement = false;
//		//Character->bUseControllerRotationYaw = true;
//		//UE_LOG(LogTemp, Display, TEXT("Equipped!"));
//
//		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
//		//CharacterState = ECharacterState::Equipped;
//
//		AttachActorToRightHand(EquippedWeapon);
//		PlayEquipWeaponSound();
//
//		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->GetController()) : Controller;
//		if (Controller)
//		{
//			Controller->SetHUDWeaponType(EquippedWeapon->GetWeaponName());
//		}
//
//		EquippedWeapon->EnableCustomDepth(false);
//
//		EquippedWeapon->SetOwner(Character);
//		EquippedWeapon->SetHUDAmmo();
//		UpdateCarriedAmmo();
//		ReloadEmptyWeapon();
//	}
//
//}
//
//void UCombatComponent::EquipSecondaryFunc()
//{
//	if (SecondaryWeapon && Character)
//	{
//		//Character->GetCharacterMovement()->bOrientRotationToMovement = false;
//		//Character->bUseControllerRotationYaw = true;
//		//UE_LOG(LogTemp, Display, TEXT("Equipped!"));
//
//		SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
//		//CharacterState = ECharacterState::;
//
//		AttachActorToBackpack(SecondaryWeapon);
//		PlaySecondaryWeaponSound();
//		SecondaryWeapon->SetOwner(Character);
//
//		//Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->GetController()) : Controller;
//		//if (Controller)
//		//{
//		//	Controller->SetHUDWeaponType(SecondaryWeapon->GetWeaponType());
//		//}
//
//
//	}
//}
//
//void UCombatComponent::EquipWeapon(AWeapon_Gun* InWeapon)
//{
//
//
//	if (Character == nullptr || InWeapon == nullptr) return;
//	if (Character->CombatState != ECombatState::Unoccupied) return;
//
//	//UE_LOG(LogTemp, Display, TEXT("LocalRole : %s"), *UEnum::GetDisplayValueAsText(Character->GetLocalRole()).ToString());
//
//	if (InWeapon->GetWeaponType() == EWeaponType::Flag)
//	{
//
//		Character->Crouch();
//		bHoldingTheFlag = true;
//		InWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
//		InWeapon->SetOwner(Character);
//		AttachFlagToLeftHand(InWeapon);
//		Flag = InWeapon;
//		//Character->GetCharacterMovement()->bOrientRotationToMovement = true;
//		//Character->bUseControllerRotationYaw = false;
//	}
//	else
//	{
//		if (EquippedWeapon != nullptr && SecondaryWeapon == nullptr) // Only Equipped One Weapon
//		{
//			SecondaryWeapon = InWeapon;
//			EquipSecondaryFunc();
//		}
//		else // First Slot and Secondary Slot is all full
//		{
//			//UE_LOG(LogTemp, Display, TEXT("EquipWeapon"));
//			DropEquippedWeapon();
//			EquippedWeapon = InWeapon;
//			EquipWeaponFunc();
//		}
//
//		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
//		Character->bUseControllerRotationYaw = true;
//	}
//
//
//
//}
//
//void UCombatComponent::SwapWeapons()
//{
//	if (Character == nullptr || Character->CombatState != ECombatState::Unoccupied) return;
//
//	Character->PlaySwapMontage();
//	Character->bFinishedSwapping = false;
//	Character->CombatState = ECombatState::SwappingWeapon;
//
//	AWeapon_Gun* TempWeapon = EquippedWeapon;
//	EquippedWeapon = SecondaryWeapon;
//	SecondaryWeapon = TempWeapon;
//
//	ABlasterPlayerController* PC = Cast<ABlasterPlayerController>(Character->GetController());
//	if (PC)
//	{
//		PC->SetHUDWeaponType(EquippedWeapon->GetWeaponName());
//	}
//
//	if (SecondaryWeapon) SecondaryWeapon->EnableCustomDepth(false);
//}
//
//void UCombatComponent::OnRep_EquippedWeapon()
//{
//	EquipWeaponFunc();
//}
//
//void UCombatComponent::OnRep_SecondaryWeapon()
//{
//	EquipSecondaryFunc();
//
//}
//
//void UCombatComponent::FireButtonPressed(bool bPressed)
//{
//	if (EquippedWeapon == nullptr) return;
//	bFireButtonPressed = bPressed;
//	if (!EquippedWeapon->IsAutomatic() && bIsFiring) return;
//
//	if (CanFire() && Character && Character->IsAiming() && bFireButtonPressed && Character->IsLocallyControlled())
//	{
//		bIsFiring = true;
//
//		FHitResult HitResult;
//		TraceUnderCrosshairs(HitResult);
//
//
//		if (EquippedWeapon && bPressed)
//		{
//			CrosshairShootingFactor = 0.75f;
//
//			switch (EquippedWeapon->GetFireType())
//			{
//			case EFireType::EFT_HitScan:
//				FireHitScanWeapon(bPressed);
//				break;
//			case EFireType::EFT_Projectile:
//				FireProjectileWeapon(bPressed);
//				break;
//			case EFireType::EFT_Shotgun:
//				FireShotgun(bPressed);
//				break;
//			default:
//				break;
//			}
//		}
//
//		//UE_LOG(LogTemp, Display, TEXT("FireTimer : %d"), FireTimer.IsValid());
//
//		//FireTimer.Invalidate();
//		StartFireTimer();
//	}
//}
//
//void UCombatComponent::FireProjectileWeapon(bool bPressed)
//{
//	if (EquippedWeapon && Character)
//	{
//		HitTarget = EquippedWeapon->bUseScatter ? EquippedWeapon->TraceEndWithScatter(HitTarget) : HitTarget;
//
//		if (!Character->HasAuthority() && Character->IsLocallyControlled()) // Autonomous
//		{
//			LocalFire(bPressed, HitTarget);
//		}
//		ServerFire(bPressed, HitTarget, EquippedWeapon->GetFireDelay());
//
//	}
//}
//
//void UCombatComponent::FireHitScanWeapon(bool bPressed)
//{
//	if (EquippedWeapon && Character)
//	{
//		HitTarget = EquippedWeapon->bUseScatter ? EquippedWeapon->TraceEndWithScatter(HitTarget) : HitTarget;
//
//		ServerFire(bPressed, HitTarget, EquippedWeapon->GetFireDelay());
//
//		bool IsAutonomous = !Character->HasAuthority() && Character->IsLocallyControlled();
//		if (IsAutonomous)
//		{
//			LocalFire(bPressed, HitTarget);
//		}
//	}
//}
//
//void UCombatComponent::FireShotgun(bool bPressed)
//{
//	AShotgun* Shotgun = Cast<AShotgun>(EquippedWeapon);
//	if (Shotgun && Character)
//	{
//		TArray<FVector_NetQuantize> HitTargets;
//		Shotgun->ShotgunTraceEndWithScatter(HitTarget, HitTargets);
//		if (!Character->HasAuthority()) ShotgunLocalFire(HitTargets);
//		ServerShotgunFire(HitTargets, EquippedWeapon->GetFireDelay());
//	}
//
//
//}
//
//void UCombatComponent::ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets)
//{
//	AShotgun* Shotgun = Cast<AShotgun>(EquippedWeapon);
//	if (Shotgun == nullptr || Character == nullptr) return;
//	if (Character->CombatState == ECombatState::Reloading || Character->CombatState == ECombatState::Unoccupied)
//	{
//		//UE_LOG(LogTemp, Display, TEXT("ShotgunLocalFire"));
//		bLocallyReloading = false;
//		Character->PlayFireMontage(bIsAiming);
//		Shotgun->FireShotgun(TraceHitTargets);
//		Character->CombatState = ECombatState::Unoccupied;
//	}
//
//}
//
//void UCombatComponent::StartFireTimer()
//{
//	//UE_LOG(LogTemp, Display, TEXT("StartFireTimer"));
//
//	if (EquippedWeapon == nullptr || Character == nullptr) return;
//
//
//	Character->GetWorldTimerManager().SetTimer(FireTimer, this, &ThisClass::FireTimerFinished, EquippedWeapon->GetFireDelay());
//	//UE_LOG(LogTemp, Display, TEXT("FireTimer : %d"), FireTimer.IsValid());
//
//}
//
//void UCombatComponent::FireTimerFinished()
//{
//	if (EquippedWeapon == nullptr || Character->bDisableGameplay) return;
//
//	//UE_LOG(LogTemp, Display, TEXT("FireTimer : %d"), FireTimer.IsValid());
//
//
//	//bCanFire = true;
//
//	ReloadEmptyWeapon();
//	if (bFireButtonPressed && EquippedWeapon->IsAutomatic())
//	{
//		FireButtonPressed(true);
//	}
//	bIsFiring = false;
//
//
//	FireTimer.Invalidate();
//}
//
//void UCombatComponent::ServerFire_Implementation(bool bPressed, const FVector_NetQuantize& TraceHitTarget, float FireDelay)
//{
//	MulticastFire(bPressed, TraceHitTarget);
//}
//
//bool UCombatComponent::ServerFire_Validate(bool bPressed, const FVector_NetQuantize& TraceHitTarget, float FireDelay)
//{
//	if (EquippedWeapon)
//	{
//		bool bNearlyEqual = FMath::IsNearlyEqual(EquippedWeapon->GetFireDelay(), FireDelay, 0.001f);
//		return bNearlyEqual;
//	}
//
//	return true;
//}
//
//void UCombatComponent::MulticastFire_Implementation(bool bPressed, const FVector_NetQuantize& TraceHitTarget)
//{
//	//UE_LOG(LogTemp, Warning, TEXT("MulticastFire_Implementation"));
//	//if (Character && Character->IsLocallyControlled() && !Character->HasAuthority())
//	//{
//
//	//	FText Str = UEnum::GetDisplayValueAsText(Character->GetLocalRole());
//	//	UE_LOG(LogTemp, Error, TEXT("Should Autonomous : %s"), *Str.ToString());
//	//	return;
//	//}
//	//else if (Character && Character->IsLocallyControlled() && Character->HasAuthority())
//	//{
//	//	FText Str = UEnum::GetDisplayValueAsText(Character->GetLocalRole());
//	//	UE_LOG(LogTemp, Error, TEXT("Should Authority : %s"), *Str.ToString());
//	//	return;
//	//}
//	//else if (Character && !Character->IsLocallyControlled() && Character->HasAuthority())
//	//{
//	//	FText Str = UEnum::GetDisplayValueAsText(Character->GetLocalRole());
//	//	UE_LOG(LogTemp, Error, TEXT("Not Local, Should Authority : %s"), *Str.ToString());
//	//	return;
//	//}
//	//else if (Character && !Character->IsLocallyControlled() && !Character->HasAuthority())
//	//{
//	//	FText Str = UEnum::GetDisplayValueAsText(Character->GetLocalRole());
//	//	UE_LOG(LogTemp, Error, TEXT("Not Local, Should Proxy : %s"), *Str.ToString());
//	//	return;
//	//}
//
//	if (Character && Character->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
//	{
//		return;
//	}
//
//	LocalFire(bPressed, TraceHitTarget);
//
//
//}
//
//void UCombatComponent::LocalFire(bool bPressed, const FVector_NetQuantize& TraceHitTarget)
//{
//	FString RoleStr = UEnum::GetDisplayValueAsText(GetOwnerRole()).ToString();
//	//UE_LOG(LogTemp, Display, TEXT("TraceHitTarget : %f, %f, %f, %s"), TraceHitTarget.X, TraceHitTarget.Y, TraceHitTarget.Z, *RoleStr);
//	//UE_LOG(LogTemp, Warning, TEXT("%s : LocalFire"), *RoleStr);
//
//	if (EquippedWeapon == nullptr) return;
//	//if (Character && CombatState == ECombatState::Reloading && EquippedWeapon->GetWeaponType() == EWeaponType::Shotgun)
//	//{
//	//	Character->PlayFireMontage(bIsAiming);
//	//	EquippedWeapon->Fire(TraceHitTarget);
//	//	CombatState = ECombatState::Unoccupied;
//	//	return;
//	//}
//
//	if (Character && Character->CombatState == ECombatState::Unoccupied)
//	{
//		if (bPressed)
//		{
//			Character->PlayFireMontage(bIsAiming);
//			EquippedWeapon->Fire(TraceHitTarget);
//		}
//
//	}
//}
//
//void UCombatComponent::ServerShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
//{
//	MulticastShotgunFire(TraceHitTargets);
//}
//
//bool UCombatComponent::ServerShotgunFire_Validate(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
//{
//	if (EquippedWeapon)
//	{
//		bool bNearlyEqual = FMath::IsNearlyEqual(EquippedWeapon->GetFireDelay(), FireDelay, 0.001f);
//		return bNearlyEqual;
//	}
//
//	return true;
//}
//
//void UCombatComponent::MulticastShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets)
//{
//	if (Character && Character->IsLocallyControlled() && !Character->HasAuthority())
//	{
//		return;
//	}
//
//	ShotgunLocalFire(TraceHitTargets);
//}
//
//bool UCombatComponent::CanFire()
//{
//	//UE_LOG(LogTemp, Display, TEXT("bLocallyReloading : %d"), bLocallyReloading);
//	if (EquippedWeapon == nullptr) return false;
//
//
//	//UE_LOG(LogTemp, Display, TEXT("CombatState : %d"), CombatState);
//	//return !EquippedWeapon->IsEmpty() && !bCanFire && CombatState == ECombatState::Unoccupied;
//	if (!EquippedWeapon->IsEmpty() && Character->CombatState == ECombatState::Reloading && EquippedWeapon->GetWeaponType() == EWeaponType::Shotgun)
//	{
//
//	}
//	if (bLocallyReloading) return false;
//
//	return !EquippedWeapon->IsEmpty() && Character->CombatState == ECombatState::Unoccupied;
//}
//
//void UCombatComponent::PickupAmmo(EWeaponType InWeaponType, int32 AmmoAmount)
//{
//	if (CarriedAmmoMap.Contains(InWeaponType))
//	{
//		CarriedAmmoMap[InWeaponType] = FMath::Clamp(CarriedAmmoMap[InWeaponType] + AmmoAmount, 0, INT32_MAX);
//
//		UpdateCarriedAmmo();
//	}
//
//	if (EquippedWeapon && EquippedWeapon->IsEmpty() && EquippedWeapon->GetWeaponType() == InWeaponType)
//	{
//		Reload();
//	}
//}
//
//void UCombatComponent::InitializeCarriedAmmo()
//{
//	CarriedAmmoMap.Emplace(EWeaponType::AssaultRifle, StartingARAmmo);
//	CarriedAmmoMap.Emplace(EWeaponType::RocketLauncher, StartingRocketAmmo);
//	CarriedAmmoMap.Emplace(EWeaponType::Pistol, StartingPistolAmmo);
//	CarriedAmmoMap.Emplace(EWeaponType::SMG, StartingSMGAmmo);
//	CarriedAmmoMap.Emplace(EWeaponType::Shotgun, StartingShotgunAmmo);
//	CarriedAmmoMap.Emplace(EWeaponType::SniperRifle, StartingSniperAmmo);
//	CarriedAmmoMap.Emplace(EWeaponType::GrenadeLauncher, StartingGrenadeLauncherAmmo);
//
//}
//
//void UCombatComponent::OnRep_CarriedAmmo()
//{
//	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->GetController()) : Controller;
//	if (Controller)
//	{
//		Controller->SetHUDCarriedAmmo(CarriedAmmo);
//	}
//
//	bool bJumpToShoutgunEnd = Character->CombatState == ECombatState::Reloading && EquippedWeapon != nullptr && EquippedWeapon->GetWeaponType() == EWeaponType::Shotgun && CarriedAmmo == 0;
//	if (bJumpToShoutgunEnd)
//	{
//		JumpToShotgunEnd();
//	}
//}
//
//void UCombatComponent::UpdateAmmoValues()
//{
//	if (Character == nullptr || EquippedWeapon == nullptr) return;
//
//	int32 ReloadAmount = AmountToReload();
//
//	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
//	{
//		CarriedAmmoMap[EquippedWeapon->GetWeaponType()] -= ReloadAmount;
//		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
//		//UE_LOG(LogTemp, Display, TEXT("CarriedAmmo : %d"), CarriedAmmo);
//	}
//
//	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->GetController()) : Controller;
//	if (Controller)
//	{
//		Controller->SetHUDCarriedAmmo(CarriedAmmo);
//	}
//	EquippedWeapon->AddAmmo(ReloadAmount);
//}
//
//void UCombatComponent::UpdateShotgunAmmoValues()
//{
//	if (Character == nullptr || EquippedWeapon == nullptr) return;
//	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
//	{
//		CarriedAmmoMap[EquippedWeapon->GetWeaponType()] -= 1;
//		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
//	}
//	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->GetController()) : Controller;
//	if (Controller)
//	{
//		Controller->SetHUDCarriedAmmo(CarriedAmmo);
//	}
//	EquippedWeapon->AddAmmo(1);
//
//	if (EquippedWeapon->IsFull() || CarriedAmmo == 0)
//	{
//		// Jump to ShotgunEnd section
//		JumpToShotgunEnd();
//	}
//}
//
//void UCombatComponent::Reload()
//{
//	//UE_LOG(LogTemp, Display, TEXT("CombatState : %d"), CombatState);
//	if (CarriedAmmo > 0 && Character->CombatState == ECombatState::Unoccupied && !EquippedWeapon->IsFull() && !bLocallyReloading)
//	{
//		ServerReload();
//		HandleReload();
//		bLocallyReloading = true;
//	}
//}
//
//void UCombatComponent::HandleReload()
//{
//	if (Character == nullptr) return;
//	Character->CombatState = ECombatState::Reloading;
//	Character->PlayReloadMontage();
//}
//
//void UCombatComponent::ShotgunShellReload()
//{
//	if (Character && Character->HasAuthority())
//	{
//		UpdateShotgunAmmoValues();
//	}
//}
//
//void UCombatComponent::JumpToShotgunEnd()
//{
//
//	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
//	if (AnimInstance && Character->GetReloadMontage())
//	{
//		AnimInstance->Montage_JumpToSection(TEXT("ShotgunEnd"), Character->GetReloadMontage());
//	}
//}
//
//void UCombatComponent::ServerReload_Implementation()
//{
//	//UE_LOG(LogTemp, Display, TEXT("ServerReload_Implementation"));
//	if (Character == nullptr || EquippedWeapon == nullptr) return;
//	//UpdateAmmoValues();
//
//	if (!Character->IsLocallyControlled()) HandleReload();
//}
//
//void UCombatComponent::FinishReloading()
//{
//	if (Character == nullptr) return;
//
//	//FString Str = UEnum::GetDisplayValueAsText(Character->GetLocalRole()).ToString();
//	//UE_LOG(LogTemp, Display, TEXT("FinishReloading %s : %d"), *Str, bLocallyReloading);
//
//
//
//	bLocallyReloading = false;
//	if (true)
//	{
//		Character->CombatState = ECombatState::Unoccupied;
//		UpdateAmmoValues();
//	}
//}
//
//int32 UCombatComponent::AmountToReload()
//{
//	if (EquippedWeapon == nullptr) return 0;
//	int32 RoomInMag = EquippedWeapon->GetMagCapacity() - EquippedWeapon->GetAmmo();
//
//	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
//	{
//		int32 AmountCarried = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
//		int32 Least = FMath::Min(RoomInMag, AmountCarried);
//		return FMath::Clamp(RoomInMag, 0, Least); // 사실 클램프는 없어도 되는데 GetAmmo 가 MagCapacity 보다 클 경우를 대비한 거임. 즉 실수가 생겼을 시 예방책
//	}
//	return 0;
//}
//
//void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
//{
//	FVector2D ViewportSize;
//	if (GEngine && GEngine->GameViewport)
//	{
//		GEngine->GameViewport->GetViewportSize(ViewportSize);
//	}
//
//	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
//	FVector CrosshairWorldPosition;
//	FVector CrosshairWorldDirection;
//	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);
//
//	if (bScreenToWorld)
//	{
//		FVector Start = CrosshairWorldPosition;
//
//		if (Character)
//		{
//			float DistanceToCharacter = (Character->GetActorLocation() - Start).Size2D();
//			Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
//			//DrawDebugSphere(GetWorld(), Start, 5.f, 12, FColor::Red);
//		}
//
//		FVector End = Start + CrosshairWorldDirection * 80'000.f;
//		//FVector End = Start + CrosshairWorldDirection * 2147483647;
//
//		bool bIsHit = GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);
//		if (!bIsHit)
//		{
//			TraceHitResult.ImpactPoint = End;
//		}
//
//		if (TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UInteractWithCrosshairsInterface>())
//		{
//			HUDPackage.CrosshairsColor = FLinearColor::Red;
//		}
//		else
//		{
//			HUDPackage.CrosshairsColor = FLinearColor::White;
//		}
//	}
//}
//
//void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
//{
//	if (Character && Character->Controller)
//	{
//		// 이 방법의 장점은 매프레임마다 호출될 때 캐스팅을 할 필요가 없다는 것임
//		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
//
//		if (Controller)
//		{
//			HUD = HUD == nullptr ? Cast<ABlasterHUD>(Controller->GetHUD()) : HUD;
//
//			if (HUD)
//			{
//
//				if (EquippedWeapon)
//				{
//					HUDPackage.CrosshairCenter = EquippedWeapon->CrosshairCenter;
//					HUDPackage.CrosshairLeft = EquippedWeapon->CrosshairLeft;
//					HUDPackage.CrosshairRight = EquippedWeapon->CrosshairRight;
//					HUDPackage.CrosshairTop = EquippedWeapon->CrosshairTop;
//					HUDPackage.CrosshairBottom = EquippedWeapon->CrosshairBottom;
//				}
//				else
//				{
//					HUDPackage.CrosshairCenter = nullptr;
//					HUDPackage.CrosshairLeft = nullptr;
//					HUDPackage.CrosshairRight = nullptr;
//					HUDPackage.CrosshairTop = nullptr;
//					HUDPackage.CrosshairBottom = nullptr;
//				}
//
//				// Calculate crosshair spread
//				// [0, 600] -> [0, 1]
//				FVector2D WalkSpeedRange(0.f, Character->GetCharacterMovement()->MaxWalkSpeed);
//				FVector2D VelocityMultiplierRange(0.f, 1.f);
//				FVector Velocity = Character->GetVelocity();
//				Velocity.Z = 0.f;
//				CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size2D());
//
//				if (Character->GetCharacterMovement()->IsFalling())
//				{
//					CrosshairAirFactor = FMath::FInterpTo(CrosshairAirFactor, 2.25f, DeltaTime, 2.25f);
//				}
//				else
//				{
//					CrosshairAirFactor = FMath::FInterpTo(CrosshairAirFactor, 0.f, DeltaTime, 30.f);
//				}
//
//				if (bIsAiming)
//				{
//					CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, -0.58f, DeltaTime, 30.f);
//				}
//				else
//				{
//					CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 30.f);
//				}
//
//
//				CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 10.f);
//
//				HUDPackage.CrosshairSpread = 0.5f + CrosshairVelocityFactor + CrosshairAirFactor + CrosshairAimFactor + CrosshairShootingFactor;
//
//
//				HUD->SetHUDPackage(HUDPackage);
//			}
//		}
//	}
//}
//
//void UCombatComponent::InterpFOV(float DeltaTime)
//{
//	if (EquippedWeapon == nullptr) return;
//
//	if (bIsAiming)
//	{
//		CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetZoomedFOV(), DeltaTime, EquippedWeapon->GetZoomInterpSpeed());
//	}
//	else
//	{
//		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, EquippedWeapon->GetZoomInterpSpeed());
//	}
//
//	if (Character && Character->GetCameraComponent())
//	{
//		Character->GetCameraComponent()->SetFieldOfView(CurrentFOV);
//	}
//}
//
//void UCombatComponent::SetMovementSpeed(float WalkSpeed, float CrouchSpeed)
//{
//	if (Character == nullptr) return;
//
//	Character->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
//	Character->GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
//}
//
////void UCombatComponent::OnRep_CombatState()
////{
////	switch (CombatState)
////	{
////	case ECombatState::Unoccupied:
////		if (bFireButtonPressed)
////		{
////			FireButtonPressed(true);
////		}
////		break;
////	case ECombatState::Reloading:
////		if (Character && !Character->IsLocallyControlled()) HandleReload();
////		break;
////	case ECombatState::ThrowingGrenade:
////		//if (Character && !Character->IsLocallyControlled()) // Simulate Proxy 만 재생하게 혹시 모르니 조건문 걸은거임
////		//{
////		//	Character->PlayThrowGrenadeMontage();
////		//}
////		Character->PlayThrowGrenadeMontage();
////		ShowAttachedGrenade(true);
////
////		break;
////	case ECombatState::SwappingWeapon:
////		if (Character && !Character->IsLocallyControlled())
////		{
////			Character->PlaySwapMontage();
////		}
////		break;
////	case ECombatState::ECS_MAX:
////		break;
////	default:
////		break;
////	}
////}
//
//void UCombatComponent::ThrowGrenade()
//{
//	if (Character->CombatState != ECombatState::Unoccupied || CharacterState == ECharacterState::UnEquipped) return;
//	//CombatState = ECombatState::ThrowingGrenade;
//	//if (Character)
//	//{
//	//	Character->PlayThrowGrenadeMontage();
//	//}
//
//	//if (Character && !Character->HasAuthority()) // 그냥 혹시모르니 조건확인용임
//	//{
//	//	ServerThrowGrenade();
//	//}
//
//	//if (Character && Character->HasAuthority())
//	//{
//	//	Grenades = FMath::Clamp(Grenades - 1, 0, MaxGrenades);
//	//	UpdateHUDGrenades();
//	//}
//
//	ServerThrowGrenade();
//}
//
//void UCombatComponent::ThrowGrenadeFinished()
//{
//	//UE_LOG(LogTemp, Warning, TEXT("ThrowGrenadeFinished"));
//	Character->CombatState = ECombatState::Unoccupied;
//	AttachActorToRightHand(EquippedWeapon);
//}
//
//void UCombatComponent::DropEquippedWeapon()
//{
//	if (EquippedWeapon)
//	{
//		EquippedWeapon->Dropped();
//	}
//}
//
//void UCombatComponent::AttachActorToRightHand(AActor* ActorToAttach)
//{
//	if (Character == nullptr || Character->GetMesh() == nullptr || ActorToAttach == nullptr) return;
//	//UE_LOG(LogTemp, Display, TEXT("AttachActorToRightHand"));
//	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(TEXT("hand_rSocket"));
//	if (HandSocket)
//	{
//		HandSocket->AttachActor(ActorToAttach, Character->GetMesh()); // 자동으로 replicated 됨
//	}
//}
//
//void UCombatComponent::AttachActorToBackpack(AActor* ActorToAttach)
//{
//	if (Character == nullptr || Character->GetMesh() == nullptr || ActorToAttach == nullptr) return;
//	//UE_LOG(LogTemp, Display, TEXT("AttachActorToRightHand"));
//	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(TEXT("BackpackSocket"));
//	if (HandSocket)
//	{
//		HandSocket->AttachActor(ActorToAttach, Character->GetMesh()); // 자동으로 replicated 됨
//	}
//}
//
//void UCombatComponent::AttachActorToLeftHand(AActor* ActorToAttach)
//{
//	if (Character == nullptr || Character->GetMesh() == nullptr || ActorToAttach == nullptr || EquippedWeapon == nullptr) return;
//	//UE_LOG(LogTemp, Display, TEXT("AttachActorToRightHand"));
//	bool bUsePistolSocket = EquippedWeapon->GetWeaponType() == EWeaponType::Pistol || EquippedWeapon->GetWeaponType() == EWeaponType::SMG;
//	FName SocketName = bUsePistolSocket ? TEXT("hand_lSocket_Pistol") : TEXT("hand_lSocket");
//	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(SocketName);
//	if (HandSocket)
//	{
//		HandSocket->AttachActor(ActorToAttach, Character->GetMesh()); // 자동으로 replicated 됨
//	}
//}
//
//void UCombatComponent::AttachFlagToLeftHand(AWeapon_Gun* InFlag)
//{
//	if (Character == nullptr || Character->GetMesh() == nullptr || InFlag == nullptr) return;
//	//UE_LOG(LogTemp, Display, TEXT("AttachActorToRightHand"));
//	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(TEXT("FlagSocket"));
//	if (HandSocket)
//	{
//		HandSocket->AttachActor(InFlag, Character->GetMesh()); // 자동으로 replicated 됨
//	}
//}
//
//void UCombatComponent::ServerThrowGrenade_Implementation()
//{
//	if (Character == nullptr || Grenades == 0) return;
//	//UE_LOG(LogTemp, Display, TEXT("ServerThrowGrenade_Implementation"));
//	Character->CombatState = ECombatState::ThrowingGrenade;
//	ShowAttachedGrenade(true);
//	AttachActorToLeftHand(EquippedWeapon);
//	Character->PlayThrowGrenadeMontage();
//
//	Grenades = FMath::Clamp(Grenades - 1, 0, MaxGrenades);
//	UpdateHUDGrenades();
//}
//
//void UCombatComponent::UpdateCarriedAmmo()
//{
//	if (EquippedWeapon == nullptr) return;
//	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
//	{
//		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
//	}
//
//	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->GetController()) : Controller;
//	if (Controller)
//	{
//		Controller->SetHUDCarriedAmmo(CarriedAmmo);
//	}
//}
//
//void UCombatComponent::PlayEquipWeaponSound()
//{
//	if (Character && EquippedWeapon && EquippedWeapon->GetTakeSound())
//	{
//		UGameplayStatics::PlaySoundAtLocation(this, EquippedWeapon->GetTakeSound(), Character->GetActorLocation());
//	}
//}
//
//void UCombatComponent::PlaySecondaryWeaponSound()
//{
//	if (Character && SecondaryWeapon && SecondaryWeapon->GetTakeSound())
//	{
//		UGameplayStatics::PlaySoundAtLocation(this, SecondaryWeapon->GetTakeSound(), Character->GetActorLocation());
//	}
//}
//
//void UCombatComponent::ReloadEmptyWeapon()
//{
//	if (EquippedWeapon && EquippedWeapon->IsEmpty())
//	{
//		Reload();
//	}
//}
//
//void UCombatComponent::ShowAttachedGrenade(bool bShowGrenade)
//{
//	if (Character && Character->GetAttachedGrenade())
//	{
//		Character->GetAttachedGrenade()->SetVisibility(bShowGrenade);
//	}
//}
//
//void UCombatComponent::LaunchGrenade()
//{
//	if (Character && Character->IsLocallyControlled())
//	{
//		//UE_LOG(LogTemp, Display, TEXT("LaunchGrenade"));
//		ServerLaunchGrenade(HitTarget);
//
//	}
//}
//
//void UCombatComponent::UpdateHUDGrenades()
//{
//	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->GetController()) : Controller;
//	if (Controller)
//	{
//		Controller->SetHUDGrenades(Grenades);
//	}
//}
//
//void UCombatComponent::OnRep_Grenades()
//{
//	UpdateHUDGrenades();
//
//}
//
//void UCombatComponent::ServerLaunchGrenade_Implementation(const FVector_NetQuantize& Target)
//{
//	if (Character && GrenadeClass && Character->GetAttachedGrenade())
//	{
//		const FVector StartingLocation = Character->GetAttachedGrenade()->GetComponentLocation();
//		FVector ToTarget = Target - StartingLocation;
//		FActorSpawnParameters SpawnParams;
//		SpawnParams.Owner = Character;
//		SpawnParams.Instigator = Character;
//		UWorld* World = GetWorld();
//		if (World)
//		{
//			DrawDebugLine(World, StartingLocation, Target, FColor::Magenta, false, 5.f);
//			AProjectileGrenade* Grenade = World->SpawnActor<AProjectileGrenade>(GrenadeClass, StartingLocation, ToTarget.Rotation(), SpawnParams);
//
//		}
//	}
//}
//
//void UCombatComponent::DisableAttachedGrenade()
//{
//	ShowAttachedGrenade(false);
//	LaunchGrenade();
//}
//
//void UCombatComponent::OnRep_HoldingTheFlag()
//{
//	if (bHoldingTheFlag && Character && Character->IsLocallyControlled())
//	{
//
//		//UE_LOG(LogTemp, Display, TEXT("OnRep_HoldingTheFlag : %s"), *UEnum::GetDisplayValueAsText(Character->GetLocalRole()).ToString());
//		Character->Crouch();
//	}
//}
//
//void UCombatComponent::Slide()
//{
//	if (Character == nullptr) return;
//
//
//	if (Character->GetVelocity().Length() > 500.f && Character->CanJump())
//	{
//		if (!Character->HasAuthority()) SlideFunc();
//		ServerSlide();
//	}
//}
//
//void UCombatComponent::SlideFunc()
//{
//	Character->PlaySlideMontage();
//	Character->Crouch();
//}
//
//void UCombatComponent::ServerSlide_Implementation()
//{
//	SlideFunc();
//	bIsSliding = true;
//}
//
//void UCombatComponent::OnRep_Sliding()
//{
//	if (bIsSliding)
//	{
//		SlideFunc();
//	}
//}
//
//void UCombatComponent::Dodge()
//{
//	//UE_LOG(LogTemp, Display, TEXT("Dodge : CombatState : %s"), *UEnum::GetDisplayValueAsText(Character->CombatState).ToString());
//	if (Character == nullptr || Character->CombatState != ECombatState::Unoccupied) return;
//	if (Character->GetCharacterMovement()->IsFalling()) return;
//
//	Character->CombatState = ECombatState::Dodging;
//
//	if (!Character->HasAuthority())
//	{
//		DodgeFunc();
//	}
//	ServerDodge(Character->KeyType);
//
//
//}
//
//void UCombatComponent::DodgeFunc()
//{
//	FName SectionName;
//
//
//
//
//
//	switch (Character->KeyType)
//	{
//	case EKeyType::Fwd:
//		SectionName = TEXT("Front");
//		break;
//	case EKeyType::Bwd:
//		SectionName = TEXT("Back");
//		break;
//	case EKeyType::Left:
//		SectionName = TEXT("Left");
//		break;
//	case EKeyType::Right:
//		SectionName = TEXT("Right");
//		break;
//	default:
//		break;
//	}
//
//	Character->Crouch();
//	Character->PlayDodgeMontage(SectionName);
//}
//
//void UCombatComponent::ServerDodge_Implementation(EKeyType InKeyType)
//{
//	Character->KeyType = InKeyType;
//	DodgeFunc();
//	bIsDodge = true;
//}
//
//void UCombatComponent::OnRep_Dodge()
//{
//	if (bIsDodge)
//	{
//		DodgeFunc();
//	}
//}
//
//void UCombatComponent::VaultOrMantle()
//{
//	if (Character->CombatState != ECombatState::Unoccupied) return;
//	//UE_LOG(LogTemp, Display, TEXT("VaultOrMantle"));
//
//
//	if (!Character->HasAuthority()) VaultOrMantleFunc();
//	ServerVaultMantle();
//
//}
//
//void UCombatComponent::VaultOrMantleFunc()
//{
//	if (Character->MantleVaultComponent->GetInitialObjectLocation_C())
//	{
//		if (!Character->HasAuthority()) UE_LOG(LogTemp, Error, TEXT("VaultOrMantleFunc"));
//		Character->CombatState = ECombatState::VaultOrMantle;
//		Character->bUseControllerRotationYaw = false;
//	}
//}
//
//void UCombatComponent::ServerVaultMantle_Implementation()
//{
//	VaultOrMantleFunc();
//	bIsMorV = true;
//}
//
//void UCombatComponent::OnRep_VaultOrMantle()
//{
//	VaultOrMantleFunc();
//}
//
