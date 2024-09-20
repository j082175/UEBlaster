// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamScore.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UTeamScore : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetBlueTeamScore(int32 InCurrentScore);
	void SetRedTeamScore(int32 InCurrentScore);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BlueTeamScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> RedTeamScoreText;
};
