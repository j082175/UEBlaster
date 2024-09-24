// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenu.h"
#include "UI/LobbyButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

//#include "Interfaces/CommonPCFuncInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Enums/MapNames.h"
#include "MultiplayerSessionsSubsystem.h"

UPauseMenu::UPauseMenu(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Continue_Btn->Button->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClickedContinue_Btn);
	Leave_Btn->Button->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClickedLeave_Btn);
	Quit_Btn->Button->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClickedQuit_Btn);


	Continue_Btn->ButtonText->SetText(FText::FromString(TEXT("Continue")));
	Leave_Btn->ButtonText->SetText(FText::FromString(TEXT("Leave")));
	Quit_Btn->ButtonText->SetText(FText::FromString(TEXT("Quit")));


	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem.IsValid())
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddUniqueDynamic(this, &ThisClass::OnDestroySessionComplete);
		}

	}
}

void UPauseMenu::OnClickedContinue_Btn()
{
	FInputModeGameOnly InputModeGameOnly;
	GetOwningPlayer()->SetInputMode(InputModeGameOnly);
	GetOwningPlayer()->SetShowMouseCursor(false);
	RemoveFromParent();
}

void UPauseMenu::OnClickedLeave_Btn()
{
	if (MultiplayerSessionsSubsystem.IsValid())
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
}

void UPauseMenu::OnClickedQuit_Btn()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UPauseMenu::OnDestroySessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(this, *UEnum::GetDisplayValueAsText(EDefaultMaps::StartupMap_SciFi_Dynamic).ToString(), true, TEXT("listen"));
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete Failed")));
	}
}
