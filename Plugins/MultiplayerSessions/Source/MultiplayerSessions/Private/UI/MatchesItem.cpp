// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MatchesItem.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
//#include "Interfaces/CommonPCFuncInterface.h"

#include "MultiplayerSessionsSubsystem.h"
#include "FindSessionsCallbackProxy.h"
#include "Kismet/KismetStringLibrary.h"


UMatchesItem::UMatchesItem(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMatchesItem::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	JoinButton->OnClicked.AddUniqueDynamic(this, &ThisClass::JoinButtonClicked);

}

void UMatchesItem::SetOnlineSessionSearchResult(FOnlineSessionSearchResult InResult)
{
	SearchResult = InResult;

	for (const auto& i : InResult.Session.SessionSettings.Settings)
	{
		ServerNameText->SetText(FText::FromString(i.Key.ToString()));
	}


	FString ServerName = UKismetStringLibrary::GetSubstring(SearchResult.Session.OwningUserName, 0, 15);
	OwnerNameText->SetText(FText::FromString(ServerName));

	int32 CurrentPlayers = SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections;
	int32 MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;

	MaxPlayersText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentPlayers, MaxPlayers)));

	PingText->SetText(FText::FromString(FString::FromInt(SearchResult.PingInMs)));
}

void UMatchesItem::JoinSessionFinished(EOnJoinSessionCompleteResult::Type Result)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::FromInt(Result));
	}

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		JoinButton->SetIsEnabled(true);
	}
}

void UMatchesItem::JoinButtonClicked()
{
	JoinButton->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem.IsValid())
	{
		MultiplayerSessionsSubsystem->JoinSession(SearchResult);
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::JoinSessionFinished);
	}

	
	//if (ICommonPCFuncInterface* CommonPC = Cast<ICommonPCFuncInterface>(GetOwningPlayer()))
	//{
	//	CommonPC->IShowLoading();
	//}

}
