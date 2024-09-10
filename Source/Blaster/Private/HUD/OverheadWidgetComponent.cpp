// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverheadWidgetComponent.h"
#include "HUD/OverheadWidget.h"
#include "Interfaces/TeamInterface.h"
#include "Blaster.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UOverheadWidgetComponent::UOverheadWidgetComponent()
{
}

void UOverheadWidgetComponent::PostLoad()
{
	Super::PostLoad();

	SetComponentTickInterval(10000.f);
}

// InitWidget() called faster than BeginPlay()
void UOverheadWidgetComponent::InitWidget()
{
	Super::InitWidget();

	AB_SUBLOG(LogABBeginPlay, Warning, TEXT("End"));
}

void UOverheadWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	AB_SUBLOG(LogABBeginPlay, Warning, TEXT("End"));

	InitTextColor();
}

void UOverheadWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	//if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(this, 0))
	//{
	//	if (USceneComponent* SC = PCM->GetTransformComponent())
	//	{
	//		FVector V = SC->GetComponentLocation();
	//		FRotator R = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), V);

	//		SetWorldRotation(R);
	//	}
	//}

	UE_LOG(LogTemp, Display, TEXT("UOverheadWidgetComponent::TickComponent"));
}

void UOverheadWidgetComponent::InitTextColor()
{
	GetWorld()->GetTimerManager().SetTimer(InitHandle, this, &ThisClass::InitTextColorFunc, 0.1f, true);
}

void UOverheadWidgetComponent::InitTextColorFunc()
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
		SetComponentTickEnabled(false);
	}
	else if (OverheadWidget && T1 && T2 && T1->IGetTeam() != T2->IGetTeam())
	{
		OverheadWidget->SetAllTextColor(Red);
		OverheadWidget->SetVisibility(ESlateVisibility::Collapsed);
		GetWorld()->GetTimerManager().ClearTimer(InitHandle);
		InitHandle.Invalidate();
		SetComponentTickEnabled(false);
	}
	else if (T2 && T2->IGetTeam() == ETeam::NoTeam)
	{
		GetWorld()->GetTimerManager().ClearTimer(InitHandle);
		InitHandle.Invalidate();
		SetComponentTickEnabled(false);
		return;
	}
	else
	{
		AB_CALLLOG(LogTemp, Log, TEXT("%s : Initializing"), *GetOwner()->GetName());
		//UE_LOG(LogTemp, Error, TEXT("Blaster: Failed ShowPlayerName"));
	}
}
