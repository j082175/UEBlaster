// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BasePlayerController.h"
#include "Blueprint/UserWidget.h"

ABasePlayerController::ABasePlayerController()
{
	//static ConstructorHelpers::FClassFinder<UUserWidget> WBP_LoadingClassRef(TEXT("/Game/A_Blaster/Blueprints/HUD/LobbyWidgets/WBP_Loading.WBP_Loading_C"));
	//ensure(WBP_LoadingClassRef.Class);
	//WBP_LoadingClass = WBP_LoadingClassRef.Class;
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
