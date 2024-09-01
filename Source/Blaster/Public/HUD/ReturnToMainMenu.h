// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD/MyUserWidget.h"
#include "ReturnToMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UReturnToMainMenu : public UMyUserWidget
{
	GENERATED_BODY()
public:
//	void MenuSetup();
//	void MenuTearDown();
//
//protected:
//	virtual bool Initialize() override;
//
//	UFUNCTION()
//	void OnDestroySession(bool bWasSuccessful);
//
//	UFUNCTION()
//	void OnPlayerLeftGame();
//
//private:
//	UFUNCTION()
//	void ReturnButtonClicked();

	UPROPERTY()
	class APlayerController* PlayerController;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ReturnButton;

	//UPROPERTY()
	//class UMultiplayerSessionsSubsystem* MultiplayerSessionSubsystem;
};
