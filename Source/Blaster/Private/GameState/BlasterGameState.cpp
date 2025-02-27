// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/BlasterGameState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/BlasterPlayerState.h"
#include "PlayerController/BlasterPlayerController.h"
#include "Components/ObjectPoolComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/TeamInterface.h"
#include "Characters/CharacterBase.h"
#include "HUD/OverheadWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/ScoreBoardComponent.h"
#include "Blaster/Blaster.h"

ABlasterGameState::ABlasterGameState()
{
	ObjectPoolComponent = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("ObjectPoolComponent"));


	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
}

void ABlasterGameState::BeginPlay()
{
	AB_LOG(LogABBeginPlay, Warning, TEXT("%s"), TEXT("Begin"));
	Super::BeginPlay();
	AB_LOG(LogABBeginPlay, Warning, TEXT("%s"), TEXT("End"));
}

void ABlasterGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Display, TEXT("BlasterGameState tick"));
}

void ABlasterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TopScoringPlayers);
	DOREPLIFETIME(ThisClass, RedTeamScore);
	DOREPLIFETIME(ThisClass, BlueTeamScore);

	DOREPLIFETIME(ThisClass, RedTeam);
	DOREPLIFETIME(ThisClass, BlueTeam);

}

void ABlasterGameState::UpdateTopScore(ABlasterPlayerState* ScoringPlayer)
{
	if (TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
	else if (ScoringPlayer->GetScore() == TopScore)
	{
		TopScoringPlayers.AddUnique(ScoringPlayer);

	}
	else if (ScoringPlayer->GetScore() > TopScore)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.AddUnique(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
}

void ABlasterGameState::RedTeamScores()
{
	++RedTeamScore;

	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDRedTeamScore(RedTeamScore);
	}
}

void ABlasterGameState::BlueTeamScores()
{
	++BlueTeamScore;

	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDBlueTeamScore(BlueTeamScore);
	}
}

void ABlasterGameState::OnRep_RedTeamScore()
{
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDRedTeamScore(RedTeamScore);
	}
}

void ABlasterGameState::OnRep_BlueTeamScore()
{
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDBlueTeamScore(BlueTeamScore);
	}
}

void ABlasterGameState::ChangeScoreBoard(const FString& InPlayerName, int32 InValue, bool IsScore)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		//ABlasterPlayerController* TempBlasterPlayerController = Cast<ABlasterPlayerController>(*It);
		//if (TempBlasterPlayerController)
		//{
		//	TempBlasterPlayerController->ClientChatCommitted(Text, PlayerName);
		//}

		UScoreBoardComponent* ScoreBoardComponent = It->Get()->GetComponentByClass<UScoreBoardComponent>();
		if (ScoreBoardComponent)
		{
			ScoreBoardComponent->ClientChangeScore(InPlayerName, InValue, IsScore);
		}
	}
}