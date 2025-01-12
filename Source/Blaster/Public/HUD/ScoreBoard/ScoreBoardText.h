// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "HUD/MyUserWidget.h"
#include "ScoreBoardText.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FScoreBoardTextStruct
{
	GENERATED_BODY()
public:
	FScoreBoardTextStruct(const FString& InPlayerName = TEXT("None"), int32 InScore = 0, int32 InElims = 0, float InLatency = 0.f)
		:PlayerName(InPlayerName), Score(InScore), Elims(InElims), Latency(InLatency)
	{}

	UPROPERTY(EditAnywhere)
	FString PlayerName;
	UPROPERTY(EditAnywhere)
	int32 Score;
	UPROPERTY(EditAnywhere)
	int32 Elims;
	UPROPERTY(EditAnywhere)
	float Latency;
};

UCLASS()
class BLASTER_API UScoreBoardText : public UUserWidget
{
	GENERATED_BODY()
public:

	FString GetPlayerName() const;

	void SetScoreBoardText(const FScoreBoardTextStruct& InScoreBoardStruct);

	void SetPlayerName(const FString& InName);
	void SetScore(int32 InScore);
	void SetElims(int32 InElims);
	//void SetLatency(float InLatency);

	void SetBackgroundColor(FLinearColor InColor);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ScoreBoardBackground;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ElimsText;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UTextBlock> LatencyText;
};
