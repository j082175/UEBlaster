// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyUserWidget.h"
#include "Interfaces/WidgetBindDelegateInterface.h"

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PollInit();
}

void UMyUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("GetOwningPlayerPawn : %x"), OPawn.Get());

	if (!OPawn.Get())
	{
		PollInit();
	}
}

void UMyUserWidget::PollInit()
{
	FTimerHandle T;
	GetWorld()->GetTimerManager().SetTimer(T, FTimerDelegate::CreateLambda([&]()
		{
			if (GetOwningPlayerPawn())
			{
				IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(GetOwningPlayerPawn());
				if (WBDI)
				{
					WBDI->IBindOverheadWidget(this);
				}
				OPawn = GetOwningPlayerPawn();
			}
		}), 0.01f, false);
}
