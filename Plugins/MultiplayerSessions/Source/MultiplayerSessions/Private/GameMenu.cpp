// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenu.h"
#include "Components/Button.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Enums/MapNames.h"

bool UGameMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (StartButton)
	{
		StartButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButtonClickedFunc);
	}

	AddToViewport();

	MenuSetup();

	return true;
}

void UGameMenu::MenuSetup()
{


	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem.IsValid())
	{
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddUniqueDynamic(this, &ThisClass::OnStartSession);
	}
}

void UGameMenu::OnButtonClickedFunc()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("StartButtonClicked"));
	}

	if (MultiplayerSessionsSubsystem.IsValid())
	{
		MultiplayerSessionsSubsystem->StartSession(); // 480 포트를 쓰는놈들이 많기 때문에 넉넉하게 만개로 잡음.
		StartButton->SetIsEnabled(false);
	}
}

void UGameMenu::OnStartSession(bool bWasSuccessful)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("OnStartSession"));
	}

	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("StartSession Completed!"));
		}

		if (GetWorld())
		{
			UGameInstance* GameInstance = GetGameInstance();
			UMultiplayerSessionsSubsystem* SubSystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();

			FString DesiredMap = SubSystem->DesiredMap;


			TArray<FFilePath> MapPath;

			for (size_t i = 0; i < static_cast<size_t>(ETeamMaps::MAX); i++)
			{
				FString Path = DEFAULT_GAMEMAP_PATH;
				Path.Append(*UEnum::GetDisplayValueAsText(static_cast<ETeamMaps>(i)).ToString());
				FFilePath FilePath{ Path };
				MapPath.Add(FilePath);
			}
			for (size_t i = 0; i < static_cast<size_t>(ECaptureTheFlagMaps::MAX); i++)
			{
				FString Path = DEFAULT_GAMEMAP_PATH;
				Path.Append(*UEnum::GetDisplayValueAsText(static_cast<ECaptureTheFlagMaps>(i)).ToString());
				FFilePath FilePath{ Path };
				MapPath.Add(FilePath);
			}

			for (size_t i = 0; i < MapPath.Num(); i++)
			{
				if (MapPath[i].FilePath.Contains(DesiredMap))
				{
					FString P = MapPath[i].FilePath.Append(TEXT("?listen"));
					GetWorld()->ServerTravel(P);

					return;
				}
			}

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("StartSession Failed! : No Maps"));
			}
			StartButton->SetIsEnabled(true);

			//GetWorld()->ServerTravel(TEXT("/Game/ThirdPerson/Maps/StartMap?listen"));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("StartSession Failed!"));
		}
		StartButton->SetIsEnabled(true);
	}
}
