// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ScoreBoardComponent.h"
#include "HUD/ScoreBoard/ScoreBoard.h"
#include "PlayerState/BlasterPlayerState.h"
#include "PlayerController/BlasterPlayerController.h"
#include "GameState/BlasterGameState.h"
#include "GameMode/TeamsGameMode.h"
#include "Net/UnrealNetwork.h"

#include "Components/ScrollBox.h"

// Sets default values for this component's properties
UScoreBoardComponent::UScoreBoardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 1.f;

	SetIsReplicatedByDefault(true);

	// ...
	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_ScoreBoardClassRef(TEXT("/Game/A_Blaster/Blueprints/HUD/ScoreBoard/WBP_ScoreBoard.WBP_ScoreBoard_C"));
	ensure(WBP_ScoreBoardClassRef.Class);
	WBP_ScoreBoardClass = WBP_ScoreBoardClassRef.Class;

	SetWidgetClass(WBP_ScoreBoardClass);
}

void UScoreBoardComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UScoreBoardComponent::PostLoad()
{
	Super::PostLoad();
	//SetIsReplicated(true);


}


// Called when the game starts
void UScoreBoardComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	UE_LOG(LogTemp, Warning, TEXT("UScoreBoardComponent::BeginPlay, Authority : %d"), GetOwner()->HasAuthority());
	WBP_ScoreBoard = WBP_ScoreBoard == nullptr ? Cast<UScoreBoard>(GetWidget()) : WBP_ScoreBoard.Get();
	if (WBP_ScoreBoard && !WBP_ScoreBoard->IsInViewport())
	{
		WBP_ScoreBoard->AddToViewport();
		WBP_ScoreBoard->SetVisibility(ESlateVisibility::Collapsed);
	}


	GetWorld()->GetTimerManager().SetTimer(InitTimer, this, &ThisClass::SetAllControllerScoreBoard, 0.5f, true);
}


// Called every frame
void UScoreBoardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//if (GetOwner()->HasAuthority() && bWorldPlayersCountChanged)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("WorldPlayersCountChanged"));
	//	SetAllControllerScoreBoard();
	//}

}

//TArray<UScoreBoardText*> UScoreBoardComponent::GetScoreBoardTexts() const
//{
//	return ScoreBoardTextArr;
//}


