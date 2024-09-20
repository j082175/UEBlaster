// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Blueprint/UserWidget.h"
#include "HUD/MyUserWidget.h"
#include "Item/Pickable/Weapon/Weapon.h"
#include "BlasterTypes/SkillAssistant.h"
#include "CharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UCharacterOverlay : public UMyUserWidget
{
	GENERATED_BODY()
	//UCharacterOverlay(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get);
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetHpBar(float InCurrent, float InMax);
	void SetShieldBar(float InCurrent, float InMax);
	void SetSpBar(float InCurrent, float InMax);
	void SetParryGaugeBar(float InCurrent, float InMax);
	void SetHUDScore(int32 Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDMatchCountdown(float CountdownTime);

	//void SetHUDWeaponAmmo(int32 CurrentAmmo);
	//void SetHUDCarriedAmmo(int32 CarriedAmmo);
	//void SetHUDWeaponType(EWeaponName InWeaponType);
	//void SetHUDGrenades(int32 Grenades);

	void SetHUDRedTeamScore(int32 RedScore);
	void SetHUDBlueTeamScore(int32 BlueScore);

	UFUNCTION()
	void StartSkillAnim(ESkillAnimType InPrefix, int32 InIndex, float InPlaybackSpeed);
	UFUNCTION()
	void SetSkillCost(int32 InIndex, const FString& InStr);
	UFUNCTION()
	void SetSoulCount(int32 InCount);
	UFUNCTION()
	void ShowCoolTimeAnnouncement(ESkillAssistant InSkillAssistant);

	//void SetCurrentAmmo(int32 InAmmo);
	//void SetMaxAmmo(int32 InAmmo);
	//void SetWeaponName(EWeaponName InWeaponName);
	//void SetGrenadeNum(int32 InGrenadeCount);
	//void SetGrenadeName()


	void HideTeamScores();
	void InitTeamScores();
		
	UFUNCTION()
	void HighPingWarning();
	UFUNCTION()
	void StopHighPingWarning();
	UFUNCTION()
	void UpdatePing(float InPing);



	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UWeaponStatus> WeaponStatus;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USkillBar> SkillBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UAttributeStatus> AttributeStatus;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UProgressBar> HealthBar;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UTextBlock> HealthText;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UProgressBar> ShieldBar;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UTextBlock> ShieldText;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UProgressBar> StaminaBar;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UTextBlock> StaminaText;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UProgressBar> ParryBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ScoreAmount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DefeatsAmount;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UTextBlock> WeaponAmmoAmount;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UTextBlock> CarriedAmmoAmount;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UTextBlock> WeaponType;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MatchCountdownText;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UTextBlock> GrenadesText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> RedTeamScore;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BlueTeamScore;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ScoreSpacerText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPingStatus> WBP_PingStatus;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCoolTimeAnnouncement> CoolTimeAnnouncement;

private:
	//void PollInit();

	//FTimerHandle H;
};
