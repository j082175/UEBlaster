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

	DOREPLIFETIME(ThisClass, WBP_ScoreBoard);
	DOREPLIFETIME(ThisClass, OwingController);
	//DOREPLIFETIME(ThisClass, ScoreBoardTextArr);

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

	if (!GetOwner()->HasAuthority())
	{
		int a = 1;
	}

	OwingController = Cast<ABlasterPlayerController>(GetOwner());

	//WBP_ScoreBoard = WBP_ScoreBoard == nullptr ? CreateWidget<UScoreBoard>(this, WBP_ScoreBoardClass) : WBP_ScoreBoard.Get();
	WBP_ScoreBoard = Cast<UScoreBoard>(GetWidget());
	WBP_ScoreBoard->AddToViewport();
	WBP_ScoreBoard->SetVisibility(ESlateVisibility::Collapsed);

	FTimerHandle H;
	GetWorld()->GetTimerManager().SetTimer(H, FTimerDelegate::CreateLambda([&]()
		{
			SetAllControllerScoreBoard();
		}), 1.f, false);
}


// Called every frame
void UScoreBoardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//TArray<UScoreBoardText*> UScoreBoardComponent::GetScoreBoardTexts() const
//{
//	return ScoreBoardTextArr;
//}

void UScoreBoardComponent::InitScoreBoard()
{
	//UE_LOG(LogTemp, Display, TEXT("InitScoreBoard"));
	ABlasterPlayerState* BlasterPlayerState = OwingController->GetPlayerState<ABlasterPlayerState>();

	if (!BlasterPlayerState)
	{
		return;
	}

	if (ABlasterGameState* BlasterGameState = GetWorld()->GetGameState<ABlasterGameState>())
	{
		//OnRedTeamScoreChanged.ExecuteIfBound(BlasterGameState->GetRedTeamScore());
		//OnBlueTeamScoreChanged.ExecuteIfBound(BlasterGameState->GetBlueTeamScore());

		FScoreBoardTextStruct S;
		S.PlayerName = BlasterPlayerState->GetPlayerName();
		S.Score = BlasterPlayerState->GetScore();
		S.Elims = BlasterPlayerState->GetDefeats();
		S.Latency = BlasterPlayerState->GetPingInMilliseconds();

		if (BlasterPlayerState && BlasterPlayerState->IGetTeam() == ETeam::RedTeam)
		{
			//AddScoreBoard(ETeam::RedTeam, S);
		}
		else if (BlasterPlayerState && BlasterPlayerState->IGetTeam() == ETeam::BlueTeam)
		{
			//AddScoreBoard(ETeam::BlueTeam, S);
		}

	}
}

void UScoreBoardComponent::ClientAddScoreBoard_Implementation(ETeam InTeam, const FString& InPlayerName, const FString& OwnerName)
{
	UE_LOG(LogTemp, Display, TEXT("ClientAddScoreBoard_Implementation"));

	UE_LOG(LogTemp, Display, TEXT("AddScoreBoard : %s, WBP_ScoreBoard : %x, Name : %s"), *UEnum::GetDisplayValueAsText(GetOwner()->GetLocalRole()).ToString(), WBP_ScoreBoard.Get(), *GetOwner()->GetName());

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
		UE_LOG(LogTemp, Error, TEXT("WBP_ScoreBoard NULL!!!"));
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

void UScoreBoardComponent::OnRep_ScoreBoard()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_ScoreBoard"));
}

void UScoreBoardComponent::SetAllControllerScoreBoard_Implementation()
{
	if (!GetOwner()->HasAuthority()) return;

	UE_LOG(LogTemp, Display, TEXT("SetAllControllerScoreBoard"));

	ATeamsGameMode* TeamsGameMode = GetWorld()->GetAuthGameMode<ATeamsGameMode>();

	ABlasterPlayerState* ThisPlayerState = OwingController->GetPlayerState<ABlasterPlayerState>();

	for (FConstPlayerControllerIterator Opponent = GetWorld()->GetPlayerControllerIterator(); Opponent; ++Opponent)
	{

		ABlasterPlayerState* OpponentBlasterPlayerState = Opponent->Get()->GetPlayerState<ABlasterPlayerState>();


		if (!CheckPC.Contains(Opponent->Get()))
		{
			ClientAddScoreBoard(OpponentBlasterPlayerState->IGetTeam(), OpponentBlasterPlayerState->GetPlayerName(), ThisPlayerState->GetPlayerName());
			CheckPC.Add(Opponent->Get());
		}


		if (Opponent->Get() == OwingController)
		{
			continue;
		}

		if (!Opponent->Get()->GetComponentByClass<UScoreBoardComponent>()->CheckPC.Contains(Cast<APlayerController>(GetOwner())))
		{
			Opponent->Get()->GetComponentByClass<UScoreBoardComponent>()->ClientAddScoreBoard(ThisPlayerState->IGetTeam(), ThisPlayerState->GetPlayerName(), OpponentBlasterPlayerState->GetPlayerName());
			Opponent->Get()->GetComponentByClass<UScoreBoardComponent>()->CheckPC.Add(Cast<APlayerController>(GetOwner()));
		}

	}
}

