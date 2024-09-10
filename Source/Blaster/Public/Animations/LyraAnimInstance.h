// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BlasterTypes/MovementInput.h"
#include "BlasterTypes/LandState.h"
#include "BlasterTypes/TurningInPlace.h"
#include "BlasterTypes/AnimState.h"
#include "LyraAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ULyraAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UFUNCTION()
	void UpdateAcceleration();

	UFUNCTION()
	void WallDitection();

	UFUNCTION()
	void RunningIntoWall();


	//void TurnInPlace(float DeltaTime);
	//void SimProxiesTurn();
	//void AimOffset(float DeltaTime);
	//void AO_PitchMappingForClient();
	//void RotateInPlace(float DeltaTime);

	void TurnInPlace(float DeltaTime);
private:

	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class ABlasterCharacter> CharacterRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCharacterMovementComponent> CharacterMovement;

	// Movement

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	uint8 bShouldMove : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsFalling : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsCrouching : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsSprint : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsAiming : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	uint8 bIsJumping : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	FVector Acceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	EMovementInput MovementInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	ELandState LandState;

	// Orientation

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orientation Angle", meta = (AllowPrivateAccess = true))
	float F_OrientationAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orientation Angle", meta = (AllowPrivateAccess = true))
	float R_OrientationAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orientation Angle", meta = (AllowPrivateAccess = true))
	float B_OrientationAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orientation Angle", meta = (AllowPrivateAccess = true))
	float L_OrientationAngle;

	// World Acceleration

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Acceleration", meta = (AllowPrivateAccess = true))
	FVector WorldAcceleration2D;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Acceleration", meta = (AllowPrivateAccess = true))
	FRotator WorldRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Acceleration", meta = (AllowPrivateAccess = true))
	FVector LocalAcceleration2D;

	// World Velocity

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Velocity", meta = (AllowPrivateAccess = true))
	FVector WorldVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Velocity", meta = (AllowPrivateAccess = true))
	FVector WorldVelocity2D;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Velocity", meta = (AllowPrivateAccess = true))
	FVector LocalVelocity2D;

	// Acceleration

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Acceleration", meta = (AllowPrivateAccess = true))
	uint8 bHasAcceleration : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Acceleration", meta = (AllowPrivateAccess = true))
	uint8 bRunningIntoWall : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Acceleration", meta = (AllowPrivateAccess = true))
	uint8 bIsWall : 1;

	// Turn in Place
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Acceleration", meta = (AllowPrivateAccess = true))
	float AO_Yaw;
	float AO_Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState", meta = (AllowPrivateAccess = true))
	ETurningInPlace TurningInPlace = ETurningInPlace::NotTurning;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState", meta = (AllowPrivateAccess = true))
	EAnimState CharacterState = EAnimState::UnEquipped;

	float InterpAO_Yaw;
	FRotator StartingAimRotation;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotationCurrentFrame;
	float ProxyYawDelta;
	float TurnThreshold = 0.5f;
	bool bRotateRootBone;

	float TimeSinceLastMovementReplication;
	//UPROPERTY(Replicated)
	//uint8 bDisableGameplay : 1 = false; // Disable button about game play except rotate button.

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = true))
	float YawOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = true))
	float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = true))
	float RootYawOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = true))
	FRotator RotationCurrentFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = true))
	FRotator RotationLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = true))
	float LastDistanceCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = true))
	float DistanceCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = true))
	float DeltaDistanceCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = true))
	float AbsRootYawOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = true))
	float YawExcess;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "", meta = (AllowPrivateAccess = true))
	float Lean;
};
