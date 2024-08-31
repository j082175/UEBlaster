// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ElimAnnouncementBox.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

void UElimAnnouncementBox::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("ElimAnnouncement Tick"));
}

void UElimAnnouncementBox::SetElimAnnouncementText(FString AttackerName, FString VictimName)
{
	FSlateColor Red(FLinearColor(1.f, 0.13f, 0.19f));
	FSlateColor Green(FLinearColor(0.1f, 1.f, 0.f));

	if (AttackerName == TEXT("You"))
	{
		AttackerText->SetColorAndOpacity(Green);
	}
	else
	{
		AttackerText->SetColorAndOpacity(Red);
	}
	
	if (VictimName == TEXT("You"))
	{
		VictimText->SetColorAndOpacity(Green);
	}
	else
	{
		VictimText->SetColorAndOpacity(Red);
	}

	//FString ElimAnnouncementText = FString::Printf(TEXT("%s elimmed %s!"), *AttackerName, *VictimName);
	//AnnouncementText->SetText(FText::FromString(ElimAnnouncementText));

	AttackerText->SetText(FText::FromString(AttackerName));
	VictimText->SetText(FText::FromString(VictimName));

}

