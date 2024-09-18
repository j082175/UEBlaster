// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BootScreen.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UBootScreen : public UUserWidget
{
	GENERATED_BODY()
	UBootScreen(const FObjectInitializer& ObjectInitializer);

	void NativeConstruct() override;

	void Travel();

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> FadeIn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class ULoading> LoadingClass;

	UPROPERTY()
	TObjectPtr<class ULoading> LoadingWidget;
};
