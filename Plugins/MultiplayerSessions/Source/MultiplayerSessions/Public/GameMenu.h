// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UGameMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void MenuSetup();

private:
	UFUNCTION()
	void OnButtonClickedFunc();

	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> StartButton;

	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
};
