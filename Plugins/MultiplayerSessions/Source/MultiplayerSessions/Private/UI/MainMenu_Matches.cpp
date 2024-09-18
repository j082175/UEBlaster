// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu_Matches.h"
#include "UI/LobbySelector.h"
#include "UI/MatchesItem.h"

#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/CircularThrobber.h"

#include "MultiplayerSessionsSubsystem.h"

UMainMenu_Matches::UMainMenu_Matches(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UMatchesItem> MatchesItemRef(TEXT("/MultiplayerSessions/Widgets/MainMenu/Matches/WBP_Matches_Item.WBP_Matches_Item_C"));
	ensure(MatchesItemRef.Class);
	MatchesItemClass = MatchesItemRef.Class;

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	SelectorArr.Add(TEXT("Search Online"));
	SelectorArr.Add(TEXT("Search LAN"));
}

void UMainMenu_Matches::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshButton->OnClicked.AddUniqueDynamic(this, &ThisClass::Refresh);

	WBP_Selector->SetText(TEXT("Search Online"));
}

void UMainMenu_Matches::ProcessAfterSearching()
{
	CircularThrobber->SetVisibility(ESlateVisibility::Collapsed);
	RefreshButton->SetIsEnabled(true);
	WBP_Selector->SetIsEnabled(true);
	bIsSearching = false;
}

void UMainMenu_Matches::FindSessionsFinished(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("FindSessionsFinished"));
	if (bWasSuccessful)
	{
		for (const FOnlineSessionSearchResult& SessionResult : SessionResults)
		{
			if (UMatchesItem* MatchesItem = CreateWidget<UMatchesItem>(this, MatchesItemClass))
			{
				ScrollBox->AddChild(MatchesItem);
				MatchesItem->SetOnlineSessionSearchResult(SessionResult);
			}
		}

		if (SessionResults.Num() == 0)
		{
			NoServers_Text->SetVisibility(ESlateVisibility::Visible);
		}

	}
	else
	{
		NoServers_Text->SetVisibility(ESlateVisibility::Visible);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("FindSessionFailed")));
		}
	}

	ProcessAfterSearching();
}

//void UMainMenu_Matches::FindSessionsSuccess(const TArray<FBlueprintSessionResult>& Results)
//{
//	for (const auto& i : Results)
//	{
//		UMatchesItem* MatchesItem = CreateWidget<UMatchesItem>(this, MatchesItemClass);
//		ScrollBox->AddChild(MatchesItem);
//	}
//
//	if (Results.Num() == 0)
//	{
//		NoServers_Text->SetVisibility(ESlateVisibility::Visible);
//	}
//
//	ProcessAfterSearching();
//}
//
//void UMainMenu_Matches::FindSessionsFailure(const TArray<FBlueprintSessionResult>& Results)
//{
//	ProcessAfterSearching();
//
//	if (Results.Num() == 0)
//	{
//		NoServers_Text->SetVisibility(ESlateVisibility::Visible);
//	}
//}

void UMainMenu_Matches::Refresh()
{
	UE_LOG(LogTemp, Display, TEXT("Refresh"));
	if (!bIsSearching)
	{

		bIsSearching = true;
		ScrollBox->ClearChildren();
		WBP_Selector->SetIsEnabled(false);
		RefreshButton->SetIsEnabled(false);
		NoServers_Text->SetVisibility(ESlateVisibility::Collapsed);
		CircularThrobber->SetVisibility(ESlateVisibility::Visible);

		//TArray<FSessionsSearchSetting> Filters;
		//UFindSessionsCallbackProxyAdvanced* FindSessionsCallbackProxyAdvanced = UFindSessionsCallbackProxyAdvanced::FindSessionsAdvanced(this, GetOwningPlayer(), 6, bUseLAN, EBPServerPresenceSearchType::AllServers, Filters);

		//FindSessionsCallbackProxyAdvanced->OnSuccess.AddUniqueDynamic(this, &ThisClass::FindSessionsSuccess);
		//FindSessionsCallbackProxyAdvanced->OnFailure.AddUniqueDynamic(this, &ThisClass::FindSessionsFailure);

		if (MultiplayerSessionsSubsystem.IsValid())
		{
			MultiplayerSessionsSubsystem->FindSessions(6);
			MultiplayerSessionsSubsystem->MultiplayerOnFindSessionComplete.AddUObject(this, &ThisClass::FindSessionsFinished);

		}
	}
}
