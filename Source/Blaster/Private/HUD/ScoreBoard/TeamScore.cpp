// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ScoreBoard/TeamScore.h"
#include "Components/TextBlock.h"

void UTeamScore::SetBlueTeamScore(int32 InCurrentScore)
{
	BlueTeamScoreText->SetText(FText::FromString(FString::FromInt(InCurrentScore)));
}

void UTeamScore::SetRedTeamScore(int32 InCurrentScore)
{
	RedTeamScoreText->SetText(FText::FromString(FString::FromInt(InCurrentScore)));
}
