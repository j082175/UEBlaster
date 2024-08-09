// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MantleSystem_Demo.h"
#include "Characters/BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Mantle/TimelineMantle.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Curves/CurveVector.h"
#include "Components/TimelineComponent.h"

// Sets default values for this component's properties
UMantleSystem_Demo::UMantleSystem_Demo()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMantleSystem_Demo::BeginPlay()
{
	Super::BeginPlay();

	// ...

	Character = Cast<ABlasterCharacter>(GetOwner());
	CharacterMovement = Character->GetCharacterMovement();
	CapsuleComponent = Character->GetCapsuleComponent();
	MainAnimInstance = Character->GetMesh()->GetAnimInstance();

	//GetWorld()->SpawnActor()
}


// Called every frame
void UMantleSystem_Demo::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMantleSystem_Demo::MantleUpdate(float BlendIn)
{
	// Step 1: Continually update the mantle target from the stored local transform to follow along with moving objects.

	//MantleTarget

	FVector Location = MantleLedgeLS.Transform.GetLocation();
	FRotator Rotation = MantleLedgeLS.Transform.GetRotation().Rotator();
	FVector Scale = MantleLedgeLS.Transform.GetScale3D();

	FTransform InvertedComponentTransform = MantleLedgeLS.Component->GetComponentTransform().Inverse();

	FVector ITL = UKismetMathLibrary::InverseTransformLocation(InvertedComponentTransform, Location);
	FRotator ITR = UKismetMathLibrary::InverseTransformRotation(InvertedComponentTransform, Rotation);
	FVector ITS = UKismetMathLibrary::InverseTransformLocation(InvertedComponentTransform, Scale);

	MantleTarget = FTransform(ITR, ITL, ITS);


	// Step 2: Update the Position and Correction Alphas using the Position/Correction curve set for each Mantle.

	FVector RV = MantleParams.PositionCorrectionCurve->GetVectorValue(MantleParams.StartingPosition + MantleTimeline->GetPlaybackPosition());
	
	float PositionAlpha = RV.X;
	float XYCorrectionAlpha = RV.Y;
	float ZCorrectionAlpha = RV.Z;

	// Step 3: Lerp multiple transforms together for independent control over the horizontal and vertical blend to the animated start position, as well as the target position.

	// Blend into the animated horizontal and rotation offset using the Y value of the Position/Correction Curve.
	FVector NewV = FVector(MantleAnimatedStartOffset.GetLocation().X, MantleAnimatedStartOffset.GetLocation().Y, MantleActualStartOffset.GetLocation().Z);

	FTransform T(MantleAnimatedStartOffset.GetRotation().Rotator(), NewV);
	FTransform NewT1 = UKismetMathLibrary::TLerp(MantleActualStartOffset, T, XYCorrectionAlpha);

	// Blend into the animated vertical offset using the Z value of the Position/Correction Curve.

	FVector NewV2 = FVector(MantleAnimatedStartOffset.GetLocation().X, MantleAnimatedStartOffset.GetLocation().Y, MantleActualStartOffset.GetLocation().Z);

	FTransform T2(MantleAnimatedStartOffset.GetRotation().Rotator(), NewV);
	FTransform NewT2 = UKismetMathLibrary::TLerp(MantleActualStartOffset, T, ZCorrectionAlpha);

	FVector SumV = FVector(NewT1.GetLocation().X, NewT1.GetLocation().Y, NewT2.GetLocation().Z);
	FTransform SumT(NewT1.GetRotation().Rotator(), SumV);

	// Blend from the currently blending transforms into the final mantle target using the X value of the Position/Correction Curve.

	FTransform NewT3 = TransformSum(MantleTarget, SumT);
	FTransform NewT4 = UKismetMathLibrary::TLerp(NewT3, MantleTarget, PositionAlpha);

	// Initial Blend In (controlled in the timeline curve) to allow the actor to blend into the Position/Correction curve at the midoint. This prevents pops when mantling an object lower than the animated mantle.
	FTransform NewT5 = TransformSum(MantleTarget, MantleActualStartOffset);

	FTransform LerpedTarget = UKismetMathLibrary::TLerp(NewT5, NewT4, BlendIn);

	// Step 4: Set the actors location and rotation to the Lerped Target.

	float Z = LerpedTarget.GetLocation().Z + Mantle_Z_Offset;
	FVector NewLoc = LerpedTarget.GetLocation();
	NewLoc.Z = Z;
	SetActorLocationAndRotation(NewLoc, LerpedTarget.GetRotation().Rotator(), false, false);
}

