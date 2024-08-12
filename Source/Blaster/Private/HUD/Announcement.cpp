// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Announcement.h"
#include "Interfaces/WidgetBindDelegateInterface.h"


void UAnnouncement::NativeConstruct()
{
	Super::NativeConstruct();

	IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(GetOwningPlayerPawn());

	if (WBDI)
	{
		WBDI->IBindOverheadWidget(this);
	}
}
