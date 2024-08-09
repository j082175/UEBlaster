// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MantleSystem_Demo.generated.h"

UENUM(BlueprintType)
enum class ECT_MantleHandType : uint8
{
	ECTMHT_LeftHand = 0 UMETA(DisplayName = "LeftHand"),
	ECTMAT_RightHand UMETA(DisplayName = "RightHand"),
	ECTMAT_MAX UMETA(DisplayName = "Default_MAX")

};

UENUM(BlueprintType)
enum class ECT_MantleAnimType : uint8
{
	ECTMAT_UE5_Manny = 0 UMETA(DisplayName = "UE5_Manny"),
	ECTMAT_UE5_Quinn UMETA(DisplayName = "UE5_Quinn"),
	ECTMAT_UE4_Mannequin UMETA(DisplayName = "UE4_Mannequin"),
	ECTMAT_UE4_ESRPG UMETA(DisplayName = "UE4_ESRPG"),

	EMT_MAX UMETA(DisplayName = "Default_MAX")
};

UENUM(BlueprintType)
enum class EMantleType : uint8
{
	EMT_HighMantle = 0 UMETA(DisplayName = "HighMantle"),
	EMT_LowMantle UMETA(DisplayName = "LowMantle"),
	EMT_FallingCatch UMETA(DisplayName = "FallingCatch"),
	EMT_MAX UMETA(DisplayName = "Default_MAX")
};

USTRUCT(BlueprintType)
struct FCT_Struct_ComponentAndTransform
{
	GENERATED_BODY()

	FTransform Transform;
	TObjectPtr<class UPrimitiveComponent> Component;
};

USTRUCT(BlueprintType)
struct FCT_Struct_MantleParams
{
	GENERATED_BODY()

	TObjectPtr<class UAnimMontage> AnimMontage;
	TObjectPtr<class UCurveVector> PositionCorrectionCurve;
	float StartingPosition;
	float PlayRate;
	FVector StartingOffset;
};

USTRUCT(BlueprintType)
struct FCT_Struct_MantleAsset
{
	GENERATED_BODY()

	TObjectPtr<class UAnimMontage> AnimMontage;
	TObjectPtr<class UCurveVector> PositionCorrectionCurve;
	FVector StartingOffset;
	float LowHeight;
	float LowPlayRate;
	float LowStartPosition;
	float HighHeight;
	float HighPlayRate;
	float HighStartPosition;
};

USTRUCT(BlueprintType)
struct FCT_Struct_MantleTraceSettings
{
	GENERATED_BODY()

};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLASTER_API UMantleSystem_Demo : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMantleSystem_Demo();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	void MantleUpdate(float BlendIn);
	void MantleEnd();
