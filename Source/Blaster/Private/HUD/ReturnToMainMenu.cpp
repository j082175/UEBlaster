//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "HUD/ReturnToMainMenu.h"
//#include "GameFramework/PlayerController.h"
//#include "Components/Button.h"
//#include "GameFramework/GameModeBase.h"
//#include "Characters/BlasterCharacter.h"
//#include "MultiplayerSessionsSubsystem.h"
//
//void UReturnToMainMenu::MenuSetup()
//{
//	AddToViewport();
//	SetVisibility(ESlateVisibility::Visible);
//	SetIsFocusable(true);
//
//
//	UWorld* World = GetWorld();
//	if (World)
//	{
//		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
//		if (PlayerController)
//		{
//			FInputModeGameAndUI InputModeData;
//			InputModeData.SetWidgetToFocus(TakeWidget());
//			PlayerController->SetInputMode(InputModeData);
//			PlayerController->SetShowMouseCursor(true);
//		}
//	}
//
//	if (ReturnButton && !ReturnButton->OnClicked.IsBound())
//	{
//		ReturnButton->OnClicked.AddDynamic(this, &ThisClass::ReturnButtonClicked);
//	}
//
//	UGameInstance* GameInstance = GetGameInstance();
//	if (GameInstance)
//	{
//		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
//		if (MultiplayerSessionSubsystem)
//		{
//			MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
//		}
//	}
//}
//
//void UReturnToMainMenu::MenuTearDown()
//{
//	RemoveFromParent();
//
//	UWorld* World = GetWorld();
//	if (World)
//	{
//		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
//		if (PlayerController)
//		{
//			FInputModeGameOnly InputModeData;
//			PlayerController->SetInputMode(InputModeData);
//			PlayerController->SetShowMouseCursor(false);
//		}
//	}
//
//	if (ReturnButton && ReturnButton->OnClicked.IsBound())
//	{
//		ReturnButton->OnClicked.RemoveDynamic(this, &ThisClass::ReturnButtonClicked);
//	}
//	if (MultiplayerSessionSubsystem && MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
//	{
//		MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this, &ThisClass::OnDestroySession);
//	}
//}
//
//bool UReturnToMainMenu::Initialize()
//{
//	if (!Super::Initialize()) return false;
//
//	return true;
//}
//
//void UReturnToMainMenu::OnDestroySession(bool bWasSuccessful)
//{
//	if (!bWasSuccessful)
//	{
//		ReturnButton->SetIsEnabled(true);
//		return;
//	}
//
//	UWorld* World = GetWorld();
//	if (World)
//	{
//		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
//		if (GameMode)
//		{
//			GameMode->ReturnToMainMenuHost();
//		}
//		else
//		{
//			PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
//			if (PlayerController)
//			{
//				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
//			}
//		}
//	}
//}
//
//void UReturnToMainMenu::OnPlayerLeftGame()
//{
//	if (MultiplayerSessionSubsystem)
//	{
//		MultiplayerSessionSubsystem->DestroySession();
//	}
//}
//
//void UReturnToMainMenu::ReturnButtonClicked()
//{
//	ReturnButton->SetIsEnabled(false);
//
//	UWorld* World = GetWorld();
//	if (World)
//	{
//		APlayerController* FirstPlayerController = World->GetFirstPlayerController();
//		if (FirstPlayerController)
//		{
//			// each client have only one PlayerController
//			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FirstPlayerController->GetPawn());
//			if (BlasterCharacter)
//			{
//				BlasterCharacter->ServerLeaveGame();
//				BlasterCharacter->OnLeftGame.AddUniqueDynamic(this, &ThisClass::OnPlayerLeftGame);
//			}
//			else
//			{
//				ReturnButton->SetIsEnabled(true);
//			}
//		}
//	}
//}
//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "HUD/ReturnToMainMenu.h"
//#include "GameFramework/PlayerController.h"
//#include "Components/Button.h"
//#include "MultiplayerSessionsSubsystem.h"
//#include "GameFramework/GameModeBase.h"
//#include "Characters/BlasterCharacter.h"
//
//void UReturnToMainMenu::MenuSetup()
//{
//	AddToViewport();
//	SetVisibility(ESlateVisibility::Visible);
//	SetIsFocusable(true);
//
//
//	UWorld* World = GetWorld();
//	if (World)
//	{
//		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
//		if (PlayerController)
//		{
//			FInputModeGameAndUI InputModeData;
//			InputModeData.SetWidgetToFocus(TakeWidget());
//			PlayerController->SetInputMode(InputModeData);
//			PlayerController->SetShowMouseCursor(true);
//		}
//	}
//
//	if (ReturnButton && !ReturnButton->OnClicked.IsBound())
//	{
//		ReturnButton->OnClicked.AddDynamic(this, &ThisClass::ReturnButtonClicked);
//	}
//
//	UGameInstance* GameInstance = GetGameInstance();
//	if (GameInstance)
//	{
//		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
//		if (MultiplayerSessionSubsystem)
//		{
//			MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
//		}
//	}
//}
//
//void UReturnToMainMenu::MenuTearDown()
//{
//	RemoveFromParent();
//
//	UWorld* World = GetWorld();
//	if (World)
//	{
//		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
//		if (PlayerController)
//		{
//			FInputModeGameOnly InputModeData;
//			PlayerController->SetInputMode(InputModeData);
//			PlayerController->SetShowMouseCursor(false);
//		}
//	}
//
//	if (ReturnButton && ReturnButton->OnClicked.IsBound())
//	{
//		ReturnButton->OnClicked.RemoveDynamic(this, &ThisClass::ReturnButtonClicked);
//	}
//	if (MultiplayerSessionSubsystem && MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
//	{
//		MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this, &ThisClass::OnDestroySession);
//	}
//}
//
//bool UReturnToMainMenu::Initialize()
//{
//	if (!Super::Initialize()) return false;
//
//	return true;
//}
//
//void UReturnToMainMenu::OnDestroySession(bool bWasSuccessful)
//{
//	if (!bWasSuccessful)
//	{
//		ReturnButton->SetIsEnabled(true);
//		return;
//	}
//
//	UWorld* World = GetWorld();
//	if (World)
//	{
//		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
//		if (GameMode)
//		{
//			GameMode->ReturnToMainMenuHost();
//		}
//		else
//		{
//			PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
//			if (PlayerController)
//			{
//				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
//			}
//		}
//	}
//}
//
//void UReturnToMainMenu::OnPlayerLeftGame()
//{
//	if (MultiplayerSessionSubsystem)
//	{
//		MultiplayerSessionSubsystem->DestroySession();
//	}
//}
//
//void UReturnToMainMenu::ReturnButtonClicked()
//{
//	ReturnButton->SetIsEnabled(false);
//
//	UWorld* World = GetWorld();
//	if (World)
//	{
//		APlayerController* FirstPlayerController = World->GetFirstPlayerController();
//		if (FirstPlayerController)
//		{
//			// each client have only one PlayerController
//			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FirstPlayerController->GetPawn());
//			if (BlasterCharacter)
//			{
//				BlasterCharacter->ServerLeaveGame();
//				BlasterCharacter->OnLeftGame.AddUniqueDynamic(this, &ThisClass::OnPlayerLeftGame);
//			}
//			else
//			{
//				ReturnButton->SetIsEnabled(true);
//			}
//		}
//	}
//}
