// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponStatus.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UWeaponStatus : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetCurrentTextBlock(class UTextBlock* InTextBlock, int32 InCurrentValue);
	void SetMaxTextBlock(class UTextBlock* InTextBlock, int32 InMaxValue);
	void SetName(class UTextBlock* InTextBlock, const FString& InName);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CurrentAmmo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MaxAmmo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> WeaponName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GrenadeName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GrenadeNum;
};
