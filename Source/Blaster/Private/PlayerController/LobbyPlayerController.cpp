// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/LobbyPlayerController.h"
#include "GameInstance/MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "InputMappingContext.h"

// EnhancedInput
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "GameMenu.h"


ALobbyPlayerController::ALobbyPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_LobbyRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/A_Blaster/Inputs/IMC_Lobby.IMC_Lobby'"));
	ensure(IMC_LobbyRef.Object);
	IMC_Lobby = IMC_LobbyRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_PressStartRef(TEXT("/Script/EnhancedInput.InputAction'/Game/A_Blaster/Inputs/IA_Tab.IA_Tab'"));
	ensure(IA_PressStartRef.Object);
	IA_PressStart = IA_PressStartRef.Object;

	static ConstructorHelpers::FClassFinder<UGameMenu> GameStartButtonClassRef(TEXT("/MultiplayerSessions/WBP_GameMenu.WBP_GameMenu_C"));
	ensure(GameStartButtonClassRef.Class);
	GameStartButtonClass = GameStartButtonClassRef.Class;
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
	}

	if (const ULocalPlayer* LocalPlayer = (GEngine && GetWorld()) ? GEngine->GetFirstGamePlayer(GetWorld()) : nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		if (IMC_Lobby)
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(IMC_Lobby, 0);
		}
	}
}

void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent == nullptr) return;

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(IA_PressStart, ETriggerEvent::Triggered, this, &ThisClass::PressStarted);
	}
}

void ALobbyPlayerController::CreateStartButton()
{
	if (IsLocalPlayerController())
	{
		GameStartButton = CreateWidget<UGameMenu>(this, GameStartButtonClass);
	}

}

void ALobbyPlayerController::PressStarted()
{
	if (IsLocalPlayerController() && GameStartButton)
	{
		GameStartButton->OnButtonClickedFunc();
	}
}

void ALobbyPlayerController::ServerSpawnCharacter_Implementation()
{
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>())
	{
		MyGameInstance->SpawnCharacter();
	}
}
