// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverlayModules/SkillBar.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Animation/WidgetAnimation.h"
#include "Blaster/Blaster.h"

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