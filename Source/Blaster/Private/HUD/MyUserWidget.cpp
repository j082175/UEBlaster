// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyUserWidget.h"
#include "Interfaces/WidgetBindDelegateInterface.h"
#include "Blaster.h"



UMyUserWidget::UMyUserWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//AB_CALLLOG(LogABDisplay, Error, TEXT(""));

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
	GetWorld()->GetTimerManager().SetTimer(InitializeTimerHandle, this, &ThisClass::PollInitFunc, 0.2f, true);
}

void UMyUserWidget::PollInitFunc()
{
	//UE_LOG(LogTemp, Display, TEXT("PollInitFunc"));

	if (OwingActor.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("OwingActor Name : %s"), *OwingActor->GetName());

		IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(OwingActor);
		if (WBDI)
		{
			WBDI->IBindWidget(this);
			GetWorld()->GetTimerManager().ClearTimer(InitializeTimerHandle);
			InitializeTimerHandle.Invalidate();
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Finding OwingActor"));
		}
	}
	else
	{
		if (GetOwningPlayer())
		{
			if (IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(GetOwningPlayer()))
			{
				WBDI->IBindWidget(this);
				GetWorld()->GetTimerManager().ClearTimer(InitializeTimerHandle);
				InitializeTimerHandle.Invalidate();
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("IWidgetBindDelegateInterface Initializing"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Finding OwingPlayer"));
		}
	}
}

void UMyUserWidget::VisibilityChanged(ESlateVisibility InVisibility)
{
	PollInit();
}
