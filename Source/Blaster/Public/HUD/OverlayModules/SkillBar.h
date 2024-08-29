// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillBar.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API USkillBar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartAnimation(const FString& InPrefix, int32 InIndex, float InPlaybackSpeed);
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SkillImage0;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SkillImage1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SkillImage2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SkillImage3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SkillImage4;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ActiveImage0;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ActiveImage1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ActiveImage2;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> SkillCoolTimeAnim0;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> SkillCoolTimeAnim1;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> SkillCoolTimeAnim2;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> SkillCoolTimeAnim3;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> SkillCoolTimeAnim4;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> ActiveCoolTimeAnim0;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> ActiveCoolTimeAnim1;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> ActiveCoolTimeAnim2;

private:
	TMap<FString, class UWidgetAnimation*> FindWidgetAnimation;
};

