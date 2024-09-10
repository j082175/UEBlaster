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
	void StartAnimation(ESkillAnimType InPrefix, int32 InIndex, float InPlaybackSpeed);

	UFUNCTION(BlueprintCallable)
	void SetSkillCost(int32 InIndex, const FString& InStr);

	UFUNCTION(BlueprintCallable)
	void SetSoulCount(int32 InCount);

	UFUNCTION(BlueprintCallable)
	void SetSpBar(float InPercent);


	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;


	
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
	TObjectPtr<class UImage> SkillImage5;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SkillImage6;


	//UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	//TObjectPtr<class UWidgetAnimation> SkillCoolTimeAnim0;

	//UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	//TObjectPtr<class UWidgetAnimation> SkillCoolTimeAnim1;

	//UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	//TObjectPtr<class UWidgetAnimation> SkillCoolTimeAnim2;

	//UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	//TObjectPtr<class UWidgetAnimation> SkillCoolTimeAnim3;

	//UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	//TObjectPtr<class UWidgetAnimation> SkillCoolTimeAnim4;

	//UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	//TObjectPtr<class UWidgetAnimation> SkillCoolTimeAnim5;

	//UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	//TObjectPtr<class UWidgetAnimation> SkillCoolTimeAnim6;


	UPROPERTY(meta = (BindWidget), Transient)
	TObjectPtr<class UTextBlock> SkillCost0;

	UPROPERTY(meta = (BindWidget), Transient)
	TObjectPtr<class UTextBlock> SkillCost1;

	UPROPERTY(meta = (BindWidget), Transient)
	TObjectPtr<class UTextBlock> SkillCost2;

	UPROPERTY(meta = (BindWidget), Transient)
	TObjectPtr<class UTextBlock> SkillCost3;

	UPROPERTY(meta = (BindWidget), Transient)
	TObjectPtr<class UTextBlock> SkillCost4;

	UPROPERTY(meta = (BindWidget), Transient)
	TObjectPtr<class UTextBlock> SkillCost5;

	UPROPERTY(meta = (BindWidget), Transient)
	TObjectPtr<class UTextBlock> SkillCost6;

	UPROPERTY(meta = (BindWidget), Transient)
	TObjectPtr<class UTextBlock> SoulCount;

	UPROPERTY(meta = (BindWidget), Transient)
	TObjectPtr<class UProgressBar> SpBar;
private:
	TMap<FString, class UWidgetAnimation*> FindWidgetAnimation;
};

