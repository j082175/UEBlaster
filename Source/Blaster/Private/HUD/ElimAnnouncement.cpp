// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ElimAnnouncement.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "HUD/ElimAnnouncementBox.h"

void UElimAnnouncement::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("ElimAnnouncement Tick"));

	AnnouncementScroll->ScrollToEnd();
}

void UElimAnnouncement::SetElimAnnouncementText(FString AttackerName, FString VictimName)
{
	ElimAnnouncementBox = CreateWidget<UElimAnnouncementBox>(this, ElimAnnouncementBoxClass);

	ElimAnnouncementBox->SetElimAnnouncementText(AttackerName, VictimName);
	if (ElimAnnouncementBox)
	{
		AnnouncementScroll->AddChild(ElimAnnouncementBox);
	}

	FTimerHandle ElimMsgTimer;
	GetWorld()->GetTimerManager().SetTimer(ElimMsgTimer, this, &ThisClass::RemoveScrollChild, ElimAnnouncementTime, false);
}

void UElimAnnouncement::RemoveScrollChild()
{
	if (AnnouncementScroll) AnnouncementScroll->GetChildAt(0)->RemoveFromParent();
}
