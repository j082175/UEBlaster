// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyWidgetComponent.h"
#include "HUD/MyUserWidget.h"

void UMyWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UMyUserWidget* UserWidget = Cast<UMyUserWidget>(GetWidget());
	if (UserWidget)
	{
		UserWidget->SetOwingActor(GetOwner());
	}
}
