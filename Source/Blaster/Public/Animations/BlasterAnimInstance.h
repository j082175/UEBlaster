// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Animation/AnimInstance.h"
#include "Animations/BaseAnimInstance.h"

// Enums
#include "BlasterTypes/TurningInPlace.h"
//#include "BlasterTypes/CharacterState.h"
//#include "BlasterTypes/CombatState.h"

#include "BlasterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UBlasterAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
public:
	UBlasterAnimInstance();
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//class ABlasterCharacter* BlasterCharacter;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//ETurningInPlace TurningInPlace;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//uint8 bRotateRootBone : 1;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//uint8 bDisableGameplay : 1;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//float AO_Yaw;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//float AO_Pitch;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlasterAnimInstance|Movement", meta = (AllowPrivateAccess = true))
	//uint8 bUseAimOffsets : 1;
};
