// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/TeamInterface.h"


void UOverheadWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("OverheadWidget Tick"));
}

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (LocaleRoleText)
	{
		LocaleRoleText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::SetTextColor(ETeam InTeam)
{
	FSlateColor Red(FLinearColor(1.f, 0.13f, 0.19f));
	FSlateColor Green(FLinearColor(0.1f, 1.f, 0.f));

	APawn* P = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (P)
	{
		ITeamInterface* T1 = Cast<ITeamInterface>(GetOwningPlayerPawn());
		ITeamInterface* T2 = Cast<ITeamInterface>(P);


		if (InTeam == T2->IGetTeam())
		{
			LocaleRoleText->SetColorAndOpacity(Green);
			PlayerIDText->SetColorAndOpacity(Green);
		}
		else
		{
			LocaleRoleText->SetColorAndOpacity(Red);
			PlayerIDText->SetColorAndOpacity(Red);
		}
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
	TWeakObjectPtr<APlayerState> PS = InPlayerState;

	if (PS.IsValid())
	{
		FString PlayerName = PS->GetPlayerName();
		FString Result = FString::Printf(TEXT("Player name : %s"), *PlayerName);

		if (PlayerIDText)
		{
			PlayerIDText->SetText(FText::FromString(Result));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerIDText is not valid"));
		}


		//UE_LOG(LogTemp, Display, TEXT("PlayerState is : %s"), *InPlayerState->GetName());
	}
	else
	{
		/*UE_LOG(LogTemp, Error, TEXT("OverheadWidget : PlayerState is not valid"));*/
	}
}

void UOverheadWidget::ShowPlayerName(const FString& InName)
{
	PlayerIDText->SetText(FText::FromString(InName));
}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}
