// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MainMenu_Matches.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMainMenu_Matches : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainMenu_Matches(const FObjectInitializer& ObjectInitializer);
	void NativeConstruct() override;

	UFUNCTION()
	void Refresh();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbySelector> WBP_Selector;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> RefreshButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> RefreshButtonText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> ScrollBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> NoServers_Text;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCircularThrobber> CircularThrobber;

private:
	void ProcessAfterSearching();

	void FindSessionsFinished(const TArray<class FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);



	uint8 bIsSearching : 1;
	uint8 bUseLAN : 1;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UMatchesItem> MatchesItemClass;


	TWeakObjectPtr<class UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;

	TArray<FString> SelectorArr;
};