void UMantleSystem_Demo::MantleEnd()
{
	// Set the Character Movement Mode to Walking

	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
	ServerSetMovementMode(EMovementMode::MOVE_Walking);
}

void UMantleSystem_Demo::MantleStart(EMantleType InMantleType, FCT_Struct_ComponentAndTransform MantleLedgeWS)
{
	// Step 1: Get the Mantle Asset and use it to set the new Mantle Params.
	FCT_Struct_MantleAsset MantleAsset = GetMantleAsset(InMantleType);

	float MantleHeight = 0.f;

	MantleParams.AnimMontage = MantleAsset.AnimMontage;
	MantleParams.PositionCorrectionCurve = MantleAsset.PositionCorrectionCurve;
	MantleParams.StartingPosition = FMath::GetMappedRangeValueClamped(FVector2D(MantleAsset.LowHeight, MantleAsset.HighHeight), FVector2D(MantleAsset.LowStartPosition, MantleAsset.HighStartPosition), MantleHeight);
	MantleParams.PlayRate = FMath::GetMappedRangeValueClamped(FVector2D(MantleAsset.LowHeight, MantleAsset.HighHeight), FVector2D(MantleAsset.LowPlayRate, MantleAsset.HighPlayRate), MantleHeight);
	MantleParams.StartingOffset = MantleAsset.StartingOffset;

	// Step 2: Convert the world space target to the mantle component's local space for use in moving objects.
	MantleLedgeLS.Transform = MantleLedgeWS.Component->GetComponentTransform() * MantleLedgeWS.Component->GetComponentTransform().Inverse();
	MantleLedgeLS.Component = MantleLedgeWS.Component;
	
	// Step 3: Set the Mantle Target and calculate the Starting Offset (offset amount between the actor and target transform).
	MantleTarget = MantleLedgeWS.Transform;
	
	FVector V = Character->GetActorLocation() - MantleTarget.GetLocation();
	FRotator R = Character->GetActorRotation() - MantleTarget.GetRotation().Rotator();

	MantleActualStartOffset = FTransform(R, V);

	// Step 4: Calculate the Animated Start Offset from the Target Location. This would be the location the actual animation starts at relative to the Target Transform. 
	FVector Vec = MantleTarget.GetRotation().Vector() * MantleParams.StartingOffset.Y;
	Vec.Z = MantleParams.StartingOffset.Z;

	FVector ALocation = MantleTarget.GetLocation() - Vec;
	FRotator ARotation = MantleTarget.GetRotation().Rotator();

	MantleAnimatedStartOffset = FTransform(ARotation - MantleTarget.GetRotation().Rotator(), ALocation - MantleTarget.GetLocation());

	// Step 5: Clear the Character Movement Mode and set the Movement State to Mantling
	CharacterMovement->SetMovementMode(EMovementMode::MOVE_None);
	ServerSetMovementMode(EMovementMode::MOVE_None);

	// Step 6: Configure the Mantle Timeline so that it is the same length as the Lerp/Correction curve minus the starting position, and plays at the same speed as the animation. Then start the timeline.

	float MinTime = 0.f;
	float MaxTime = 0.f;
	MantleParams.PositionCorrectionCurve.Get()->GetTimeRange(MinTime, MaxTime);

	MantleTimeline->SetTimelineLength(MaxTime - MantleParams.StartingPosition);
	MantleTimeline->SetPlayRate(MantleParams.PlayRate);
	MantleTimeline->PlayFromStart();

	// Step 7: Play the Anim Montaget if valid.

	if (MantleParams.AnimMontage)
	{
		MainAnimInstance->Montage_Play(MantleParams.AnimMontage, MantleParams.PlayRate, EMontagePlayReturnType::MontageLength, MantleParams.StartingPosition);
		ServerPlayMontage(MantleParams.AnimMontage, MantleParams.PlayRate, EMontagePlayReturnType::MontageLength, MantleParams.StartingPosition, false);
	}
}

void UMantleSystem_Demo::MantleCheck(FCT_Struct_MantleTraceSettings TraceSettings)
{
	// Step 1: Trace forward to find a wall / object the character cannot walk on.


}

