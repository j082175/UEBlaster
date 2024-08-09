// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/LyraAnimInstance.h"
#include "Characters/BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void ULyraAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CharacterRef = Cast<ABlasterCharacter>(GetOwningActor());
	if (CharacterRef)
	{
		CharacterMovement = CharacterRef->GetCharacterMovement();
	}
}

void ULyraAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (!IsValid(CharacterRef) || !IsValid(CharacterMovement))
	{
		CharacterRef = Cast<ABlasterCharacter>(TryGetPawnOwner());
		return;
	}

	// Running into Wall
	UpdateAcceleration();
	WallDitection();
	RunningIntoWall();


	// 1.
	Velocity = CharacterMovement->Velocity;
	Velocity.Z = 0.f;

	GroundSpeed = Velocity.Size2D();

	// 2.
	Acceleration = CharacterMovement->GetCurrentAcceleration();

	if (GroundSpeed > 3.f && UKismetMathLibrary::NotEqual_VectorVector(Acceleration, FVector::Zero()))
	{
		bShouldMove = true;
	}

	// 3.
	bIsFalling = CharacterMovement->IsFalling();

	// 4. Calculate Direction
	const FVector Velocity2D = UKismetMathLibrary::MakeVector(Velocity.X, Velocity.Y, 0.f);
	const float Angle = UAnimInstance::CalculateDirection(Velocity2D, CharacterRef->GetActorRotation());
	Direction = UKismetMathLibrary::NormalizeAxis(Angle);

	// 5.



	if (UKismetMathLibrary::InRange_FloatFloat(Direction, -70.f, 70.f, true, true))
	{
		MovementInput = EMovementInput::EMI_Fwd;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(Direction, 70.f, 110.f, true, true))
	{
		MovementInput = EMovementInput::EMI_Right;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(Direction, -110.f, -70.f, true, true))
	{
		MovementInput = EMovementInput::EMI_Left;
	}
	else
	{
		MovementInput = EMovementInput::EMI_Bwd;
	}

	UE_LOG(LogTemp, Display, TEXT("MovementInput : %s"), *UEnum::GetDisplayValueAsText(MovementInput).ToString());

	// 6. Orientation

	F_OrientationAngle = Direction;
	R_OrientationAngle = Direction - 90.f;
	B_OrientationAngle = Direction - 180.f;
	L_OrientationAngle = Direction + 90.f;

	// 7. Turn In Place

	AO_Yaw = CharacterRef->GetAO_Yaw();
	AO_Pitch = CharacterRef->GetAO_Pitch();
	TurnInPlace(DeltaTime);

	// 8. Movement Mode
	bIsCrouching = CharacterRef->bIsCrouched;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = CharacterRef->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 3.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);
}

void ULyraAnimInstance::UpdateAcceleration()
{
	if (!IsValid(CharacterMovement) || !IsValid(CharacterRef)) return;
	// 1.

	float AccelerateValue = UKismetMathLibrary::VSizeXYSquared(LocalAcceleration2D);
	bHasAcceleration = !UKismetMathLibrary::NearlyEqual_FloatFloat(AccelerateValue, 0.f);

	// 2.

	WorldAcceleration2D = CharacterMovement->GetCurrentAcceleration();
	WorldAcceleration2D.Z = 0.f;

	LocalAcceleration2D = UKismetMathLibrary::LessLess_VectorRotator(WorldAcceleration2D, WorldRotation);
	WorldVelocity2D = WorldVelocity;
	WorldVelocity2D.Z = 0.f;

	LocalVelocity2D = UKismetMathLibrary::LessLess_VectorRotator(WorldVelocity2D, WorldRotation);
	// 3.

	WorldRotation = CharacterRef->GetActorRotation();
	WorldVelocity = CharacterRef->GetVelocity();
}

void ULyraAnimInstance::WallDitection()
{
	bool IsLocalAccelerate = LocalAcceleration2D.Length() > 0.f;
	bool IsLessThan200 = LocalVelocity2D.Length() < 200.f;

	float DotValue = FVector::DotProduct(LocalAcceleration2D.GetSafeNormal2D(), LocalVelocity2D.GetSafeNormal2D());
	bool IsInRange = UKismetMathLibrary::InRange_FloatFloat(DotValue, -0.6f, 0.6f, true, true);

	bRunningIntoWall = IsLocalAccelerate && IsLessThan200 && IsInRange;
}

