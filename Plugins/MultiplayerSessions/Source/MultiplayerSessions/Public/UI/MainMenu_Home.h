// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/MapNames.h"
#include "OnlineSessionSettings.h"

#include "MainMenu_Home.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMainMenu_Home : public UUserWidget
{
	GENERATED_BODY()
public:
	UMainMenu_Home(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	UFUNCTION()
	void LeftArrowClicked_Map();

	UFUNCTION()
	void RightArrowClicked_Map();

	UFUNCTION()
	void LeftArrowClicked_LAN();

	UFUNCTION()
	void RightArrowClicked_LAN();

	UFUNCTION()
	void LeftArrowClicked_Players();

	UFUNCTION()
	void RightArrowClicked_Players();

	UFUNCTION()
	void CreateButtonClicked();

	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbySelector> WBP_Selector_Map;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbySelector> WBP_Selector_Players;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbySelector> WBP_Selector_LAN;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbyButton> WBP_Button_Create;

private:
	UFUNCTION()
	void OnCreateSessionFinished(bool bWasSuccessful);


	TArray<EMapNames> MapArr;
	TArray<int32> PlayerArr;

	int32 MapLength;


	int32 MaxPlayer = 8;

	EMapNames CurrentMap;
	uint8 bUseLAN : 1;
	uint32 CurrentPlayer;


	TWeakObjectPtr<class UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;




	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class ULoading> LoadingClass;

	UPROPERTY()
	TObjectPtr<class ULoading> LoadingWidget;
};
