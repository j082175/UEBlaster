// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverlayModules/SkillBar.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Animation/WidgetAnimation.h"

void USkillBar::NativeConstruct()
{
	Super::NativeConstruct();

	FindWidgetAnimationName();
}

void USkillBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void USkillBar::StartAnimation(const FString& InPrefix, int32 InIndex, float InPlaybackSpeed)
{
	FString Str = FString::Printf(TEXT("%sCoolTimeAnim%d_INST"), *InPrefix, InIndex);
	//UE_LOG(LogTemp, Display, TEXT("%s"), *Str);

	if (FindWidgetAnimation.Contains(Str))
	{
		UWidgetAnimation* WidgetAnimation = FindWidgetAnimation[Str];

		if (WidgetAnimation)
		{
			PlayAnimation(WidgetAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f / InPlaybackSpeed);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("USkillBar::StartAnimation Find WidgetAnimation Failed!!!"));
		}
	}
}

void USkillBar::FindWidgetAnimationName()
{
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();

	FindWidgetAnimation.Empty();

	for (int i = 0; i < WidgetClass->Animations.Num(); i++)
	{
		PlayAnimation(WidgetClass->Animations[i], 0.f);
		FString Name = WidgetClass->Animations[i]->GetName();
		FindWidgetAnimation.Add(Name, WidgetClass->Animations[i]);
	}
}