void ULyraAnimInstance::RunningIntoWall()
{
	if (!IsValid(CharacterMovement)) return;


	if (bRunningIntoWall)
	{
		bIsWall = true;
		CharacterMovement->MaxWalkSpeed = 0.f;
	}
	else
	{
		bIsWall = false;
		if (bIsSprint)
		{
			CharacterMovement->MaxWalkSpeed = 600.f;
		}
		else
		{
			CharacterMovement->MaxWalkSpeed = 300.f;
		}
	}
}

void ULyraAnimInstance::TurnInPlace(float DeltaTime)
{
	if (bShouldMove || bIsFalling)
	{
		YawOffset = FMath::FInterpTo(YawOffset, 0.f, DeltaTime, 20.f);
		RotationCurrentFrame = CharacterRef->GetActorRotation();
		RotationLastFrame = RotationCurrentFrame;
	}
	else
	{
		RotationLastFrame = RotationCurrentFrame;
		RotationCurrentFrame = CharacterRef->GetActorRotation();

		FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(RotationCurrentFrame, RotationLastFrame);
		YawOffset -= DeltaRotation.Yaw;

		if (UAnimInstance::GetCurveValue(TEXT("IsTurn")) > 0.f)
		{
			LastDistanceCurve = DistanceCurve;
			DistanceCurve = GetCurveValue(TEXT("DistanceCurve"));

			DeltaDistanceCurve = DistanceCurve - LastDistanceCurve;

			if (RootYawOffset > 0.f)
			{
				RootYawOffset -= DeltaDistanceCurve;
			}
			else
			{
				RootYawOffset += DeltaDistanceCurve;
			}

			AbsRootYawOffset = FMath::Abs(RootYawOffset);

			if (AbsRootYawOffset > 45.f)
			{
				YawExcess = AbsRootYawOffset - 45.f;

				if (RootYawOffset > 0.f)
				{
					RootYawOffset -= YawExcess;
				}
				else
				{
					RootYawOffset += YawExcess;
				}
			}
		}
	}
}

