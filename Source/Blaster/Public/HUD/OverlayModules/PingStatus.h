// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/MyUserWidget.h"
#include "PingStatus.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UPingStatus : public UMyUserWidget
{
	GENERATED_BODY()
public:
	void UpdatePing(float InPing);
	void HighPingWarning();
	void StopHighPingWarning();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> HighPingImage;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> HighPingAnimation;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PingText;
};
