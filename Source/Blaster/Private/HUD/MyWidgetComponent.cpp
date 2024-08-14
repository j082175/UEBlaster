// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyWidgetComponent.h"
#include "HUD/MyUserWidget.h"
//#include "Interfaces/WidgetBindDelegateInterface.h"

void UMyWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UMyUserWidget* UserWidget = Cast<UMyUserWidget>(GetWidget());
	if (UserWidget)
	{
		UserWidget->SetOwingActor(GetOwner());
	}

	//IWidgetBindDelegateInterface* WidgetOwner = Cast<IWidgetBindDelegateInterface>(GetOwner());
	//if (WidgetOwner)
	//{
	//	WidgetOwner->IBindOverheadWidget(this);
	//}
}
