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

	if (MultiplayerSessionsSubsystem.IsValid())
	{
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionComplete.AddUObject(this, &ThisClass::FindSessionsFinished);
	}
}

void UMainMenu_Matches::ProceedResetTimer()
{
	if (!Handle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ThisClass::ProcessAfterSearching, 2.f);
	}
}

void UMainMenu_Matches::ProcessAfterSearching()
{
	CircularThrobber->SetVisibility(ESlateVisibility::Collapsed);
	RefreshButton->SetIsEnabled(true);
	WBP_Selector->SetIsEnabled(true);
	bIsSearching = false;

	if (!ScrollBox->HasAnyChildren())
	{
		NoServers_Text->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ScrollBox->SetVisibility(ESlateVisibility::Visible);
	}

	Handle.Invalidate();
}

void UMainMenu_Matches::FindSessionsFinished(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Display, TEXT("FindSessionsFinished, ResultNum : %d"), SessionResults.Num());

	if (bWasSuccessful && SessionResults.Num() > 0)
	{
		for (const FOnlineSessionSearchResult& SessionResult : SessionResults)
		{
			if (UMatchesItem* MatchesItem = CreateWidget<UMatchesItem>(this, MatchesItemClass))
			{
				ScrollBox->AddChild(MatchesItem);
				MatchesItem->SetOnlineSessionSearchResult(SessionResult);
			}
		}

	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("FindSessionFailed")));
		}
	}

	ProceedResetTimer();


	//ProcessAfterSearching();
}

void UMainMenu_Matches::Refresh()
{
	UE_LOG(LogTemp, Display, TEXT("Refresh, bIsSearching : %d"), bIsSearching);


	if (!bIsSearching)
	{
		bIsSearching = true;
		ScrollBox->ClearChildren();
		ScrollBox->SetVisibility(ESlateVisibility::Collapsed);
		WBP_Selector->SetIsEnabled(false);
		RefreshButton->SetIsEnabled(false);
		NoServers_Text->SetVisibility(ESlateVisibility::Collapsed);
		CircularThrobber->SetVisibility(ESlateVisibility::Visible);

		if (MultiplayerSessionsSubsystem.IsValid())
		{
			MultiplayerSessionsSubsystem->FindSessions(10000);
		}
	}
}
