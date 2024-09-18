// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Settings.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API USettings : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


	UFUNCTION()
	void OnNextClickedWindowMode();

	UFUNCTION()
	void OnPreviousClickedWindowMode();

	UFUNCTION()
	void OnNextClickedResolution();

	UFUNCTION()
	void OnPreviousClickedResolution();

	UFUNCTION()
	void OnNextClickedGraphics();

	UFUNCTION()
	void OnPreviousClickedGraphics();


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbySelector> WBP_Selector_WindowMode;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbySelector> WBP_Selector_Resolution;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbySelector> WBP_Selector_Graphics;

private:
	EWindowMode::Type GetWindowMode();
	FIntPoint GetResoultion();
	int32 GetGraphicsQuality();
	EWindowMode::Type ParseWindowMode(EWindowMode::Type InCurrentWindowMode);


	TWeakObjectPtr<UGameUserSettings> GameUserSettings;

	TArray<FIntPoint> ResolutionArr;
};
