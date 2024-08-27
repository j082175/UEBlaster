// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverlayModules/AttributeStatus.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UAttributeStatus::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAttributeStatus::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


}

//void UAttributeStatus::SetHpBar(float InCurrentHp, float InMaxHp)
//{
//	HpBar->SetPercent(InCurrentHp / InMaxHp);
//}
//
//void UAttributeStatus::SetShieldBar(float InCurrentShield, float InMaxShield)
//{
//	ShieldBar->SetPercent(InCurrentShield / InMaxShield);
//}
//
//void UAttributeStatus::SetSpBar(float InCurrentSp, float InMaxSp)
//{
//	SpBar->SetPercent(InCurrentSp / InMaxSp);
//}
//
//void UAttributeStatus::SetParryGaugeBar(float InCurrentParryGauge, float InMaxParryGauge)
//{
//	ParryGagueBar->SetPercent(InCurrentParryGauge / InMaxParryGauge);
//}


//void UAttributeStatus::SetCurrentHp(float InCurrentHp)
//{
//	CurrentHp->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::CeilToInt(FMath::Clamp(InCurrentHp, 0.f, InCurrentHp)))));
//}
//
//void UAttributeStatus::SetMaxHp(float InMaxHp)
//{
//	MaxHp->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::CeilToInt(InMaxHp))));
//}
//
//void UAttributeStatus::SetCurrentShield(float InCurrentShield)
//{
//	CurrentShield->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::CeilToInt(FMath::Clamp(InCurrentShield, 0.f, InCurrentShield)))));
//}
//
//void UAttributeStatus::SetMaxShield(float InMaxShield)
//{
//	MaxShield->SetText(FText::FromString(FString::Printf(TEXT("%f"), InMaxShield)));
//}
//
//void UAttributeStatus::SetCurrentSp(float InCurrentSp)
//{
//	CurrentSp->SetText(FText::FromString(FString::Printf(TEXT("%f"), FMath::CeilToInt(FMath::Clamp(InCurrentSp, 0.f, InCurrentSp)))));
//}
//
//void UAttributeStatus::SetMaxSp(float InMaxSp)
//{
//	MaxSp->SetText(FText::FromString(FString::Printf(TEXT("%f"), InMaxSp)));
//}
//
//void UAttributeStatus::SetCurrentParryGauge(float InCurrentParryGauge)
//{
//	CurrentParryGauge->SetText(FText::FromString(FString::Printf(TEXT("%f"), FMath::CeilToInt(FMath::Clamp(InCurrentParryGauge, 0.f, InCurrentParryGauge)))));
//}
//
//void UAttributeStatus::SetMaxParryGauge(float InMaxParryGauge)
//{
//	MaxParryGauge->SetText(FText::FromString(FString::Printf(TEXT("%f"), InMaxParryGauge)));
//}

void UAttributeStatus::SetProgressBar(UProgressBar* InProgressBar, float InPercent)
{
	InProgressBar->SetPercent(InPercent);
}

void UAttributeStatus::SetCurrentTextBlock(UTextBlock* InTextBlock, float InCurrentValue)
{
	InTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::CeilToInt(FMath::Clamp(InCurrentValue, 0.f, InCurrentValue)))));
}

void UAttributeStatus::SetMaxTextBlock(UTextBlock* InTextBlock, float InMaxValue)
{
	InTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::CeilToInt(InMaxValue))));
}
