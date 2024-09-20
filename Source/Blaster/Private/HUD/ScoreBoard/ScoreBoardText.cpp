// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ScoreBoard/ScoreBoardText.h"
#include "Components/TextBlock.h"

FString UScoreBoardText::GetPlayerName() const
{
	return PlayerNameText->GetText().ToString();
}

void UScoreBoardText::SetScoreBoardText(const FScoreBoardTextStruct& InScoreBoardStruct)
{
	if (PlayerNameText) PlayerNameText->SetText(InScoreBoardStruct.PlayerName);
	if (ScoreText) ScoreText->SetText(FText::FromString(FString::FromInt(InScoreBoardStruct.Score)));
	if (ElimsText) ElimsText->SetText(FText::FromString(FString::FromInt(InScoreBoardStruct.Elims)));
	if (LatencyText) LatencyText->SetText(FText::FromString(FString::Printf(TEXT("%f"), InScoreBoardStruct.Latency)));
}

void UScoreBoardText::SetPlayerName(const FString& InName)
{
	PlayerNameText->SetText(FText::FromString(InName));
}

void UScoreBoardText::SetScore(int32 InScore)
{
	ScoreText->SetText(FText::FromString(FString::FromInt(InScore)));
}

void UScoreBoardText::SetElims(int32 InElims)
{
	ElimsText->SetText(FText::FromString(FString::FromInt(InElims)));
}

void UScoreBoardText::SetLatency(float InLatency)
{
	LatencyText->SetText(FText::FromString(FString::Printf(TEXT("%f"), InLatency)));
}
