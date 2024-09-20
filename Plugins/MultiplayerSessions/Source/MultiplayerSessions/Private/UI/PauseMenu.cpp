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
	//if (ICommonPCFuncInterface* CommonFunc = Cast<ICommonPCFuncInterface>(GetOwningPlayer()))
	//{
	//	CommonFunc->IShowLoading();
	//}

	UGameplayStatics::OpenLevel(this, *UEnum::GetDisplayValueAsText(EDefaultMaps::StartupMap_SciFi_Dynamic).ToString(), true, TEXT("listen"));

	if (UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem = GetGameInstance()->GetSubsystem<UMultiplayerSessionsSubsystem>())
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
}

void UPauseMenu::OnClickedQuit_Btn()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
