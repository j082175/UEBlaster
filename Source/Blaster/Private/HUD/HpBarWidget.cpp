// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Animation/WidgetAnimation.h"


void UHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHpBarWidget::SetHpBar(float Percentage)
{
	HpBar->SetPercent(Percentage);
}

void UHpBarWidget::SetShieldBar(float Percentage)
{
	ShieldBar->SetPercent(Percentage);
}

void UHpBarWidget::SetParryBar(float Percentage)
{
	ParryGauge->SetPercent(Percentage);
}

void UHpBarWidget::ParryGaugeAnimStart(bool InCheck)
{
	if (InCheck)
	{
		//UE_LOG(LogTemp, Display, TEXT("ParryGaugeAnimation Start"));
		PlayAnimation(ParryGaugeAnimationStart, 0.f, 10);
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("ParryGaugeAnimation Stop"));
		StopAnimation(ParryGaugeAnimationStart);
		PlayAnimation(ParryGaugeAnimationEnd);
	}
}
