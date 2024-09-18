// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenu.h"
#include "Components/Button.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsSubsystem.h"

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

	return true;
}

void UGameMenu::MenuSetup()
{


	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
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

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->StartSession(); // 480 포트를 쓰는놈들이 많기 때문에 넉넉하게 만개로 잡음.
	}
}

void UGameMenu::OnStartSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("StartSession Completed!"));
		}

		if (GetWorld())
		{
			GetWorld()->ServerTravel(TEXT("/Game/ThirdPerson/Maps/StartMap?listen"));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("StartSession Failed!"));
		}
	}
}
