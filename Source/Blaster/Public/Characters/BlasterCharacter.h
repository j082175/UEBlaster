// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
#include "Characters/CharacterBase.h"

// Input Struct
#include "InputActionValue.h"

// Interfaces
#include "Interfaces/OverlapItemInterface.h"
#include "Interfaces/InteractWithCrosshairsInterface.h"


// Enums
#include "BlasterTypes/TurningInPlace.h"
#include "BlasterTypes/AnimState.h"
#include "BlasterTypes/CombatState.h"
#include "BlasterTypes/Team.h"
#include "BlasterTypes/KeyType.h"

#include "Components/TimelineComponent.h"
#include "Components/LagCompensationComponent.h"

#include "BlasterCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacterBase, public IOverlapItemInterface, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlasterCharacter();
	friend class UCombatComponent;

	// Overrides
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	// Getters
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }
	FORCEINLINE class UOverheadWidget* GetOverheadWidget() const { return OverheadWidget; }

	FORCEINLINE float GetGrenades() const { return Grenades; }
	FORCEINLINE float GetCarriedAmmo() const { return CarriedAmmo; }

	// Setters
	FORCEINLINE void SetDisableGameplay(bool DisableGameplay) { bDisableGameplay = DisableGameplay; }


	// Scope UI
	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

private:
	virtual void PollInit() override;
	void OnPlayerStateInitialized();
	virtual void InitializeDefaults() override;
	virtual void InitializeCollisionStates() override;


	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

protected:
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser) override;
	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Interfaces
private:
	virtual void IGetItem(class AItem* InItem) override;


	// Inputs
public:
	void _Crouch();
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Jump() override;
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed(AWeapon* InItem);
	void Interact();

	void AimButtonToggle();
	void AimButtonPressed(const FInputActionValue& isPressed);
	void AimButtonReleased(const FInputActionValue& isPressed);

	void FireButtonPressed(const FInputActionValue& isPressed);
	void FireButtonReleased(const FInputActionValue& isPressed);

	UFUNCTION(Server, Reliable)
	void NotifyFireButtonPressed(bool IsFired);

	void ReloadButtonPressed();
	void ThrowGrenadeButtonPressed();

	void Sprint();
	void SlideButtonPressed();
	void DodgeButtonPressed();

	void VaultOrMantleButtonPressed();
	void DashButtonPressed();
	void MeleeButtonPressed();
	void TestingButtonPressed();

	uint8 bIsFirebuttonPressed : 1;
	uint8 bCheckIsSemi : 1;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_Crouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_Equip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_Aim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_Fire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_Reload;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_ThrowGrenade;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_Sprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_Slide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_Dodge;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_VaultOrMantle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_Dash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_Melee;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_TestingKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputMappingContext> IMC_Shoulder;




	// Components
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// Widgets
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UOverheadWidget> OverheadWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UWidgetComponent> OverheadWidgetComponent;


	UPROPERTY(Replicated)
	EKeyType KeyType;


	// For Calculate
private:

	void HideCameraIfCharacterClose();


private:
	//float InterpAO_Yaw;

	//UPROPERTY(EditAnywhere)
	//float CameraThreshold = 100.f;

	//bool bRotateRootBone;
	//float TurnThreshold = 0.5f;
	//FRotator ProxyRotationLastFrame;
	//FRotator ProxyRotationCurrentFrame;
	//float ProxyYawDelta;
	//float TimeSinceLastMovementReplication;

	//float AO_Yaw;
	//float AO_Pitch;
	//FRotator StartingAimRotation;

	// Elim
public:
	virtual void MulticastElim(bool bPlayerLeftGame) override;
private:
	virtual void ElimTimerFinished() override;


	// GameMode
public:
	UFUNCTION(Server, Reliable)
	void ServerLeaveGame();

	FOnLeftGame OnLeftGame;
private:

	//UPROPERTY(Replicated)
	//uint8 bDisableGameplay : 1 = false; // Disable button about game play except rotate button.

	uint8 bLeftGame = false;

	// Grenadee
private:
	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<class UStaticMeshComponent> AttachedGrenade;

public:
	virtual void MulticastGainedTheLead() override;

	virtual void MulticastLostTheLead() override;


	// Crosshair & Zoom
private:
	//void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

	void InterpFOV(float DeltaTime);

	float CrosshairVelocityFactor;
	float CrosshairAirFactor;
	float CrosshairAimFactor;
	float CrosshairCrouchFactor;

	float DefaultFOV;
	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomInterpSpeed = 20.f;

	FHUDPackage HUDPackage;

	virtual void SetAiming(bool InbIsAiming) override;

	virtual void FinishSwapAttachWeapons() override;
	virtual void EquipWeaponFunc() override;

	void EquipButtonFunc(AWeapon* InWeapon);

	UPROPERTY()
	class ABlasterHUD* HUD;

	// HUD
protected:
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);


	// Dash
	uint32 MoveButtonPressedCount;

	virtual void ThrowGrenadeFinished() override;
	virtual void UpdateAmmoValues() override;
	virtual void UpdateCarriedAmmo() override;



	// Test
	UFUNCTION(BlueprintCallable)
	void Test();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerTest();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastTest();


	uint8 Check : 1 = false;
};
