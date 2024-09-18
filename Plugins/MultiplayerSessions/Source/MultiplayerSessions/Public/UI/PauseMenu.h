// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
	UPauseMenu(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedContinue_Btn();

	UFUNCTION()
	void OnClickedLeave_Btn();

	UFUNCTION()
	void OnClickedQuit_Btn();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbyButton> Continue_Btn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbyButton> Leave_Btn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbyButton> Quit_Btn;
};