void UScoreBoardComponent::ClientAddScoreBoard_Implementation(ETeam InTeam, const FString& InPlayerName, const FString& OwnerName)
{
	//UE_LOG(LogTemp, Display, TEXT("ClientAddScoreBoard_Implementation"));

	WBP_ScoreBoard = WBP_ScoreBoard == nullptr ? Cast<UScoreBoard>(GetWidget()) : WBP_ScoreBoard.Get();

	if (WBP_ScoreBoard)
	{
		FScoreBoardTextStruct S;
		S.PlayerName = InPlayerName;
		S.Score = 0;
		S.Elims = 0;
		S.Latency = 0;

		if (InTeam == ETeam::RedTeam)
		{
			WBP_ScoreBoard->AddRedTeam(S, OwnerName);

		}
		else if (InTeam == ETeam::BlueTeam)
		{
			WBP_ScoreBoard->AddBlueTeam(S, OwnerName);
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WBP_ScoreBoard NULL!!!, Authority : %d"), GetOwner()->HasAuthority());
	}
}

void UScoreBoardComponent::RemoveScoreBoard(const FString& InPlayerName)
{
	if (WBP_ScoreBoard) WBP_ScoreBoard->RemoveTeam(InPlayerName);
}

void UScoreBoardComponent::ShowScoreBoard()
{
	WBP_ScoreBoard->SetVisibility(ESlateVisibility::Visible);
}

void UScoreBoardComponent::ReleaseScoreBoard()
{
	if (WBP_ScoreBoard && WBP_ScoreBoard->IsInViewport())
	{
		WBP_ScoreBoard->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UScoreBoardComponent::ClientChangeScore_Implementation(const FString& InPlayerName, int32 InValue, bool IsScore)
{
	if (!WBP_ScoreBoard)
	{
		return;
	}

	{
		TArray<UWidget*> Children = WBP_ScoreBoard->BlueTeamScoreBox->GetAllChildren();
		for (size_t i = 0; i < Children.Num(); i++)
		{
			if (UScoreBoardText* ScoreBoardText = Cast<UScoreBoardText>(Children[i]))
			{
				if (ScoreBoardText->GetPlayerName() == InPlayerName)
				{
					if (IsScore)
					{
						//UE_LOG(LogTemp, Warning, TEXT("SetScore"));
						ScoreBoardText->SetScore(InValue);
					}
					else
					{
						//UE_LOG(LogTemp, Warning, TEXT("SetElims"));
						ScoreBoardText->SetElims(InValue);
					}
				}
			}

		}
	}

	{
		TArray<UWidget*> Children = WBP_ScoreBoard->RedTeamScoreBox->GetAllChildren();
		for (size_t i = 0; i < Children.Num(); i++)
		{
			if (UScoreBoardText* ScoreBoardText = Cast<UScoreBoardText>(Children[i]))
			{
				if (ScoreBoardText->GetPlayerName() == InPlayerName)
				{
					if (IsScore)
					{
						//UE_LOG(LogTemp, Warning, TEXT("SetScore"));
						ScoreBoardText->SetScore(InValue);
					}
					else
					{
						//UE_LOG(LogTemp, Warning, TEXT("SetElims"));
						ScoreBoardText->SetElims(InValue);
					}
				}
			}

		}
	}


}

void UScoreBoardComponent::ClientRemoveScoreText_Implementation(const FString& InPlayerName)
{
	if (!WBP_ScoreBoard)
	{
		return;
	}

	{
		TArray<UWidget*> Children = WBP_ScoreBoard->BlueTeamScoreBox->GetAllChildren();
		for (size_t i = 0; i < Children.Num(); i++)
		{
			if (UScoreBoardText* ScoreBoardText = Cast<UScoreBoardText>(Children[i]))
			{
				if (ScoreBoardText->GetPlayerName() == InPlayerName)
				{
					ScoreBoardText->RemoveFromViewport();
				}
			}

		}
	}

	{
		TArray<UWidget*> Children = WBP_ScoreBoard->RedTeamScoreBox->GetAllChildren();
		for (size_t i = 0; i < Children.Num(); i++)
		{
			if (UScoreBoardText* ScoreBoardText = Cast<UScoreBoardText>(Children[i]))
			{
				if (ScoreBoardText->GetPlayerName() == InPlayerName)
				{
					ScoreBoardText->RemoveFromViewport();
				}
			}

		}
	}
}

void UScoreBoardComponent::SetAllControllerScoreBoard()
{
	if (!GetOwner()->HasAuthority()) return;

	//UE_LOG(LogTemp, Display, TEXT("SetAllControllerScoreBoard"));

	//WBP_ScoreBoard = WBP_ScoreBoard == nullptr ? Cast<UScoreBoard>(GetWidget()) : WBP_ScoreBoard.Get();

	OwingController = OwingController == nullptr ? Cast<ABlasterPlayerController>(GetOwner()) : OwingController;

	if (!OwingController.IsValid() || !OwingController->GetPlayerState<ABlasterPlayerState>())
	{
		return;
	}



	ABlasterPlayerState* ThisPlayerState = OwingController->GetPlayerState<ABlasterPlayerState>();

	for (FConstPlayerControllerIterator Opponent = GetWorld()->GetPlayerControllerIterator(); Opponent; ++Opponent)
	{
		ABlasterPlayerState* OpponentBlasterPlayerState = Opponent->Get()->GetPlayerState<ABlasterPlayerState>();

		if (!OpponentBlasterPlayerState)
		{
			return;
		}

		if (!CheckPC.Contains(Opponent->Get()))
		{
			ClientAddScoreBoard(OpponentBlasterPlayerState->IGetTeam(), OpponentBlasterPlayerState->GetPlayerName(), ThisPlayerState->GetPlayerName());
			CheckPC.Add(Opponent->Get());
		}

		if (Opponent->Get() == OwingController)
		{
			continue;
		}

		UScoreBoardComponent* OpponentSC = Opponent->Get()->GetComponentByClass<UScoreBoardComponent>();
		if (!OpponentSC) return;
		APlayerController* ThisPC = Cast<APlayerController>(GetOwner());
		if (!ThisPC) return;

		if (!OpponentSC->CheckPC.Contains(ThisPC))
		{
			if (OpponentSC->GetScoreBoard())
			{
				OpponentSC->ClientAddScoreBoard(ThisPlayerState->IGetTeam(), ThisPlayerState->GetPlayerName(), OpponentBlasterPlayerState->GetPlayerName());
				OpponentSC->CheckPC.Add(ThisPC);
			}
			else
			{
				return;
			}
		}

	}

	UE_LOG(LogTemp, Display, TEXT("SetAllControllerScoreBoard Initialize Finished"));
	GetWorld()->GetTimerManager().ClearTimer(InitTimer);
	InitTimer.Invalidate();
}
