// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Blueprint/UserWidget.h"
#include "HUD/MyUserWidget.h"
#include "HpBarWidget.generated.h"

/**
 *
 */
UCLASS()
class BLASTER_API UHpBarWidget : public UMyUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	FORCEINLINE class UProgressBar* GetHpBar() const { return HpBar; }
	FORCEINLINE class UProgressBar* GetShieldBar() const { return ShieldBar; }
	FORCEINLINE class UProgressBar* GetParryBar() const { return ParryGauge; }


	void SetHpBar(float InCurrent, float InMax);
	void SetShieldBar(float Percentage);
	void SetParryBar(float InCurrent, float InMax);

	void ParryGaugeAnimStart(bool InCheck);


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ShieldBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ParryGauge;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim), meta = (AllowPrivateAccess = true))
	class UWidgetAnimation* ParryGaugeAnimationStart;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim), meta = (AllowPrivateAccess = true))
	class UWidgetAnimation* ParryGaugeAnimationEnd;
};
