// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CharacterOverlay.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Interfaces/WidgetBindDelegateInterface.h"

void UCharacterOverlay::NativeConstruct()
{
	Super::NativeConstruct();


	if (GetOwningPlayerPawn())
	{
		//if (OwingActor) UE_LOG(LogTemp, Warning, TEXT("HUD : %s"), *OwingActor->GetOwner()->GetName());
		if (OwingActor) UE_LOG(LogTemp, Warning, TEXT("HUD : %s"), *GetOwningPlayerPawn()->GetName());

		//IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(OwingActor->GetOwner());
		IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(GetOwningPlayer());

		if (WBDI)
		{
			WBDI->IBindOverheadWidget(this);
		}
	}

	//if (OwingActor)
	//{
	//	if (OwingActor) UE_LOG(LogTemp, Warning, TEXT("HUD : %s"), *OwingActor->GetName());

	//	IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(OwingActor);
	//	if (WBDI)
	//	{
	//		WBDI->IBindOverheadWidget(this);
	//	}
	//}

}

void UCharacterOverlay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("GetOwningPlayerPawn : %x"), GetOwningPlayerPawn());

	//ScoreAmount->SetText(FText::FromString(FString::Printf(TEXT("%f"), InDeltaTime)));
}

void UCharacterOverlay::Test(float DeltaTime)
{
	if (ScoreAmount) ScoreAmount->SetText(FText::FromString(FString::Printf(TEXT("%f"), DeltaTime)));
}

void UCharacterOverlay::SetHpBar(float InCurrent, float InMax)
{
	UE_LOG(LogTemp, Display, TEXT("SetHpBar : %f"), InCurrent / InMax);
	HealthBar->SetPercent(InCurrent / InMax);
	HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(FMath::Clamp((InCurrent), 0.f, InMax)), (int)InMax)));

}
