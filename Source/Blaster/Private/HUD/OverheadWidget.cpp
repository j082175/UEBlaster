// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	ENetRole LocalRole = InPawn->GetLocalRole();
	FString Role;
	switch (LocalRole)
	{
	case ROLE_None:
		Role = TEXT("None");
		break;
	case ROLE_SimulatedProxy:
		Role = TEXT("Simulated Proxy");
		break;
	case ROLE_AutonomousProxy:
		Role = TEXT("Autonomous Proxy");
		break;
	case ROLE_Authority:
		Role = TEXT("Authority");
		break;
	case ROLE_MAX:
		break;
	default:
		break;
	}
	FString LocalRoleString = FString::Printf(TEXT("Local Role : %s"), *Role);

	SetDisplayText(LocalRoleString);
}

void UOverheadWidget::ShowPlayerName(APlayerState* InPlayerState)
{
	if (InPlayerState)
	{
		FString PlayerName = InPlayerState->GetPlayerName();

		FString Result = FString::Printf(TEXT("Player name : %s"), *PlayerName);

		if (DisplayText2)
		{
			DisplayText2->SetText(FText::FromString(Result));
		}

		//UE_LOG(LogTemp, Display, TEXT("PlayerState is : %s"), *InPlayerState->GetName());
	}
}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}