private:
	void MantleStart(EMantleType InMantleType, FCT_Struct_ComponentAndTransform MantleLedgeWS);
	void MantleCheck(FCT_Struct_MantleTraceSettings TraceSettings);

	FCT_Struct_MantleAsset GetMantleAsset(EMantleType InMantleType);

	UFUNCTION(Server, Reliable)
	void ServerSetMovementMode(EMovementMode MovementMode);


	UFUNCTION(Server, Reliable)
	void ServerPlayMontage(class UAnimMontage* MontageToPlay, float InPlayRate, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayMontage(class UAnimMontage* MontageToPlay, float InPlayRate, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages);


	FTransform TransformSum(const FTransform& T1, const FTransform& T2);

	void SetActorLocationAndRotation(FVector NewLocation, FRotator NewRotation, bool bSweep, bool bTeleport);
	UFUNCTION(Server, Reliable)
	void ServerSetActorLocationAndRotation(FVector NewLocation, FRotator NewRotation, bool bSweep, bool bTeleport);
	


	UPROPERTY(VisibleAnywhere, Category = "References", meta = (AllowPrivateAccess = true))
	TObjectPtr<class ABlasterCharacter> Character;

	UPROPERTY(VisibleAnywhere, Category = "References", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCharacterMovementComponent> CharacterMovement;

	UPROPERTY(VisibleAnywhere, Category = "References", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, Category = "References", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimInstance> MainAnimInstance;

	UPROPERTY(VisibleAnywhere, Category = "References", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UTimelineComponent> MantleTimeline;


	UPROPERTY(VisibleAnywhere, Category = "MantleSystem", meta = (AllowPrivateAccess = true))
	FCT_Struct_ComponentAndTransform MantleLedgeLS;
	UPROPERTY(VisibleAnywhere, Category = "MantleSystem", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleParams MantleParams;
	UPROPERTY(VisibleAnywhere, Category = "MantleSystem", meta = (AllowPrivateAccess = true))
	FTransform MantleAnimatedStartOffset;
	UPROPERTY(VisibleAnywhere, Category = "MantleSystem", meta = (AllowPrivateAccess = true))
	FTransform MantleActualStartOffset;
	UPROPERTY(VisibleAnywhere, Category = "MantleSystem", meta = (AllowPrivateAccess = true))
	FTransform MantleTarget;

	UPROPERTY(VisibleAnywhere, Category = "Anims", meta = (AllowPrivateAccess = true))
	ECT_MantleAnimType Mantle_AnimType;
	UPROPERTY(VisibleAnywhere, Category = "Anims", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset Custom_Mantle_2m;
	UPROPERTY(VisibleAnywhere, Category = "Anims", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset Custom_Mantle_1m_RH;
	UPROPERTY(VisibleAnywhere, Category = "Anims", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset Custom_Mantle_1m_LH;

	UPROPERTY(VisibleAnywhere, Category = "Configs", meta = (AllowPrivateAccess = true))
	ECT_MantleHandType HandType;
	UPROPERTY(VisibleAnywhere, Category = "Configs", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleTraceSettings FallingTraceSettings;
	UPROPERTY(VisibleAnywhere, Category = "Configs", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleTraceSettings GroundedTraceSettings;
	UPROPERTY(VisibleAnywhere, Category = "Configs", meta = (AllowPrivateAccess = true))
	//ETraceTypeQuery TraceChannel;
	//UPROPERTY(VisibleAnywhere, Category = "Configs", meta = (AllowPrivateAccess = true))
	uint8 FallingCatch : 1;
	//UPROPERTY(VisibleAnywhere, Category = "Configs", meta = (AllowPrivateAccess = true))
	//EDrawDebugTrace DebugType;
	UPROPERTY(VisibleAnywhere, Category = "Configs", meta = (AllowPrivateAccess = true))
	float Mantle_Z_Offset;

	UPROPERTY(VisibleAnywhere, Category = "Anims Default", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset UE5_Manny_Mantle_2m;
	UPROPERTY(VisibleAnywhere, Category = "Anims Default", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset UE5_Manny_Mantle_1m_RH;
	UPROPERTY(VisibleAnywhere, Category = "Anims Default", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset UE5_Manny_Mantle_1m_LH;
	UPROPERTY(VisibleAnywhere, Category = "Anims Default", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset UE5_Quinn_Mantle_2m;
	UPROPERTY(VisibleAnywhere, Category = "Anims Default", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset UE5_Quinn_Mantle_1m_RH;
	UPROPERTY(VisibleAnywhere, Category = "Anims Default", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset UE5_Quinn_Mantle_1m_LH;
	UPROPERTY(VisibleAnywhere, Category = "Anims Default", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset UE4_Mantle_2m;
	UPROPERTY(VisibleAnywhere, Category = "Anims Default", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset UE4_Mantle_1m_RH;
	UPROPERTY(VisibleAnywhere, Category = "Anims Default", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset UE4_Mantle_1m_LH;
	UPROPERTY(VisibleAnywhere, Category = "Anims Default", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset UE4_ESRPG_Mantle_2m;
	UPROPERTY(VisibleAnywhere, Category = "Anims Default", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset UE4_ESRPG_Mantle_1m_RH;
	UPROPERTY(VisibleAnywhere, Category = "Anims Default", meta = (AllowPrivateAccess = true))
	FCT_Struct_MantleAsset UE4_ESRPG_Mantle_1m_LH;

	// componenttags

	//uint8 ReplicateUsingRegisteredSubObjectList : 1;
	//uint8 ComponentReplicates : 1;
	//uint8 AutoActivate;
};
