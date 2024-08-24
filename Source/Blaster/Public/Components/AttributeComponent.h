// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHpChangedDelegate, float/*, Percentage*/, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnShieldChangedDelegate, float/*, Percentage*/, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSpChangedDelegate, float/*, Percentage*/, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnParryGaugeChangedDelegate, float/*, Percentage*/, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnParryGaugeAnimDelegate, bool /*Start or End*/);



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLASTER_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttributeComponent();
	friend class ABlasterCharacter;
	class ACharacterBase* Owner;

	void Init();
	void InitStatus();

	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE float GetMaxHp() const { return MaxHp; }
	FORCEINLINE float GetCurrentShield() const { return CurrentShield; }
	FORCEINLINE float GetMaxShield() const { return MaxShield; }
	FORCEINLINE float GetCurrentSp() const { return CurrentSp; }
	FORCEINLINE float GetMaxSp() const { return MaxSp; }
	FORCEINLINE float GetCurrentParryGauge() const { return CurrentParryGauge; }
	FORCEINLINE float GetMaxParryGauge() const { return MaxParryGauge; }
	FORCEINLINE float GetGolds() const { return Golds; }
	FORCEINLINE float GetSouls() const { return Souls; }
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetDashCost() const { return DashCost; }
	FORCEINLINE float GetSpRecoveringRate() const { return SpRecoveringRate; }
	FORCEINLINE float GetParryGaugeIncrement() const { return ParryGaugeIncrement; }



	FORCEINLINE void SetCurrentHp(float InCurrentHp) { CurrentHp = InCurrentHp; }
	FORCEINLINE void SetMaxHp(float InMaxHp) { MaxHp = InMaxHp; }
	FORCEINLINE void SetCurrentShield(float InCurrentShield) { CurrentShield = InCurrentShield; }
	FORCEINLINE void SetMaxShield(float InMaxShield) { MaxShield = InMaxShield; }
	void SetCurrentSp(float InCurrentSp);

	FORCEINLINE void SetMaxSp(float InMaxSp) { MaxSp = InMaxSp; }
	void SetCurrentParryGauge(float InCurrentParryGauge);
	FORCEINLINE void SetMaxParryGauge(float InMaxParryGauge) { MaxParryGauge = InMaxParryGauge; }
	FORCEINLINE void SetSpRecoveringRate(float InSpRecoveringRate) { SpRecoveringRate = InSpRecoveringRate; }
	FORCEINLINE void SetParryGaugeIncrement(float InParryGaugeIncrement) { ParryGaugeIncrement = InParryGaugeIncrement; }


	FORCEINLINE void AddGolds(float InGolds) { Golds += InGolds; }
	FORCEINLINE void AddSouls(float InSouls) { Souls += InSouls; }

	FORCEINLINE float GetHpPercentage() const { return CurrentHp / MaxHp; }
	FORCEINLINE float GetSpPercentage() const { return CurrentSp / MaxSp; }


	// Delegates
	FOnHpChangedDelegate OnHpChanged;
	FOnShieldChangedDelegate OnShieldChanged;
	FOnSpChangedDelegate OnSpChanged;
	FOnParryGaugeChangedDelegate OnParryGaugeChanged;
	FOnParryGaugeAnimDelegate OnParryGaugeAnim;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


private:


	float RecoveringResourceRate(float CurrentVal, float MaxVal, float Rate, float InDeltaTime);
	float DecreaseResourceRate(float CurrentVal, float MaxVal, float Rate, float InDeltaTime);
	void CheckParryGaugeMaximum(float DeltaTime);
private:
	UFUNCTION()
	void OnRep_Health(float LastHealth);

	UPROPERTY(ReplicatedUsing = OnRep_Health, EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = true))
	float CurrentHp;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = true))
	float MaxHp = 100.f;

	UFUNCTION()
	void OnRep_Shield(float LastShield);

	UPROPERTY(ReplicatedUsing = OnRep_Shield, VisibleAnywhere, Category = "Player Stats")
	float CurrentShield = 100.f;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxShield = 100.f;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = true))
	float CurrentSp;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = true))
	float MaxSp = 100.f;

	UFUNCTION()
	void OnRep_ParryGauge(float LastParryGauge);

	UPROPERTY(ReplicatedUsing = OnRep_ParryGauge, VisibleAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = true))
	float CurrentParryGauge;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = true))
	float MaxParryGauge = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = true))
	float ParryGaugeCoolDown = 10.f;
	float ParryGaugeCoolDownCheck = 0.f;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = true))
	float Golds;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = true))
	float Souls;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = true))
	float DodgeCost = 20.f;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = true))
	float DashCost = 40.f;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = true))
	float SpRecoveringRate = 50.f;

	float RecoveringStartTimeCheck = 0.f;
	float RecoveringStartThreshold = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = true))
	float ParryGaugeIncrement = 100.f;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = true))
	float ParryGaugeDecreasingRate = 50.f;


	UFUNCTION()
	void OnRep_ParryGaugeAnim();
public:
	UPROPERTY(ReplicatedUsing = OnRep_ParryGaugeAnim)
	uint8 bIsParryGaugeAnimPlaying : 1;


	uint8 bHasSpChanged : 1;



	FTimerHandle H;
};
