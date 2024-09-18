// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MatchesItem.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
//#include "Interfaces/CommonPCFuncInterface.h"

#include "MultiplayerSessionsSubsystem.h"
#include "FindSessionsCallbackProxy.h"


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

	ServerNameText->SetText(FText::FromString(SearchResult.Session.OwningUserName));
	MaxPlayersText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections, SearchResult.Session.SessionSettings.NumPublicConnections)));
	PingText->SetText(FText::FromString(FString::FromInt(SearchResult.PingInMs)));


}

void UMatchesItem::JoinSessionFinished(EOnJoinSessionCompleteResult::Type Result)
{
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
