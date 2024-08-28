// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class ACharacterBase;
public:	
	UBuffComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetIsHealing(bool InIsHealing) { bHealing = InIsHealing; }

	void SetInitialSpeeds(float BaseSpeed, float CrouchSpeed);
	void SetInitialJumpVelocity(float Velocity);

	UFUNCTION(BlueprintCallable)
	void Heal(float HealAmount, float HealingTime);

	UFUNCTION(BlueprintCallable)
	void ReplenishShield(float ShieldAmount, float ReplenishingTime);

	void BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime);
	void BuffJump(float BuffJumpVelocity, float BuffTime);

	class UAttributeComponent* OwnerAttribute;

	bool bIsBuffingSpeed = false;
protected:
	void HealRampUp(float DeltaTime);
	void ShieldRampUp(float DeltaTime);

	UPROPERTY()
	TObjectPtr<class ACharacterBase> Character;

	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHealing = false;
	float HealingRate = 0.f;
	float AmountToHeal = 0.f;

	// Shield
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bReplenishingShield = false;
	float ShieldReplenishRate = 0.f;
	float AmountToShield = 0.f;


	// Speed
	void ResetSpeeds();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpeedBuff(float BaseSpeed, float CrouchSpeed);
	FTimerHandle SpeedBuffTimer;
	float InitialBaseSpeed;
	float InitialCrouchSpeed;

	// Jump
	UFUNCTION(NetMulticast, Reliable)
	void MulticastJumpBuff(float JumpVelocity);
	void ResetJump();
	FTimerHandle JumpBuffTimer;
	float InitialJumpVelocity;


	// Buff
public:
	UPROPERTY(VisibleAnywhere)
	class ASpeedPickup* SpeedBuff;
};
