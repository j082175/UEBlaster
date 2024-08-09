// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GolemAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UGolemAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsInAir : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsJumping : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsAccelerating : 1;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//uint8 bWeaponEquipped : 1;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//uint8 bIsCrouched : 1;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//uint8 bIsAiming : 1;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//uint8 bLocallyControlled : 1;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//uint8 bRotateRootBone : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsElimmed : 1;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//uint8 bUseFABRIK : 1;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//uint8 bUseAimOffsets : 1;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//uint8 bDisableGameplay : 1;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Character", meta = (AllowPrivateAccess = true))
	TObjectPtr<class AGolem> Owner;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Character", meta = (AllowPrivateAccess = true))
	//TObjectPtr<class AWeapon_Gun> EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	float Speed;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//FTransform LeftHandTransform;

	//FRotator CharacterRotationLastFrame;
	//FRotator CharacterRotation;
	//FRotator DeltaRotation;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//ETurningInPlace TurningInPlace;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//ECharacterState CharacterState;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//FRotator RightHandRotation;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//ECombatState CombatState;


	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	//uint8 bIsSprint : 1;
};
