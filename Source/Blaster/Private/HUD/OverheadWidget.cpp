// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/TeamInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/WidgetBindDelegateInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/BlasterCharacter.h"
#include "Blaster.h"


UOverheadWidget::UOverheadWidget(const FObjectInitializer& ObjectInitializer)
	//:Super(ObjectInitializer)
{
	AB_CALLLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
}

void UOverheadWidget::NativeConstruct()
{
	AB_CALLLOG(LogABBeginPlay, Warning, TEXT("%s"), TEXT("Begin"));
	Super::NativeConstruct();
	AB_CALLLOG(LogABBeginPlay, Warning, TEXT("%s"), TEXT("End"));

	SetVisibility(ESlateVisibility::Visible);

	ShowPlayerName(Cast<APawn>(OwingActor));
}

void UOverheadWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UE_LOG(LogTemp, Display, TEXT("OverheadWidget Tick"));


}

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (LocaleRoleText)
	{
		LocaleRoleText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::SetAllTextColor(FLinearColor InColor)
{
	LocaleRoleText->SetColorAndOpacity(InColor);
	PlayerIDText->SetColorAndOpacity(InColor);
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

void UOverheadWidget::ShowPlayerName(APawn* InPawn)
{
	//UE_LOG(LogTemp, Display, TEXT("ShowPlayerName"));

	//game instance > game mode > game state > player state > controller > pawn
	//begin play is initialized around the time player state is,
	//so smetimes it's not initialized yet so we get the name once on tick 

	PawnToInit = InPawn;
	
	GetWorld()->GetTimerManager().SetTimer(InitHandle, this, &ThisClass::ShowPlayerNameInitFunc, 0.1f, true);
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

void UOverheadWidget::ShowPlayerNameInitFunc()
{
	if (PawnToInit.IsValid())
	{
		APlayerState* PlayerState = PawnToInit->GetPlayerState();

		if (PlayerState)
		{
			FString PlayerName = PlayerState->GetPlayerName();
			FString PlayerNameToDisplay = FString::Printf(TEXT("%s"), *PlayerName);

			PlayerIDText->SetText(FText::FromString(PlayerNameToDisplay));
			GetWorld()->GetTimerManager().ClearTimer(InitHandle);
			InitHandle.Invalidate();
		}
	}
}
