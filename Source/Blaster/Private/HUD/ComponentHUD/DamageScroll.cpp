// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ComponentHUD/DamageScroll.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "HUD/ComponentHUD/DamageText.h"

UDamageScroll::UDamageScroll(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UDamageText> DamageTextClassRef(TEXT("/Game/A_Blaster/Blueprints/HUD/ComponentHUD/WBP_DamageText.WBP_DamageText_C"));
	DamageTextClass = DamageTextClassRef.Class;
	ensure(DamageTextClass);
}

void UDamageScroll::NativeConstruct()
{
	Super::NativeConstruct();

}

void UDamageScroll::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	DamageScroll->ScrollToEnd();
}

void UDamageScroll::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	switch (InVisibility)
	{
	case ESlateVisibility::Visible:
		break;
	case ESlateVisibility::Collapsed:
		DamageScroll->ClearChildren();
		break;
	case ESlateVisibility::Hidden:
		break;
	case ESlateVisibility::HitTestInvisible:
		break;
	case ESlateVisibility::SelfHitTestInvisible:
		break;
	default:
		break;
	}

}

void UDamageScroll::AddDamageText(int32 InDamage, const FLinearColor& InColor)
{
	DamageText = CreateWidget<UDamageText>(this, DamageTextClass);
	if (!DamageText.IsValid()) return;

	static int32 Sign = 1;
	float Range = 10.f;

	DamageText->SetDamageText(InDamage, InColor);

	float Result = FMath::RandRange(-Range, Range);

	FWidgetTransform T;
	T.Translation.X += Result * Sign;
	Sign *= -1;
	DamageText->SetRenderTransform(T);

	DamageScroll->AddChild(DamageText.Get());
	SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().ClearTimer(InvisibleTimer);
	InvisibleTimer.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(InvisibleTimer, this, &ThisClass::Invisible, InvisibleTime, false);
}

void UDamageScroll::Invisible()
{
	SetVisibility(ESlateVisibility::Collapsed);
	InvisibleTimer.Invalidate();
}
