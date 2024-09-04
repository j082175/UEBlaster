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

void UMyWidgetComponent::PostLoad()
{
	Super::PostLoad();

	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UMyWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UE_LOG(LogTemp, Warning, TEXT("MyWidgetComponent tick"));

	if (GetWidget() && !GetWidget()->IsVisible())
	{
		//UE_LOG(LogTemp, Display, TEXT("Widget Hidden"));
		SetComponentTickEnabled(false);
	}


}

void UMyWidgetComponent::InitWidget()
{
	Super::InitWidget();
	SetComponentTickEnabled(true);

	UMyUserWidget* UserWidget = Cast<UMyUserWidget>(GetWidget());
	if (UserWidget)
	{
		UserWidget->SetOwingActor(GetOwner());
	}

	//IWidgetBindDelegateInterface* WidgetOwner = Cast<IWidgetBindDelegateInterface>(GetOwner());
	//if (WidgetOwner)
	//{
	//	WidgetOwner->IBindWidget(this);
	//}
}
