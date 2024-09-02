// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/OverheadWidgetComponent.h"
#include "HUD/OverheadWidget.h"

UOverheadWidgetComponent::UOverheadWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickInterval = 0.1f;


}

void UOverheadWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	OverheadWidget = Cast<UOverheadWidget>(GetWidget());
}
