// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Blueprint/UserWidget.h"
#include "HUD/MyUserWidget.h"
#include "CharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UCharacterOverlay : public UMyUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void Test(float DeltaTime);


public:
	void SetHpBar(float InCurrent, float InMax);
	

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> HealthText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ShieldBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ShieldText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> StaminaBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> StaminaText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ParryBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ScoreAmount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DefeatsAmount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> WeaponAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CarriedAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> WeaponType;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MatchCountdownText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GrenadesText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> RedTeamScore;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BlueTeamScore;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ScoreSpacerText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> HighPingImage;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> HighPingAnimation;
};
