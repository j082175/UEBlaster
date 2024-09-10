// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlasterTypes/CharacterState.h"
#include "BlasterTypes/CombatState.h"
#include "Item/Pickable/Weapon/WeaponTypes.h"
#include "HUD/BlasterHUD.h"

#include "CombatComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	friend class ABlasterCharacter;
	friend class ACharacterBase;
	friend class AEnemyRange;

	UCombatComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//	FORCEINLINE int32 GetGrenades() const { return Grenades; }
//	FORCEINLINE int32 GetCarriedAmmo() const { return CarriedAmmo; }
//	FORCEINLINE class AWeapon_Gun* GetEquippedWeapon() const { return EquippedWeapon; }
//
//
//	// Dynamic Delegates
//private:
//	UFUNCTION()
//	virtual void OnPlayMontageNotifyBeginFunc(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
//
//private:
//	UPROPERTY()
//	class ACharacterBase* Character;
//
//	UPROPERTY()
//	class ABlasterPlayerController* Controller;
//	UPROPERTY()
//	class ABlasterHUD* HUD;
//
//	FVector HitTarget;
//
//
//
//public:
//	bool ShouldSwapWeapons();
//	void FinishSwapAttachWeapons();
//
//	void EquipWeapon(class AWeapon_Gun* InWeapon);
//private:
//	void EquipWeaponFunc();
//	void EquipSecondaryFunc();
//
//
//	void SwapWeapons();
//
//	UFUNCTION()
//	void OnRep_EquippedWeapon();
//
//	UFUNCTION()
//	void OnRep_SecondaryWeapon();
//
//	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon, VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = true))
//	TObjectPtr<class AWeapon_Gun> EquippedWeapon;
//
//	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon, VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = true))
//	TObjectPtr<class AWeapon_Gun> SecondaryWeapon;
//
//	UPROPERTY(Replicated, VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = true))
//	TObjectPtr<class AWeapon_Gun> Flag;
//
//
//	// Fire
//public:
//	void FireButtonPressed(bool bPressed);
//private:
//	void FireProjectileWeapon(bool bPressed);
//	void FireHitScanWeapon(bool bPressed);
//	void FireShotgun(bool bPressed);
//	void ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets);
//
//	void StartFireTimer();
//	void FireTimerFinished();
//
//	UFUNCTION(Server, Reliable, WithValidation)
//	void ServerFire(bool bPressed, const FVector_NetQuantize& TraceHitTarget, float FireDelay);
//
//	UFUNCTION(NetMulticast, Reliable)
//	void MulticastFire(bool bPressed, const FVector_NetQuantize& TraceHitTarget);
//	void LocalFire(bool bPressed, const FVector_NetQuantize& TraceHitTarget);
//
//	UFUNCTION(Server, Reliable, WithValidation)
//	void ServerShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay);
//
//	UFUNCTION(NetMulticast, Reliable)
//	void MulticastShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets);
//
//	bool CanFire();
//
//	FTimerHandle FireTimer;
//	bool bFireButtonPressed = false;
//	uint8 bIsFiring : 1 = false;
//
//	// CarriedAmmo
//public:
//	void PickupAmmo(EWeaponType InWeaponType, int32 AmmoAmount);
//private:
//	void InitializeCarriedAmmo();
//	UFUNCTION()
//	void OnRep_CarriedAmmo();
//
//	UFUNCTION()
//	void UpdateAmmoValues();
//	void UpdateShotgunAmmoValues();
//
//	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
//	int32 CarriedAmmo;
//
//	UPROPERTY(EditAnywhere)
//	int32 StartingARAmmo = 30;
//
//	UPROPERTY(EditAnywhere)
//	int32 StartingRocketAmmo = 0;
//
//	UPROPERTY(EditAnywhere)
//	int32 StartingPistolAmmo = 0;
//
//	UPROPERTY(EditAnywhere)
//	int32 StartingSMGAmmo = 0;
//
//	UPROPERTY(EditAnywhere)
//	int32 StartingShotgunAmmo = 0;
//
//	UPROPERTY(EditAnywhere)
//	int32 StartingSniperAmmo = 0;
//
//	UPROPERTY(EditAnywhere)
//	int32 StartingGrenadeLauncherAmmo = 0;
//
//	TMap<EWeaponType, MaxAmmoInt> CarriedAmmoMap;
//
//	// Reload
//public:
//	void JumpToShotgunEnd();
//private:
//	void Reload();
//	void HandleReload();
//	UFUNCTION(BlueprintCallable)
//	void ShotgunShellReload();
//
//	UFUNCTION(Server, Reliable)
//	void ServerReload();
//	void FinishReloading();
//	int32 AmountToReload();
//
//	bool bLocallyReloading = false;
//
//	// Crosshair & Zoom
//private:
//	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
//
//	void SetHUDCrosshairs(float DeltaTime);
//
//	void InterpFOV(float DeltaTime);
//
//	//float CrosshairVelocityFactor;
//	//float CrosshairAirFactor;
//	//float CrosshairAimFactor;
//	//float CrosshairShootingFactor;
//
//	//float DefaultFOV;
//	//float CurrentFOV;
//
//	//UPROPERTY(EditAnywhere, Category = "Combat")
//	//float ZoomedFOV = 30.f;
//
//	//UPROPERTY(EditAnywhere, Category = "Combat")
//	//float ZoomInterpSpeed = 20.f;
//	FHUDPackage HUDPackage;
//public:
//	void SetMovementSpeed(float WalkSpeed, float CrouchSpeed);
//private:
//
//	// Movement
//	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = true))
//	float BaseWalkSpeed = 600.f;
//
//	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = true))
//	float AimWalkSpeed = 450.f;
//
//	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = true))
//	float BaseCrouchSpeed = 300.f;
//
//	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = true))
//	float AimCrouchSpeed = 225.f;
//
//
//	// State
//private:
//	//UFUNCTION()
//	//void OnRep_CombatState();
//
//	//UPROPERTY(ReplicatedUsing = OnRep_CombatState)
//	//ECombatState CombatState = ECombatState::Unoccupied;
//
//	UPROPERTY(Replicated, VisibleAnywhere, Category = "Combat", meta = (AllowPrivateAccess = true))
//	ECharacterState CharacterState = ECharacterState::UnEquipped;
//
//	//bool bCanFire = false;
//
//private:
//	void ThrowGrenade();
//	UFUNCTION(Server, Reliable)
//	void ServerThrowGrenade();
//	UFUNCTION(BlueprintCallable)
//	void ThrowGrenadeFinished();
//
//private:
//	void DropEquippedWeapon();
//	void AttachActorToRightHand(AActor* ActorToAttach);
//	void AttachActorToBackpack(AActor* ActorToAttach);
//	void AttachActorToLeftHand(AActor* ActorToAttach);
//	void AttachFlagToLeftHand(AWeapon_Gun* Flag);
//
//	void UpdateCarriedAmmo();
//	void PlayEquipWeaponSound();
//	void PlaySecondaryWeaponSound();
//	void ReloadEmptyWeapon();
//	void ShowAttachedGrenade(bool bShowGrenade);
//	void LaunchGrenade();
//	UFUNCTION(Server, Reliable)
//	void ServerLaunchGrenade(const FVector_NetQuantize& Target);
//
//	void DisableAttachedGrenade();
//
//	void UpdateHUDGrenades();
//
//	UPROPERTY(EditAnywhere)
//	TSubclassOf<class AProjectileGrenade> GrenadeClass;
//
//	UFUNCTION()
//	void OnRep_Grenades();
//	UPROPERTY(ReplicatedUsing = OnRep_Grenades)
//	int32 Grenades = 0;
//	UPROPERTY(EditAnywhere)
//	int32 MaxGrenades = 4;
//
//	// Buff
//public:
//	UPROPERTY(VisibleAnywhere)
//	class ASpeedPickup* SpeedBuff;
//
//
//	// Flag
//private:
//	UFUNCTION()
//	void OnRep_HoldingTheFlag();
//
//	UPROPERTY(ReplicatedUsing = OnRep_HoldingTheFlag)
//	bool bHoldingTheFlag = false;
//
//
//
//
//
//	// State booleans
//private:
//	UPROPERTY(VisibleAnywhere);
//	uint8 bIsSprint : 1;
//
//
//	// Slide
//	void Slide();
//	void SlideFunc();
//	UFUNCTION(Server, Reliable)
//	void ServerSlide();
//
//	UFUNCTION()
//	void OnRep_Sliding();
//
//	UPROPERTY(ReplicatedUsing = OnRep_Sliding, VisibleAnywhere);
//	uint8 bIsSliding : 1 = false;
//
//
//
//	// Dodge
//	void Dodge();
//	void DodgeFunc();
//	UFUNCTION(Server, Reliable)
//	void ServerDodge(EKeyType InKeyType);
//
//	UFUNCTION()
//	void OnRep_Dodge();
//	UPROPERTY(ReplicatedUsing = OnRep_Dodge, VisibleAnywhere);
//	uint8 bIsDodge : 1 = false;
//
//	// MantleOrVault
//	void VaultOrMantle();
//	void VaultOrMantleFunc();
//	UFUNCTION(Server, Reliable)
//	void ServerVaultMantle();
//	UFUNCTION()
//	void OnRep_VaultOrMantle();
//
//	UPROPERTY(ReplicatedUsing = OnRep_VaultOrMantle, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
//	uint8 bIsMorV : 1;
};
