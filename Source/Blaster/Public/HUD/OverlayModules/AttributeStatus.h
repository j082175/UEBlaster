// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Blueprint/UserWidget.h"
#include "HUD/MyUserWidget.h"
#include "AttributeStatus.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UAttributeStatus : public UMyUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//void SetHpBar(float InCurrentHp, float InMaxHp);
	//void SetShieldBar(float InCurrentShield, float InMaxShield);
	//void SetSpBar(float InCurrentSp, float InMaxSp);
	//void SetParryGaugeBar(float InCurrentParryGauge, float InMaxParryGauge);
	
	//void SetCurrentHp(float InCurrentHp);
	//void SetMaxHp(float InMaxHp);
	//void SetCurrentShield(float InCurrentShield);
	//void SetMaxShield(float InMaxShield);
	//void SetCurrentSp(float InCurrentSp);
	//void SetMaxSp(float InMaxSp);
	//void SetCurrentParryGauge(float InCurrentParryGauge);
	//void SetMaxParryGauge(float InMaxParryGauge);

	void SetProgressBar(class UProgressBar* InProgressBar, float InPercent);
	void SetCurrentTextBlock(class UTextBlock* InTextBlock, float InCurrentValue);
	void SetMaxTextBlock(class UTextBlock* InTextBlock, float InMaxValue);


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ShieldBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> SpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ParryGaugeBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CurrentHp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MaxHp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CurrentShield;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MaxShield;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CurrentSp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MaxSp;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UTextBlock> CurrentParryGauge;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UTextBlock> MaxParryGauge;
};
