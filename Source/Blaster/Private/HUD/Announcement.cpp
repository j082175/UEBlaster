// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Announcement.h"
#include "Components/TextBlock.h"

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

void UAnnouncement::SetAnnouncementText(const FString& InString)
{
	AnnouncementText->SetText(FText::FromString(InString));
}

void UAnnouncement::SetInfoText(const FString& InString)
{
	InfoText->SetText(FText::FromString(InString));
}