//void ULyraAnimInstance::TurnInPlace(float DeltaTime)
//{
//	if (CharacterRef == nullptr) return;
//
//	float TurningThreshold = 45.f;
//
//	if (AO_Yaw > TurningThreshold) // Right Turning Threshold
//	{
//		TurningInPlace = ETurningInPlace::ETIP_TurningRight;
//	}
//	else if (AO_Yaw < -TurningThreshold) // Left Turning Threshold
//	{
//		TurningInPlace = ETurningInPlace::ETIP_TurningLeft;
//	}
//
//	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
//	{
//		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 6.f);
//		AO_Yaw = InterpAO_Yaw;
//		if (FMath::Abs(AO_Yaw) < 15.f)
//		{
//			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
//			StartingAimRotation = FRotator(0.f, CharacterRef->GetBaseAimRotation().Yaw, 0.f);
//		}
//	}
//}
//
//void ULyraAnimInstance::SimProxiesTurn()
//{
//	if (CombatComponent == nullptr || CombatComponent->EquippedWeapon == nullptr) return;
//
//	//bRotateRootBone = false;
//
//	float Speed = Velocity.Size2D();
//	if (Speed > 0.f)
//	{
//		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
//		return;
//	}
//
//
//	//AO_PitchMappingForClient();
//	ProxyRotationLastFrame = ProxyRotationCurrentFrame;
//	ProxyRotationCurrentFrame = CharacterRef->GetActorRotation();
//	ProxyYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotationCurrentFrame, ProxyRotationLastFrame).Yaw;
//
//	if (FMath::Abs(ProxyYawDelta) > TurnThreshold)
//	{
//		if (ProxyYawDelta > TurnThreshold)
//		{
//			TurningInPlace = ETurningInPlace::ETIP_TurningRight;
//		}
//		else if (ProxyYawDelta < -TurnThreshold)
//		{
//			TurningInPlace = ETurningInPlace::ETIP_TurningLeft;
//		}
//		else
//		{
//			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
//		}
//	}
//	else
//	{
//		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
//	}
//}
//
//// This function must be executed when character is idling. not moving.
//void ULyraAnimInstance::AimOffset(float DeltaTime)
//{
//	//if (CombatComponent && CombatComponent->EquippedWeapon == nullptr) return;
//	if (CharacterRef == nullptr) return;
//
//	float Speed = Velocity.Size2D();
//	bool bIsInAir = CharacterMovement->IsFalling();
//
//
//	if (CombatComponent && CombatComponent->CharacterState != ECharacterState::EAS_UnEquipped)
//	{
//		// Manage Left and Right
//		if (Speed <= 0.f && !bIsInAir)
//		{
//
//
//			bRotateRootBone = true;
//			FRotator CurrentAimRotation = FRotator(0.f, CharacterRef->GetBaseAimRotation().Yaw, 0.f);
//			FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
//
//			//UE_LOG(LogTemp, Display, TEXT("CurrentAimRotation : %f"), CurrentAimRotation.Yaw);
//			//UE_LOG(LogTemp, Display, TEXT("StartingAimRotation : %f"), StartingAimRotation.Yaw);
//			//UE_LOG(LogTemp, Display, TEXT("DeltaAimRotation : %f"), DeltaAimRotation.Yaw);
//
//			AO_Yaw = DeltaAimRotation.Yaw;
//			//if (IsAiming()) bUseControllerRotationYaw = false;
//
//			if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
//			{
//				InterpAO_Yaw = AO_Yaw;
//			}
//
//			CharacterRef->bUseControllerRotationYaw = true;
//			TurnInPlace(DeltaTime);
//		}
//
//		if (Speed > 0.f || bIsInAir)
//		{
//			if (CombatComponent->CharacterState == ECharacterState::EAS_Combat)
//			{
//				bRotateRootBone = false;
//			}
//			StartingAimRotation = FRotator(0.f, CharacterRef->GetBaseAimRotation().Yaw, 0.f);
//			AO_Yaw = 0.f;
//			//if (IsAiming()) bUseControllerRotationYaw = true;
//			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
//		}
//		//
//	}
//
//	AO_PitchMappingForClient();
//}
//
//void ULyraAnimInstance::AO_PitchMappingForClient()
//{
//	// 서버에서 아래 방향으로의 Pitch 는 음수로 시작하는 반면, 클라에서의 아래 방향으로의 Pitch 는 360도에서부터 내려가는 방식으로 시작하기 때문에, 이를 서버와 같은 방식으로 매핑해줄 필요가 있다.
//	// Server's pitch going to negative value when scrolling down, but client's pitch starts 360 degree. so we need to mapping client's value.
//
//	AO_Pitch = CharacterRef->GetBaseAimRotation().Pitch;
//	//UE_LOG(LogTemp, Display, TEXT("Pitch : %f"), AO_Pitch);
//	if (AO_Pitch > 90.f && !CharacterRef->IsLocallyControlled())
//	{
//		FVector2d InRange(270.f, 360.f);
//		FVector2d OutRange(-90.f, 0.f);
//		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
//	}
//}

//void ULyraAnimInstance::RotateInPlace(float DeltaTime)
//{
//	if (CombatComponent && CombatComponent->bHoldingTheFlag)
//	{
//		CharacterRef->bUseControllerRotationYaw = false;
//		CharacterMovement->bOrientRotationToMovement = true;
//		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
//		return;
//	}
//
//	if (CombatComponent && CombatComponent->EquippedWeapon)
//	{
//		CharacterMovement->bOrientRotationToMovement = false;
//		CharacterRef->bUseControllerRotationYaw = true;
//	}
//
//
//	if (bDisableGameplay)
//	{
//		CharacterRef->bUseControllerRotationYaw = false;
//		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
//		return;
//	}
//	if (CharacterRef->GetLocalRole() > ENetRole::ROLE_SimulatedProxy && CharacterRef->IsLocallyControlled())
//	{
//		AimOffset(DeltaTime);
//	}
//	else
//	{
//		TimeSinceLastMovementReplication += DeltaTime;
//		if (TimeSinceLastMovementReplication > 0.25f)
//		{
//			CharacterRef->OnRep_ReplicatedMovement();
//		}
//		AO_PitchMappingForClient();
//	}
//}