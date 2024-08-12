// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Animation/WidgetAnimation.h"
#include "Interfaces/WidgetBindDelegateInterface.h"


void UHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IWidgetBindDelegateInterface* WidgetOwner = Cast<IWidgetBindDelegateInterface>(OwingActor);
	if (WidgetOwner)
	{
		WidgetOwner->IBindOverheadWidget(this);
	}
}

void UHpBarWidget::SetHpBar(float InCurrent, float InMax)
{
	HpBar->SetPercent(InCurrent / InMax);
}

void UHpBarWidget::SetShieldBar(float Percentage)
{
	ShieldBar->SetPercent(Percentage);
}

void UHpBarWidget::SetParryBar(float InCurrent, float InMax)
{
	ParryGauge->SetPercent(InCurrent / InMax);
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
