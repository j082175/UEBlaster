// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyUserWidget.h"
#include "Interfaces/WidgetBindDelegateInterface.h"



UMyUserWidget::UMyUserWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PollInit();

	OnVisibilityChanged.AddUniqueDynamic(this, &ThisClass::VisibilityChanged);
}

void UMyUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("MyUserWidget Tick"));

	//UE_LOG(LogTemp, Display, TEXT("GetOwningPlayerPawn : %x"), OPawn.Get());
	//UE_LOG(LogTemp, Display, TEXT("GetOwningPlayerPawn : %x"), GetOwningPlayerPawn());


	//if (!OPawn.Get())
	//{
	//	//UE_LOG(LogTemp, Display, TEXT("Get PollInit"));
	//	PollInit();
	//	SetVisibility(ESlateVisibility::Hidden);
	//}
}

void UMyUserWidget::PollInit()
{
	//OPawn = GetOwningPlayerPawn();

	//FTimerHandle T;
	//GetWorld()->GetTimerManager().SetTimer(T, FTimerDelegate::CreateLambda([&]()
	//	{
	//		if (OwingActor.IsValid())
	//		{
	//			IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(OwingActor);
	//			if (WBDI)
	//			{
	//				WBDI->IBindWidget(this);
	//			}
	//			OPawn = OwingActor;
	//		}
	//		else if (GetOwningPlayer())
	//		{
	//			IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(GetOwningPlayer());
	//			if (WBDI)
	//			{
	//				WBDI->IBindWidget(this);
	//			}
	//			//OPawn = GetOwningPlayer();
	//		}
	//	}), 0.01f, false);

	if (OwingActor.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(InitializeTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(OwingActor);
				if (WBDI)
				{
					WBDI->IBindWidget(this);
					GetWorld()->GetTimerManager().ClearTimer(InitializeTimerHandle);
					InitializeTimerHandle.Invalidate();
				}
			}), 0.01f, true);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(InitializeTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(GetOwningPlayer());
				if (WBDI)
				{
					WBDI->IBindWidget(this);
					GetWorld()->GetTimerManager().ClearTimer(InitializeTimerHandle);
					InitializeTimerHandle.Invalidate();
				}
			}), 0.01f, true);
	}






}

void UMyUserWidget::VisibilityChanged(ESlateVisibility InVisibility)
{
	PollInit();
}
