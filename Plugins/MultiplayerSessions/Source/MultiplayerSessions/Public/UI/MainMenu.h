// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Home_Btn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Matches_Btn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Character_Btn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Settings_Btn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> QuitGame_Btn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> Switcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UMainMenu_Home> WBP_Home;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UMainMenu_Matches> WBP_Matches;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UButton> WBP_Characters;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USettings> WBP_Settings;

private:
	UFUNCTION()
	void HomeBtnClicked();

	UFUNCTION()
	void MatchBtnClicked();

	UFUNCTION()
	void CharacterBtnClicked();

	UFUNCTION()
	void SettingsBtnClicked();

	UFUNCTION()
	void QuitBtnClicked();

	void SetActive();
};
