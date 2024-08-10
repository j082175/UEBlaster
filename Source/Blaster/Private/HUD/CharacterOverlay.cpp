// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CharacterOverlay.h"
#include "Components/TextBlock.h"

void UCharacterOverlay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//ScoreAmount->SetText(FText::FromString(FString::Printf(TEXT("%f"), InDeltaTime)));
}

void UCharacterOverlay::Test(float DeltaTime)
{
	if (ScoreAmount) ScoreAmount->SetText(FText::FromString(FString::Printf(TEXT("%f"), DeltaTime)));
}
