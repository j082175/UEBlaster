// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyUserWidget.h"
#include "Interfaces/WidgetBindDelegateInterface.h"

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PollInit();
	

	EWidgetTickFrequency T = GetDesiredTickFrequency();

	int a = 1;
}

void UMyUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("GetOwningPlayerPawn : %x"), OPawn.Get());
	//UE_LOG(LogTemp, Display, TEXT("GetOwningPlayerPawn : %x"), GetOwningPlayerPawn());


	if (!OPawn.Get())
	{
		//UE_LOG(LogTemp, Display, TEXT("Get PollInit"));
		PollInit();
	}
}

void UMyUserWidget::PollInit()
{
	OPawn = GetOwningPlayerPawn();

	FTimerHandle T;
	GetWorld()->GetTimerManager().SetTimer(T, FTimerDelegate::CreateLambda([&]()
		{
			if (OwingActor.IsValid())
			{
				IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(OwingActor);
				if (WBDI)
				{
					WBDI->IBindOverheadWidget(this);
				}
				OPawn = OwingActor;
			}
			else if (GetOwningPlayer())
			{
				IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(GetOwningPlayer());
				if (WBDI)
				{
					WBDI->IBindOverheadWidget(this);
				}
				//OPawn = GetOwningPlayer();
			}
		}), 0.01f, false);
}
