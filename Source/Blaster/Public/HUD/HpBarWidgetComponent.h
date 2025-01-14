// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Components/WidgetComponent.h"
#include "HUD/MyWidgetComponent.h"
#include "HpBarWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UHpBarWidgetComponent : public UMyWidgetComponent
{
	GENERATED_BODY()
public:
	UHpBarWidgetComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	virtual void InitWidget() override;
	virtual void BeginPlay() override;

	void SetHpBar(float InCurrent, float InMax);
	void SetShieldBar(float InCurrent, float InMax);
	void SetParryBar(float InCurrentP, float InMaxP);

	void ParryGaugeAnimStart(bool InCheck);

	void ResetHpBarTimer();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UHpBarWidget> HpBarWidget;


	void CheckHpBarWidget(float DeltaTime);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHpBarVisible(bool InIsVisible);

	float HpCountdown;
};
