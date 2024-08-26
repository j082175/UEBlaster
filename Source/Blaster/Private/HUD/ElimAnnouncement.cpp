// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ElimAnnouncement.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

void UElimAnnouncement::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UE_LOG(LogTemp, Display, TEXT("ElimAnnouncement Tick"));
}

void UElimAnnouncement::SetElimAnnouncementText(FString AttackerName, FString VictimName)
{
	FString ElimAnnouncementText = FString::Printf(TEXT("%s elimmed %s!"), *AttackerName, *VictimName);
	if (AnnouncementText)
	{
		AnnouncementText->SetText(FText::FromString(ElimAnnouncementText));
	}
}
