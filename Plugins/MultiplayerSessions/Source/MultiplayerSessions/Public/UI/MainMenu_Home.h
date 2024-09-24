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
	void LeftArrowClicked_GameMode();

	UFUNCTION()
	void RightArrowClicked_GameMode();

	UFUNCTION()
	void LeftArrowClicked_Players();

	UFUNCTION()
	void RightArrowClicked_Players();

	UFUNCTION()
	void CreateButtonClicked();

	void CreateButtonFinished(const FString& InServerName);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> MapPreviewImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbySelector> WBP_Selector_Map;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbySelector> WBP_Selector_Players;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbySelector> WBP_Selector_GameMode;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbyButton> WBP_Button_Create;

private:
	UFUNCTION()
	void OnCreateSessionFinished(bool bWasSuccessful);

	void CreateSession();

	void SelectMap(bool IsRight);


	FString SelectType(EMatchTypes InCurrentMatchType);

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TArray<uint32> PlayerArr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	uint32 MaxPlayer = 2;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TMap<ETeamMaps, class UTexture2D*> TeamsMapPreviewImageMap;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TMap<ECaptureTheFlagMaps, class UTexture2D*> CTFMapPreviewImageMap;


	uint8 bUseLAN : 1;


	//EFreeForAllMaps CurrentFreeForAllMap;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	ETeamMaps CurrentTeamMap;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	ECaptureTheFlagMaps CurrentCaptureTheFlagMap;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	EDefaultMaps CurrentDefaultMap;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	EMatchTypes CurrentMatchType = EMatchTypes::Teams;

	TWeakObjectPtr<class UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class ULoading> LoadingClass;

	UPROPERTY()
	TObjectPtr<class ULoading> LoadingWidget;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UServerNameTextBox> ServerNameTextBoxClass;

	UPROPERTY()
	TObjectPtr<class UServerNameTextBox> ServerNameTextBox;
};
