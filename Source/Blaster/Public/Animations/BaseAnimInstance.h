// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

// Enums
#include "BlasterTypes/TurningInPlace.h"
#include "BlasterTypes/CharacterState.h"
#include "BlasterTypes/CombatState.h"
#include "BlasterTypes/AnimState.h"

#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	class ACharacterBase* CharacterOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsInAir : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsJumping : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsAccelerating : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bWeaponEquipped : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsCrouched : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsAiming : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bLocallyControlled : 1;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsElimmed : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bUseFABRIK : 1;





	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bHoldingTheFlag : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Character", meta = (AllowPrivateAccess = true))
	TObjectPtr<class AWeapon_Gun> EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	float YawOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	float Lean;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	FTransform LeftHandTransform;

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	EAnimState AnimState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	FRotator RightHandRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	ECombatState CombatState;



	// Orientation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orientation Angle", meta = (AllowPrivateAccess = true))
	float F_OrientationAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orientation Angle", meta = (AllowPrivateAccess = true))
	float R_OrientationAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orientation Angle", meta = (AllowPrivateAccess = true))
	float B_OrientationAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orientation Angle", meta = (AllowPrivateAccess = true))
	float L_OrientationAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bIsSprint : 1;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	class ABlasterCharacter* BlasterCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	ETurningInPlace TurningInPlace;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bRotateRootBone : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bDisableGameplay : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	float AO_Yaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	float AO_Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bUseAimOffsets : 1;

	// Ragdoll
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsRagdollStateStopped : 1;
};
