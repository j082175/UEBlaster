// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BasePlayerController.h"
#include "Blueprint/UserWidget.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ABasePlayerController::ABasePlayerController()
{
	//static ConstructorHelpers::FClassFinder<UUserWidget> WBP_LoadingClassRef(TEXT("/Game/A_Blaster/Blueprints/HUD/LobbyWidgets/WBP_Loading.WBP_Loading_C"));
	//ensure(WBP_LoadingClassRef.Class);
	//WBP_LoadingClass = WBP_LoadingClassRef.Class;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_QuitActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/A_Blaster/Inputs/IA_Escape.IA_Escape'"));
	ensure(IA_QuitActionRef.Object);
	IA_QuitAction = IA_QuitActionRef.Object;

	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_PauseMenuClassRef(TEXT("/MultiplayerSessions/Widgets/PauseMenu/WBP_PauseMenu.WBP_PauseMenu_C"));
	ensure(WBP_PauseMenuClassRef.Class);
	WBP_PauseMenuClass = WBP_PauseMenuClassRef.Class;
}

void ABasePlayerController::IShowLoading()
{
	if (IsLocalPlayerController())
	{
		if (UUserWidget* Loading = CreateWidget<UUserWidget>(this, WBP_LoadingClass))
		{
			Loading->AddToViewport();
		}
	}
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent == nullptr) return;

	//InputComponent->BindAction(TEXT("Quit"), IE_Pressed, this, &ThisClass::ShowReturnToMainMenu);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(IA_QuitAction, ETriggerEvent::Triggered, this, &ThisClass::ShowPauseMenu);
	}
}

void ABasePlayerController::ShowPauseMenu()
{
	WBP_PauseMenu = WBP_PauseMenu == nullptr ? CreateWidget<UUserWidget>(this, WBP_PauseMenuClass) : WBP_PauseMenu.Get();
	if (WBP_PauseMenu->IsInViewport())
	{
		WBP_PauseMenu->RemoveFromParent();
		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		SetShowMouseCursor(false);
	}
	else
	{
		WBP_PauseMenu->AddToViewport();

		FInputModeGameAndUI InputModeGameAndUI;
		InputModeGameAndUI.SetWidgetToFocus(WBP_PauseMenu->TakeWidget());
		SetInputMode(InputModeGameAndUI);
		SetShowMouseCursor(true);
	}
}

