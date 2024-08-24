// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyWidgetComponent.h"
#include "HUD/MyUserWidget.h"
//#include "Interfaces/WidgetBindDelegateInterface.h"

UMyWidgetComponent::UMyWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickInterval = 0.1f;
}

void UMyWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UE_LOG(LogTemp, Warning, TEXT("MyWidgetComponent tick"));
}

void UMyWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UMyUserWidget* UserWidget = Cast<UMyUserWidget>(GetWidget());
	if (UserWidget)
	{
		UserWidget->SetOwingActor(GetOwner());
	}

	//SetComponentTickEnabled(false);
	SetComponentTickInterval(1.f);

	//IWidgetBindDelegateInterface* WidgetOwner = Cast<IWidgetBindDelegateInterface>(GetOwner());
	//if (WidgetOwner)
	//{
	//	WidgetOwner->IBindOverheadWidget(this);
	//}
}
