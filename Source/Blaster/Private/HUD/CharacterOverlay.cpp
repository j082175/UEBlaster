// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CharacterOverlay.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Interfaces/WidgetBindDelegateInterface.h"

#include "HUD/OverlayModules/AttributeStatus.h"
#include "HUD/OverlayModules/SkillBar.h"
#include "HUD/OverlayModules/WeaponStatus.h"
#include "HUD/CoolTimeAnnouncement.h"
#include "HUD/OverlayModules/PingStatus.h"
#include "Blaster.h"

//UCharacterOverlay::UCharacterOverlay(const FObjectInitializer& ObjectInitializer)
//	:Super(ObjectInitializer)
//{
//	AB_CALLLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
//}

void UCharacterOverlay::NativeConstruct()
{
	AB_CALLLOG(LogABBeginPlay, Warning, TEXT("%s"), TEXT("Begin"));
	Super::NativeConstruct();
	AB_CALLLOG(LogABBeginPlay, Warning, TEXT("%s"), TEXT("End"));

	SetVisibility(ESlateVisibility::Visible);

	//OnVisibilityChanged.AddUniqueDynamic(this, &ThisClass::VisibilityChanged);
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

	//AttributeStatus->SetCurrentTextBlock(AttributeStatus->CurrentSp, InCurrent);
	//AttributeStatus->SetMaxTextBlock(AttributeStatus->MaxSp, InMax);
	//AttributeStatus->SetProgressBar(AttributeStatus->SpBar, InCurrent / InMax);
	SkillBar->SetSpBar(InCurrent / InMax);
}

void UCharacterOverlay::SetParryGaugeBar(float InCurrent, float InMax)
{
	AttributeStatus->SetProgressBar(AttributeStatus->ParryGaugeBar, InCurrent / InMax);
}

void UCharacterOverlay::SetHUDScore(int32 Score)
{
	FString ScoreText = FString::Printf(TEXT("%d"), Score);
	ScoreAmount->SetText(FText::FromString(ScoreText));
}

void UCharacterOverlay::SetHUDDefeats(int32 Defeats)
{
	FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
	DefeatsAmount->SetText(FText::FromString(DefeatsText));
}

//void UCharacterOverlay::SetHUDWeaponAmmo(int32 CurrentAmmo)
//{
//	WeaponStatus->SetCurrentTextBlock(WeaponStatus->CurrentAmmo, CurrentAmmo);
//}
//
//void UCharacterOverlay::SetHUDCarriedAmmo(int32 CarriedAmmo)
//{
//	WeaponStatus->SetMaxTextBlock(WeaponStatus->MaxAmmo, CarriedAmmo);
//}
//
//void UCharacterOverlay::SetHUDWeaponType(EWeaponName InWeaponType)
//{
//	WeaponStatus->SetName(WeaponStatus->WeaponName, UEnum::GetDisplayValueAsText(InWeaponType).ToString());
//}

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

//void UCharacterOverlay::SetHUDGrenades(int32 Grenades)
//{
//	WeaponStatus->SetCurrentTextBlock(WeaponStatus->GrenadeNum, Grenades);
//}

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

void UCharacterOverlay::StartSkillAnim(ESkillAnimType InPrefix, int32 InIndex, float InPlaybackSpeed)
{
	UE_LOG(LogTemp, Display, TEXT("StartSkillAnim"));
	SkillBar->StartAnimation(InPrefix, InIndex, InPlaybackSpeed);
}

void UCharacterOverlay::SetSkillCost(int32 InIndex, const FString& InStr)
{
	SkillBar->SetSkillCost(InIndex, InStr);
}

void UCharacterOverlay::SetSoulCount(int32 InCount)
{
	SkillBar->SetSoulCount(InCount);
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
	WBP_PingStatus->HighPingWarning();
}

void UCharacterOverlay::StopHighPingWarning()
{
	WBP_PingStatus->StopHighPingWarning();
}

void UCharacterOverlay::UpdatePing(float InPing)
{
	WBP_PingStatus->UpdatePing(InPing);
}

void UCharacterOverlay::ShowCoolTimeAnnouncement(int32 InSkillIndex)
{
	CoolTimeAnnouncement->SetCoolTimeAnnouncementText(InSkillIndex);
}

//void UCharacterOverlay::SetCurrentAmmo(int32 InAmmo)
//{
//	WeaponStatus->SetCurrentAmmo(InAmmo);
//}
//
//void UCharacterOverlay::SetMaxAmmo(int32 InAmmo)
//{
//	WeaponStatus->SetMaxAmmo(InAmmo);
//}
//
//void UCharacterOverlay::SetWeaponName(EWeaponName InWeaponName)
//{
//	WeaponStatus->SetWeaponName(InWeaponName);
//}
//
//void UCharacterOverlay::SetGrenadeNum(int32 InGrenadeCount)
//{
//	WeaponStatus->SetGrenadeNum(InGrenadeCount);
//}

