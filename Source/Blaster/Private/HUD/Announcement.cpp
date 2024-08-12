// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Announcement.h"
#include "Components/TextBlock.h"
#include "Interfaces/WidgetBindDelegateInterface.h"


void UAnnouncement::NativeConstruct()
{
	Super::NativeConstruct();

	IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(GetOwningPlayerPawn());

	if (WBDI)
	{
		WBDI->IBindOverheadWidget(this);
	}
}

void UAnnouncement::SetHUDAnnouncementCountdown(float CountdownTime)
{
	if (CountdownTime < 0.f)
	{
		WarmupTime->SetText(FText());
		return;
	}

	int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
	int32 Seconds = CountdownTime - Minutes * 60;

	FString CountdownText = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);
	WarmupTime->SetText(FText::FromString(CountdownText));
}