// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/LobbyPlayerController.h"
#include "GameInstance/MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "InputMappingContext.h"

// EnhancedInput
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ALobbyPlayerController::ALobbyPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_LobbyRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/A_Blaster/Inputs/IMC_Lobby.IMC_Lobby'"));
	ensure(IMC_LobbyRef.Object);
	IMC_Lobby = IMC_LobbyRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_QuitActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/A_Blaster/Inputs/IA_Escape.IA_Escape'"));
	ensure(IA_QuitActionRef.Object);
	IA_QuitAction = IA_QuitActionRef.Object;
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

	}
}

void ALobbyPlayerController::ServerSpawnCharacter_Implementation()
{
	if (UMyGameInstance* MyGameInstance = GetGameInstance<UMyGameInstance>())
	{
		MyGameInstance->SpawnCharacter();
	}
}
