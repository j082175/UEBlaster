// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverlayModules/SkillBar.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Blaster/Blaster.h"
#include "Components/ProgressBar.h"

void USkillBar::NativeConstruct()
{
	Super::NativeConstruct();

	WidgetAnimHelper::FindWidgetAnimationName(this, FindWidgetAnimation);
}

void USkillBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void USkillBar::StartAnimation(const FString& InPrefix, int32 InIndex, float InPlaybackSpeed)
{
	WidgetAnimHelper::StartAnimation(InPrefix, TEXT("CoolTime"), InIndex, InPlaybackSpeed, this, FindWidgetAnimation);
}

void USkillBar::SetSkillCost(int32 InIndex, const FString& InStr)
{
	TWeakObjectPtr<UTextBlock> TB = Cast<UTextBlock>(GetWidgetFromName(*FString::Printf(TEXT("SkillCost%d"), InIndex)));
	if (TB.IsValid())
	{
		TB->SetText(FText::FromString(InStr));
	}
}

void USkillBar::SetSoulCount(int32 InCount)
{
	//UE_LOG(LogTemp, Display, TEXT("SoulCountChanged"));
	SoulCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), InCount)));
}

void USkillBar::SetSpBar(float InPercent)
{
	SpBar->SetPercent(InPercent);
}
