// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MatchesItem.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMatchesItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMatchesItem(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	FORCEINLINE void SetOnlineSessionSearchResult(class FOnlineSessionSearchResult InResult) { SearchResult = InResult; }

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ServerNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MaxPlayersText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PingText;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> JoinButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> JoinButtonText;


private:
	void JoinSessionFinished(EOnJoinSessionCompleteResult::Type Result);

	void JoinButtonClicked();


	class FOnlineSessionSearchResult SearchResult;

	TWeakObjectPtr<class UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;
};
