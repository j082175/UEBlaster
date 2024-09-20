// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ScoreBoard/ScoreBoard.h"
#include "Components/ScrollBox.h"
#include "Interfaces/WidgetBindDelegateInterface.h"
#include "HUD/ScoreBoard/TeamScore.h"


UScoreBoard::UScoreBoard(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ScoreBoardTextRef(TEXT("/Game/A_Blaster/Blueprints/HUD/ScoreBoard/WBP_ScoreBoardText.WBP_ScoreBoardText_C"));
	ensure(ScoreBoardTextRef.Class);
	ScoreBoardTextClass = ScoreBoardTextRef.Class;
}

void UScoreBoard::NativeConstruct()
{
	Super::NativeConstruct();

	OnVisibilityChanged.Clear();
}

void UScoreBoard::SetBlueTeamScore(int32 InCurrentScore)
{
	WBP_TeamScore->SetBlueTeamScore(InCurrentScore);
}

void UScoreBoard::SetRedTeamScore(int32 InCurrentScore)
{
	WBP_TeamScore->SetRedTeamScore(InCurrentScore);
}

void UScoreBoard::AddBlueTeam(const FScoreBoardTextStruct& InScoreBoardTextStruct)
{
	ScoreBoardText = CreateWidget<UScoreBoardText>(this, ScoreBoardTextClass);
	if (ScoreBoardText)
	{
		ScoreBoardText->SetScoreBoardText(InScoreBoardTextStruct);
		BlueTeamScoreBox->AddChild(ScoreBoardText);
	}


}

void UScoreBoard::AddRedTeam(const FScoreBoardTextStruct& InScoreBoardTextStruct)
{
	ScoreBoardText = CreateWidget<UScoreBoardText>(this, ScoreBoardTextClass);
	if (ScoreBoardText)
	{
		ScoreBoardText->SetScoreBoardText(InScoreBoardTextStruct);
		RedTeamScoreBox->AddChild(ScoreBoardText);
	}

}

void UScoreBoard::RemoveTeam(const FString& InPlayerName)
{
	UE_LOG(LogTemp, Display, TEXT("RemoveTeamStart"));

	for (auto& i : RedTeamScoreBox->GetAllChildren())
	{
		if (UScoreBoardText* T = Cast<UScoreBoardText>(i))
		{
			if (T->GetPlayerName() == InPlayerName)
			{
				UE_LOG(LogTemp, Display, TEXT("RemoveTeam"));
				T->RemoveFromParent();
			}
		}
	}

	for (auto& i : BlueTeamScoreBox->GetAllChildren())
	{
		if (UScoreBoardText* T = Cast<UScoreBoardText>(i))
		{
			if (T->GetPlayerName() == InPlayerName)
			{
				UE_LOG(LogTemp, Display, TEXT("RemoveTeam"));

				T->RemoveFromParent();
			}
		}
	}
}
