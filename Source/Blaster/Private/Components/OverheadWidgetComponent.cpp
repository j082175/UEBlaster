// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/OverheadWidgetComponent.h"
#include "HUD/OverheadWidget.h"
#include "Interfaces/TeamInterface.h"
#include "Blaster.h"

UOverheadWidgetComponent::UOverheadWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickInterval = 0.1f;


}

void UOverheadWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	InitTextColor();
}

void UOverheadWidgetComponent::InitTextColor()
{
	GetWorld()->GetTimerManager().SetTimer(InitHandle, FTimerDelegate::CreateLambda([&]()
		{
			FLinearColor Green(0.1f, 1.f, 0.f);
			FLinearColor Red(1.f, 0.13f, 0.19f);

			OverheadWidget = Cast<UOverheadWidget>(GetWidget());

			//ITeamInterface* T1 = Cast<ITeamInterface>(GetWorld()->GetFirstPlayerController()->GetPlayerState<APlayerState>());
			//ITeamInterface* T2 = Cast<ITeamInterface>(GetPlayerState<APlayerState>());

			ITeamInterface* T1 = Cast<ITeamInterface>(GetWorld()->GetFirstPlayerController()->GetPawn());
			ITeamInterface* T2 = Cast<ITeamInterface>(GetOwner());

			if (OverheadWidget && T1 && T2 && T1->IGetTeam() == T2->IGetTeam())
			{
				OverheadWidget->SetAllTextColor(Green);
				GetWorld()->GetTimerManager().ClearTimer(InitHandle);
				InitHandle.Invalidate();
			}
			else if (OverheadWidget && T1 && T2 && T1->IGetTeam() != T2->IGetTeam())
			{
				OverheadWidget->SetAllTextColor(Red);
				OverheadWidget->SetVisibility(ESlateVisibility::Collapsed);
				GetWorld()->GetTimerManager().ClearTimer(InitHandle);
				InitHandle.Invalidate();
			}
			else if (T2 && T2->IGetTeam() == ETeam::ET_NoTeam)
			{
				GetWorld()->GetTimerManager().ClearTimer(InitHandle);
				InitHandle.Invalidate();
				return;
			}
			else
			{
				AB_CALLLOG(LogTemp, Log, TEXT("%s : Initializing"), *GetOwner()->GetName());
				//UE_LOG(LogTemp, Error, TEXT("Blaster: Failed ShowPlayerName"));
			}

		}), 0.1f, true);
}
