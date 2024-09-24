// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ComponentHUD/DamageText.h"
#include "Components/TextBlock.h"

void UDamageText::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ThisClass::RemoveFromViewport, DestroyTime, false);
}

void UDamageText::SetDamageText(int32 InDamage, const FLinearColor& InColor)
{
	DamageText->SetText(FText::FromString(FString::FromInt(InDamage)));
	DamageText->Font.OutlineSettings.OutlineColor = InColor;
}
