// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

void UMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);

	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;

	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	if (GetWorld())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddUniqueDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionComplete.AddUObject(this, &ThisClass::OnFindSession);
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddUniqueDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddUniqueDynamic(this, &ThisClass::OnStartSession);

	}
}

bool UMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (HostButton)
	{
		HostButton->OnClicked.AddUniqueDynamic(this, &ThisClass::HostButtonClicked);
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddUniqueDynamic(this, &ThisClass::JoinButtonClicked);
	}

	return true;
}

void UMenu::NativeDestruct()
{
	MenuTearDown();
	Super::NativeDestruct();
}

void UMenu::OnCreateSession(bool bWasSuccessful)
{


	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("CreateSession Completed!"));
		}

		if (GetWorld())
		{
			GetWorld()->ServerTravel(PathToLobby);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("CreateSession Failed!"));
		}
		HostButton->SetIsEnabled(true);
	}
}

void UMenu::OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		return;
	}

	if (bWasSuccessful)
	{
		for (const auto& Result : SessionResults)
		{
			FString SettingsValue;
			Result.Session.SessionSettings.Get(TEXT("MatchType"), SettingsValue);
			if (SettingsValue == MatchType)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("SessionResults num : % d"), SessionResults.Num()));
				MultiplayerSessionsSubsystem->JoinSession(Result);
				return;
			}
		}
	}

	if (!bWasSuccessful || SessionResults.Num() > 0)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("FindSession Failed!"));
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("bWasSuccessful : %d, SessionResults num : %d"), bWasSuccessful, SessionResults.Num()));

		}
		JoinButton->SetIsEnabled(true);
	}

}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
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

void UMenu::OnDestroySession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("DestroySession Completed!"));
		}

		if (GetWorld())
		{
			GetWorld()->ServerTravel(PathToLobby);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("DestroySession Failed!"));
		}
	}
}

void UMenu::OnStartSession(bool bWasSuccessful)
{
}

void UMenu::HostButtonClicked()
{
	HostButton->SetIsEnabled(false);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("HostButtonClicked"));
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
		//if (GetWorld())
		//{
		//	GetWorld()->ServerTravel(TEXT("/Game/ThirdPerson/Maps/Lobby?listen"));
		//}
	}
}

void UMenu::JoinButtonClicked()
{
	JoinButton->SetIsEnabled(false);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("JoinButtonClicked"));
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(10000); // 480 포트를 쓰는놈들이 많기 때문에 넉넉하게 만개로 잡음.
	}
}

void UMenu::MenuTearDown()
{
	RemoveFromParent();
	if (GetWorld())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}
