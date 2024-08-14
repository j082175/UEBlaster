// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Animation/WidgetAnimation.h"
#include "Interfaces/WidgetBindDelegateInterface.h"


void UHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//IWidgetBindDelegateInterface* WidgetOwner = Cast<IWidgetBindDelegateInterface>(GetOwningPlayerPawn());
	//if (WidgetOwner)
	//{
	//	WidgetOwner->IBindOverheadWidget(this);
	//}


}

void UHpBarWidget::SetHpBar(float InCurrent, float InMax)
{
	HpBar->SetPercent(InCurrent / InMax);
}

void UHpBarWidget::SetShieldBar(float InCurrent, float InMax)
{
	//UE_LOG(LogTemp, Display, TEXT("%s"), *UEnum::GetDisplayValueAsText(GetOwningPlayer()->GetLocalRole()).ToString());
	//UE_LOG(LogTemp, Display, TEXT("SetShieldBar : %s"), *GetOwningPlayer()->GetName());
	ShieldBar->SetPercent(InCurrent / InMax);
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
