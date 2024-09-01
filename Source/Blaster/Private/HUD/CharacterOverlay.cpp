// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CharacterOverlay.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Interfaces/WidgetBindDelegateInterface.h"

#include "HUD/OverlayModules/AttributeStatus.h"
#include "HUD/OverlayModules/SkillBar.h"
#include "HUD/OverlayModules/WeaponStatus.h"


void UCharacterOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Visible);

}

void UCharacterOverlay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("CharacterOverlay Tick"));
	//PollInit();

}

void UCharacterOverlay::SetHpBar(float InCurrent, float InMax)
{
	//UE_LOG(LogTemp, Display, TEXT("SetHpBar : %f"), InCurrent / InMax);
	//HealthBar->SetPercent(InCurrent / InMax);
	//HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(FMath::Clamp((InCurrent), 0.f, InMax)), (int)InMax)));

	AttributeStatus->SetCurrentTextBlock(AttributeStatus->CurrentHp, InCurrent);
	AttributeStatus->SetMaxTextBlock(AttributeStatus->MaxHp, InMax);
	AttributeStatus->SetProgressBar(AttributeStatus->HpBar, InCurrent / InMax);
}

void UCharacterOverlay::SetShieldBar(float InCurrent, float InMax)
{
	//ShieldBar->SetPercent(InCurrent / InMax);
	//ShieldText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(FMath::Clamp((InCurrent), 0.f, InMax)), (int)InMax)));

	AttributeStatus->SetCurrentTextBlock(AttributeStatus->CurrentShield, InCurrent);
	AttributeStatus->SetMaxTextBlock(AttributeStatus->MaxShield, InMax);
	AttributeStatus->SetProgressBar(AttributeStatus->ShieldBar, InCurrent / InMax);
}

void UCharacterOverlay::SetSpBar(float InCurrent, float InMax)
{
	//StaminaBar->SetPercent(InCurrent / InMax);
	//StaminaText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(FMath::Clamp((InCurrent), 0.f, InMax)), (int)InMax)));

	AttributeStatus->SetCurrentTextBlock(AttributeStatus->CurrentSp, InCurrent);
	AttributeStatus->SetMaxTextBlock(AttributeStatus->MaxSp, InMax);
	AttributeStatus->SetProgressBar(AttributeStatus->SpBar, InCurrent / InMax);
}

void UCharacterOverlay::SetParryGaugeBar(float InCurrent, float InMax)
{
	AttributeStatus->SetProgressBar(AttributeStatus->ParryGaugeBar, InCurrent / InMax);
}

void UCharacterOverlay::SetHUDScore(float Score)
{
	FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
	ScoreAmount->SetText(FText::FromString(ScoreText));
}

void UCharacterOverlay::SetHUDDefeats(int32 Defeats)
{
	FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
	DefeatsAmount->SetText(FText::FromString(DefeatsText));
}

void UCharacterOverlay::SetHUDWeaponAmmo(int32 CurrentAmmo)
{
	//FString AmmoText = FString::Printf(TEXT("%d"), CurrentAmmo);
	//WeaponAmmoAmount->SetText(FText::FromString(AmmoText));

	WeaponStatus->SetCurrentTextBlock(WeaponStatus->CurrentAmmo, CurrentAmmo);

	//bInitializedCurrentAmmo = true;
}

void UCharacterOverlay::SetHUDCarriedAmmo(int32 CarriedAmmo)
{
	//UE_LOG(LogTemp, Display, TEXT("SetHUDCarriedAmmo"));
	//FString AmmoText = FString::Printf(TEXT("%d"), CarriedAmmo);
	//CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	// 

	WeaponStatus->SetMaxTextBlock(WeaponStatus->MaxAmmo, CarriedAmmo);
	// 
	//bInitializedCarriedAmmo = true;
}

void UCharacterOverlay::SetHUDWeaponType(EWeaponName InWeaponType)
{
	//UE_LOG(LogTemp, Display, TEXT("SetHUDWeaponType"));
	//FText WeaponTypeText = UEnum::GetDisplayValueAsText(InWeaponType);
	//WeaponType->SetText(WeaponTypeText);

	WeaponStatus->SetName(WeaponStatus->WeaponName, UEnum::GetDisplayValueAsText(InWeaponType).ToString());
}

void UCharacterOverlay::SetHUDMatchCountdown(float CountdownTime)
{
	if (CountdownTime < 0.f)
	{
		MatchCountdownText->SetText(FText());
		return;
	}

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Red, FString::Printf(TEXT("SetHUDMatchCountdown : %f"), CountdownTime));
	//}

	int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
	int32 Seconds = CountdownTime - Minutes * 60;

	FString CountdownText = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);
	MatchCountdownText->SetText(FText::FromString(CountdownText));
}

void UCharacterOverlay::SetHUDGrenades(int32 Grenades)
{
	//UE_LOG(LogTemp, Warning, TEXT("SetHUDGrenades"));
	//if (GEngine) GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Magenta, FString::Printf(TEXT("SetHUDGrenades")));

	//FString GT = FString::Printf(TEXT("%d"), Grenades);
	//GrenadesText->SetText(FText::FromString(GT));

	WeaponStatus->SetCurrentTextBlock(WeaponStatus->GrenadeNum, Grenades);


	//bInitializedGrenades = true;
}

void UCharacterOverlay::SetHUDRedTeamScore(int32 RedScore)
{
	FString ScoreText = FString::Printf(TEXT("%d"), RedScore);

	RedTeamScore->SetText(FText::FromString(ScoreText));
}

void UCharacterOverlay::SetHUDBlueTeamScore(int32 BlueScore)
{
	FString ScoreText = FString::Printf(TEXT("%d"), BlueScore);

	BlueTeamScore->SetText(FText::FromString(ScoreText));
}

void UCharacterOverlay::StartCoolTimeAnim(const FString& InPrefix, int32 InIndex, float InPlaybackSpeed)
{
	UE_LOG(LogTemp, Display, TEXT("StartCoolTimeAnim"));
	SkillBar->StartAnimation(InPrefix, InIndex, InPlaybackSpeed);
}

void UCharacterOverlay::HideTeamScores()
{
	RedTeamScore->SetText(FText());
	BlueTeamScore->SetText(FText());
	//ScoreSpacerText->SetText(FText());
}

void UCharacterOverlay::InitTeamScores()
{
	FString Zero(TEXT("0"));
	FString Spacer(TEXT("|"));

	RedTeamScore->SetText(FText::FromString(Zero));
	BlueTeamScore->SetText(FText::FromString(Zero));
	//ScoreSpacerText->SetText(FText::FromString(Spacer));
}

void UCharacterOverlay::HighPingWarning()
{
	HighPingImage->SetOpacity(1.f);
	PlayAnimation(HighPingAnimation, 0.f, 10);
}

void UCharacterOverlay::StopHighPingWarning()
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	HighPingImage->SetOpacity(0.f);
	if (IsAnimationPlaying(HighPingAnimation))
	{
		StopAnimation(HighPingAnimation);
	}
}

void UCharacterOverlay::PollInit()
{
	IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(GetOwningPlayer());
	if (WBDI)
	{
		WBDI->IBindOverheadWidget(this);
	}
}
