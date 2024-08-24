// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/BaseAnimInstance.h"

// 
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
#include "Item/Pickable/Weapon/Weapon_Gun.h"
#include "BlasterTypes/CombatState.h"
#include "Net/UnrealNetwork.h"

#include "Characters/CharacterBase.h"

#include "Blaster/Blaster.h"

//void UBaseAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(ThisClass, bUseFABRIK);
//}

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	
	
	CharacterOwner = Cast<ACharacterBase>(TryGetPawnOwner());
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	//if (CharacterOwner) UE_LOG(LogTemp, Warning, TEXT("BaseAnimInstance tick, owner : %s"), *CharacterOwner->GetName());

	if (CharacterOwner == nullptr)
	{
		CharacterOwner = Cast<ACharacterBase>(TryGetPawnOwner());

	}
	else
	{


		if (CharacterOwner->GetCharacterMovement())
		{
			bIsInAir = CharacterOwner->GetCharacterMovement()->IsFalling();
			bIsAccelerating = CharacterOwner->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

		}

		Speed = CharacterOwner->GetVelocity().Size2D();
		bIsJumping = bIsInAir == true && CharacterOwner->GetVelocity().Z >= 0.f;
		bIsCrouched = CharacterOwner->bIsCrouched;
		bWeaponEquipped = CharacterOwner->IsWeaponEquipped();
		bIsAiming = CharacterOwner->IsAiming();
		EquippedWeapon = Cast<AWeapon_Gun>(CharacterOwner->GetEquippedWeapon());
		AnimState = CharacterOwner->GetAnimState();
		bIsElimmed = CharacterOwner->IsElimmed();
		CombatState = CharacterOwner->GetCombatState();
		bHoldingTheFlag = CharacterOwner->IsHoldingTheFlag();
		bIsSprint = CharacterOwner->IsSprint();
		bIsRagdollStateStopped = CharacterOwner->GetIsRagdollStateStopped();

		/* For Derived Class */

		TurningInPlace = CharacterOwner->GetTurningInPlace();
		bRotateRootBone = CharacterOwner->ShouldRotateRootBone();
		bDisableGameplay = CharacterOwner->GetDisableGameplay();
		AO_Yaw = CharacterOwner->GetAO_Yaw();
		AO_Pitch = CharacterOwner->GetAO_Pitch();

		//UE_LOG(LogTemp, Display, TEXT("AOPITCH : %f"), AO_Pitch);

		/*------------------------------------*/


		// Offset Yaw for Strafing
		FRotator AimRotation = CharacterOwner->GetBaseAimRotation();
		//FRotationMatrix::MakeFromX(CharacterOwner->GetVelocity());

		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(CharacterOwner->GetVelocity());
		//UE_LOG(LogTemp, Display, TEXT("X : %f, Y : %f, Z : %f"), CharacterOwner->GetVelocity().X, CharacterOwner->GetVelocity().Y, CharacterOwner->GetVelocity().Z);
		//UE_LOG(LogTemp, Display, TEXT("AimRotation - Yaw : %f, Pitch : %f, Roll : %f"), AimRotation.Yaw, AimRotation.Pitch, AimRotation.Roll);
		//UE_LOG(LogTemp, Display, TEXT("Movement - Yaw : %f, Pitch : %f, Roll : %f"), MovementRotation.Yaw, MovementRotation.Pitch, MovementRotation.Roll);
		//YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

		FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
		//UE_LOG(LogTemp, Display, TEXT("DeltaRot - Yaw : %f, Pitch : %f, Roll : %f"), DeltaRot.Yaw, DeltaRot.Pitch, DeltaRot.Roll);
		DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 15.f);
		YawOffset = DeltaRotation.Yaw;

		//if (!CharacterOwner->HasAuthority() && !CharacterOwner->IsLocallyControlled())
		//{
		//	UE_LOG(LogTemp, Display, TEXT("MovementRotation : %f"), MovementRotation.Yaw);
		//}

		CharacterRotationLastFrame = CharacterRotation;
		CharacterRotation = CharacterOwner->GetActorRotation();
		const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
		const float Target = Delta.Yaw / DeltaTime;
		const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 3.f);
		Lean = FMath::Clamp(Interp, -90.f, 90.f);



		if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && CharacterOwner->GetMesh())
		{
			// LeftHandSocket 소켓을 월드 공간으로 변환한 값을 저장
			LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName(TEXT("LeftHandSocket")), ERelativeTransformSpace::RTS_World);
			//LeftHandTransform.DebugPrint();


			//UE_LOG(LogTemp, Display, TEXT("Name : %s"), *Name.ToString());
			//LeftHandTransform.DebugPrint();

			FVector OutPosition;
			FRotator OutRotation;

			// 위에서 저장한 값을 캐릭터의 뼈 공간에서 hand_r 기준으로 변환
			CharacterOwner->GetMesh()->TransformToBoneSpace(FName(TEXT("hand_r")), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);

			LeftHandTransform.SetLocation(OutPosition);
			LeftHandTransform.SetRotation(FQuat(OutRotation));


			FTransform MuzzleTipTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(TEXT("MuzzleFlash"));

			if (CharacterOwner->IsLocallyControlled())
			{
				bLocallyControlled = true;
				//FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName(TEXT("hand_r")), ERelativeTransformSpace::RTS_World);

				//RightHandRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - CharacterOwner->GetHitTarget()));
				//RightHandRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleTipTransform.GetLocation(), CharacterOwner->GetHitTarget());

				FRotator LookAtRotation = UKismetMathLibrary::MakeRotFromX(-(CharacterOwner->GetHitTarget() - MuzzleTipTransform.GetLocation()));

				FRotator LookAtRotation2 = UKismetMathLibrary::MakeRotFromY(-(CharacterOwner->GetHitTarget() - MuzzleTipTransform.GetLocation()));

				//FVector ToHit = (CharacterOwner->GetHitTarget() - MuzzleTipTransform.GetLocation()).GetSafeNormal();
				//FVector ToHitSpine = (CharacterOwner->GetHitTarget() - RightHandTransform.GetLocation()).GetSafeNormal();

				//FRotator LookAtRotation3 = UKismetMathLibrary::MakeRotFromY(ToHit - ToHitSpine);

				RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 30.f);
				//RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation2, DeltaTime, 10.f);

			}




			// Rotation Matrix 으로부터 X축 벡터를 얻는 방법.
			FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));
			DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), MuzzleTipTransform.GetLocation() + MuzzleX * 1000.f, FColor::Red);
			DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), CharacterOwner->GetHitTarget(), FColor::Orange);


		}

		//bUseFABRIK = CharacterOwner->GetCombatState() != ECombatState::ECS_Reloading;
		//bool bFABRIKOverride = CharacterOwner->IsLocallyControlled() && CharacterOwner->GetCombatState() != ECombatState::ECS_ThrowingGrenade && CharacterOwner->bFinishedSwapping;
		//if (bFABRIKOverride)
		//{
		//	bUseFABRIK = !CharacterOwner->IsLocallyReloading();
		//}

		if (AnimState != EAnimState::EAS_UnEquipped)
		{
			bUseFABRIK = (int)CharacterOwner->GetCombatState() <= (int)ECombatState::ECS_Attacking;
		}

		//bUseFABRIK = (int)CharacterOwner->GetCombatState() <= (int)ECombatState::ECS_Attacking && AnimState == EAnimState::EAS_Combat || AnimState == EAnimState::EAS_Equipped;

		//UE_LOG(LogTemp, Display, TEXT("CombatState: %s"), *UEnum::GetDisplayValueAsText(CombatState).ToString());
		//UE_LOG(LogTemp, Display, TEXT("bUseFABRIK: %d"), bUseFABRIK);

		//UE_LOG(LogTemp, Display, TEXT("AnimState : %s"), *UEnum::GetDisplayValueAsText(AnimState).ToString());
		//UE_LOG(LogTemp, Display, TEXT("TurningInPlace : %s"), *UEnum::GetDisplayValueAsText(TurningInPlace).ToString());


		//if (CharacterOwner->HasAuthority())
		//PRINT_DEBUG_LOCALROLE(CharacterOwner);
		//if (!CharacterOwner->HasAuthority() && CharacterOwner->IsLocallyControlled()) UE_LOG(LogTemp, Display, TEXT("bUseFABRIK : %d"), bUseFABRIK);




		// Orientation
		if (CharacterOwner->GetCharacterMovement())
		{
			const FVector Velocity2D = UKismetMathLibrary::MakeVector(CharacterOwner->GetCharacterMovement()->Velocity.X, CharacterOwner->GetCharacterMovement()->Velocity.Y, 0.f);
			const float Angle = UKismetAnimationLibrary::CalculateDirection(Velocity2D, CharacterOwner->GetActorRotation());
			Direction = UKismetMathLibrary::NormalizeAxis(Angle);

			F_OrientationAngle = Direction;
			R_OrientationAngle = Direction - 90.f;
			B_OrientationAngle = Direction - 180.f;
			L_OrientationAngle = Direction + 90.f;
		}


		//UE_LOG(LogTemp, Display, TEXT("Ao_yaw : %f"), Lean);
	}
}

