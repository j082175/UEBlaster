// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ChattingUI/ChatViewer.h"
#include "Components/ScrollBox.h"

void UChatViewer::NativeConstruct()
{
	Super::NativeConstruct();

}

void UChatViewer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ChatTextBox->ScrollToEnd();
}