FCT_Struct_MantleAsset UMantleSystem_Demo::GetMantleAsset(EMantleType InMantleType)
{
	switch (InMantleType)
	{
	case EMantleType::EMT_HighMantle:
		goto Label1;
		break;
	case EMantleType::EMT_LowMantle:
		
		switch (Mantle_AnimType)
		{
		case ECT_MantleAnimType::ECTMAT_UE5_Manny:

			if (HandType == ECT_MantleHandType::ECTMHT_LeftHand) return UE5_Manny_Mantle_1m_LH;
			if (HandType == ECT_MantleHandType::ECTMAT_RightHand) return UE5_Manny_Mantle_1m_RH;

			break;
		case ECT_MantleAnimType::ECTMAT_UE5_Quinn:

			if (HandType == ECT_MantleHandType::ECTMHT_LeftHand) return UE5_Quinn_Mantle_1m_LH;
			if (HandType == ECT_MantleHandType::ECTMAT_RightHand) return UE5_Quinn_Mantle_1m_RH;

			break;
		case ECT_MantleAnimType::ECTMAT_UE4_Mannequin:

			if (HandType == ECT_MantleHandType::ECTMHT_LeftHand) return UE4_Mantle_1m_LH;
			if (HandType == ECT_MantleHandType::ECTMAT_RightHand) return UE4_Mantle_1m_RH;

			break;
		case ECT_MantleAnimType::ECTMAT_UE4_ESRPG:

			if (HandType == ECT_MantleHandType::ECTMHT_LeftHand) return UE4_ESRPG_Mantle_1m_LH;
			if (HandType == ECT_MantleHandType::ECTMAT_RightHand) return UE4_ESRPG_Mantle_1m_RH;

			break;
		case ECT_MantleAnimType::EMT_MAX:

			if (HandType == ECT_MantleHandType::ECTMHT_LeftHand) return Custom_Mantle_1m_LH;
			if (HandType == ECT_MantleHandType::ECTMAT_RightHand) return Custom_Mantle_1m_RH;

			break;
		default:
			break;
		}

		break;
	case EMantleType::EMT_FallingCatch:
		goto Label1;
		break;
	case EMantleType::EMT_MAX:
		break;
	default:
		break;
	}

Label1:
	switch (Mantle_AnimType)
	{
	case ECT_MantleAnimType::ECTMAT_UE5_Manny:
		return UE5_Manny_Mantle_2m;
		break;
	case ECT_MantleAnimType::ECTMAT_UE5_Quinn:
		return UE5_Quinn_Mantle_2m;
		break;
	case ECT_MantleAnimType::ECTMAT_UE4_Mannequin:
		return UE4_Mantle_2m;
		break;
	case ECT_MantleAnimType::ECTMAT_UE4_ESRPG:
		return UE4_ESRPG_Mantle_2m;
		break;
	case ECT_MantleAnimType::EMT_MAX:
		return Custom_Mantle_2m;
		break;
	default:
		break;
	}

	
	return FCT_Struct_MantleAsset();
}

void UMantleSystem_Demo::ServerPlayMontage_Implementation(class UAnimMontage* MontageToPlay, float InPlayRate, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages)
{
	MulticastPlayMontage(MontageToPlay, InPlayRate, ReturnValueType, InTimeToStartMontageAt, bStopAllMontages);
}

void UMantleSystem_Demo::MulticastPlayMontage_Implementation(class UAnimMontage* MontageToPlay, float InPlayRate, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages)
{
	if (Character == nullptr) return;
	Character->GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay, InPlayRate, ReturnValueType, InTimeToStartMontageAt, bStopAllMontages);
}

FTransform UMantleSystem_Demo::TransformSum(const FTransform& T1, const FTransform& T2)
{
	FVector Location = T1.GetLocation() + T2.GetLocation();
	FQuat Rotation = T1.GetRotation() + T2.GetRotation();

	return FTransform(Rotation, Location);
}

void UMantleSystem_Demo::SetActorLocationAndRotation(FVector NewLocation, FRotator NewRotation, bool bSweep, bool bTeleport)
{
	FHitResult OutHitResult;
	ServerSetActorLocationAndRotation(NewLocation, NewRotation, bSweep, bTeleport);
	Character->K2_SetActorLocationAndRotation(NewLocation, NewRotation, bSweep, OutHitResult, bTeleport);
}

void UMantleSystem_Demo::ServerSetActorLocationAndRotation_Implementation(FVector NewLocation, FRotator NewRotation, bool bSweep, bool bTeleport)
{
	FHitResult OutHitResult;
	Character->K2_SetActorLocationAndRotation(NewLocation, NewRotation, bSweep, OutHitResult, bTeleport);
}

void UMantleSystem_Demo::ServerSetMovementMode_Implementation(EMovementMode MovementMode)
{
	if (CharacterMovement == nullptr) return;
	CharacterMovement->SetMovementMode(MovementMode);
}

