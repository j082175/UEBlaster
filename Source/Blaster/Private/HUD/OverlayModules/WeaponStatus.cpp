// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverlayModules/WeaponStatus.h"
#include "Components/TextBlock.h"
#include "Item/Pickable/Weapon/WeaponTypes.h"

void UWeaponStatus::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWeaponStatus::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


}

void UWeaponStatus::SetCurrentTextBlock(UTextBlock* InTextBlock, int32 InCurrentValue)
{
	InTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::Clamp(InCurrentValue, 0, InCurrentValue))));
}

void UWeaponStatus::SetMaxTextBlock(UTextBlock* InTextBlock, int32 InMaxValue)
{
	InTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), InMaxValue)));
}

void UWeaponStatus::SetName(UTextBlock* InTextBlock, const FString& InName)
{
	InTextBlock->SetText(FText::FromString(InName));
}

void UWeaponStatus::SetCurrentAmmo(int32 InAmmo)
{
	CurrentAmmo->SetText(FText::FromString(FString::FromInt(InAmmo)));
}

void UWeaponStatus::SetMaxAmmo(int32 InAmmo)
{
	MaxAmmo->SetText(FText::FromString(FString::FromInt(InAmmo)));
}

void UWeaponStatus::SetWeaponName(EWeaponName InWeaponName)
{
	WeaponName->SetText(FText::FromString(*UEnum::GetDisplayValueAsText(InWeaponName).ToString()));
}

void UWeaponStatus::SetGrenadeNum(int32 InGrenadeCount)
{
	GrenadeNum->SetText(FText::FromString(FString::FromInt(InGrenadeCount)));
}
