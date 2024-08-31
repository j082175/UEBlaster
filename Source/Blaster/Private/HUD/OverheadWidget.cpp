// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/TeamInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/WidgetBindDelegateInterface.h"


void UOverheadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Visible);


	Init();

}

void UOverheadWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UE_LOG(LogTemp, Display, TEXT("OverheadWidget Tick"));
}

void UOverheadWidget::Init()
{

	GetWorld()->GetTimerManager().SetTimer(InitTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(GetOwningPlayerPawn());
			if (WBDI)
			{
				//UE_LOG(LogTemp, Display, TEXT("Initializing"));
				WBDI->IBindOverheadWidget(this);
				GetWorld()->GetTimerManager().ClearTimer(InitTimerHandle);
				InitTimerHandle.Invalidate();
			}
		}), 0.01f, true);
}

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (LocaleRoleText)
	{
		LocaleRoleText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::SetTextColor(FLinearColor InColor)
{
	LocaleRoleText->SetColorAndOpacity(InColor);
	PlayerIDText->SetColorAndOpacity(InColor);

	//APawn* P = GetWorld()->GetFirstPlayerController()->GetPawn();
	//if (P)
	//{

	//	ITeamInterface* T2 = Cast<ITeamInterface>(P);

	//	TArray<AActor*> FoundActors;
	//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActorToFind, FoundActors);

	//	for (auto& i : FoundActors)
	//	{
	//		ITeamInterface* T1 = Cast<ITeamInterface>(i);
	//		if (T1 && T1->IGetTeam() == T2->IGetTeam())
	//		{
	//			LocaleRoleText->SetColorAndOpacity(Green);
	//			PlayerIDText->SetColorAndOpacity(Green);
	//		}
	//		else
	//		{
	//			LocaleRoleText->SetColorAndOpacity(Red);
	//			PlayerIDText->SetColorAndOpacity(Red);
	//		}
	//	}

		//ITeamInterface* T1 = Cast<ITeamInterface>(GetOwningPlayerPawn());



		//if (InTeam == T2->IGetTeam())
		//{
		//	LocaleRoleText->SetColorAndOpacity(Green);
		//	PlayerIDText->SetColorAndOpacity(Green);
		//}
		//else
		//{
		//	LocaleRoleText->SetColorAndOpacity(Red);
		//	PlayerIDText->SetColorAndOpacity(Red);
		//}
	//}
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

void UOverheadWidget::ServerShowPlayerName_Implementation(APlayerState* InPlayerState)
{
	MulticastShowPlayerName(InPlayerState);
}

void UOverheadWidget::MulticastShowPlayerName_Implementation(APlayerState* InPlayerState)
{
	ShowPlayerName(InPlayerState);
}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}
