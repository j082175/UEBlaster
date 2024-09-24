// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/PlayerController.h"
#include "PlayerController/BasePlayerController.h"
#include "Interfaces/WidgetBindDelegateInterface.h"
#include "Item/Pickable/Weapon/WeaponTypes.h"
#include "Interfaces/TeamInterface.h"
// Enums
#include "Types/SlateEnums.h"
#include "BlasterTypes/Team.h"
#include "HUD/ScoreBoard/ScoreBoardText.h"

#include "BlasterPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FHighPingDelegate, bool /*bPingTooHigh*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPingChangedDelegate, float /*Ping*/);
DECLARE_DELEGATE(FOnPingAnimStartedDelegate);
DECLARE_DELEGATE(FOnPingAnimStoppedDelegate);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchCountdownDelegate, float);
DECLARE_DELEGATE_OneParam(FOnRedTeamScoreChangedDelegate, int32);
DECLARE_DELEGATE_OneParam(FOnBlueTeamScoreChangedDelegate, int32);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnScoreChangedDelegate, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDefeatsChangedDelegate, int32);


DECLARE_DELEGATE_OneParam(FOnAnnouncementCountdownChangedDelegate, float);
DECLARE_DELEGATE_OneParam(FOnAnnouncementTextChangedDelegate, const FString&);
DECLARE_DELEGATE_OneParam(FOnAnnouncementInfoTextChangedDelegate, const FString&);
DECLARE_DELEGATE_OneParam(FOnAnnouncementVisibilityChangedDelegate, ESlateVisibility);
DECLARE_DELEGATE(FOnCharacterOverlayRemovedDelegate);


/**
 *
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public ABasePlayerController, public IWidgetBindDelegateInterface
{
	GENERATED_BODY()
public:
	ABlasterPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_Pawn() override;

	virtual void IBindWidget(class UUserWidget* InUserWidget) override;


	FORCEINLINE class ABlasterHUD* GetBlasterHUD() const { return BlasterHUD; }
	FORCEINLINE float GetWarmUpTime() const { return WarmupTime; }

	void PollInit(APawn* InPawn);
protected:
	virtual void SetupInputComponent() override;

public:
	void UpdateHUDHealth();
	void UpdateHUDShield();
	void UpdateHUDAmmo();

	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDShield(float Shield, float MaxShield);
	void SetHUDSp(float InSp, float InMaxSp);
	void SetHUDParryGauge(float InP, float InMaxP);

	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 CurrentAmmo);
	void SetHUDCarriedAmmo(int32 CarriedAmmo);
	void SetHUDWeaponType(EWeaponName InWeaponType);
	void SetHUDMatchCountdown(float CountdownTime);
	void SetHUDAnnouncementCountdown(float CountdownTime);
	void SetHUDGrenades(int32 Grenades);

	void SetHUDRedTeamScore(int32 RedScore);
	void SetHUDBlueTeamScore(int32 BlueScore);
	void HideTeamScores();
	void InitTeamScores();
	virtual float GetServerTime(); // Synced with server world clock

	void SetHUDTime();
	void CheckTimeSync(float DeltaTime);

private:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	//TObjectPtr<class UCharacterOverlay> CharacterOverlay;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class ABlasterGameMode> BlasterGameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class ABlasterCharacter> BlasterCharacter;



protected:

	// Requests the current server time, passing in the client's time when the request was sent
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	// Reports the current server time to the client in response to ServerRequestServerTime
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);


	virtual void ReceivedPlayer() override; // Sync with server clock as soon as possible




	float ClientServerDelta = 0.f; // Difference between client and server time
	UPROPERTY(EditAnywhere, Category = "Time")
	float TimeSyncFrequency = 5.f;
	float TimeSyncRunningTime = 0.f;



	// Pings
public:
	FHighPingDelegate HighPingDelegate;
	FOnPingChangedDelegate OnPingChanged;
	FOnPingAnimStartedDelegate OnPingAnimStarted;
	FOnPingAnimStoppedDelegate OnPingAnimStopped;
	FOnMatchCountdownDelegate MatchCountdown;

	FOnRedTeamScoreChangedDelegate OnRedTeamScoreChanged;
	FOnBlueTeamScoreChangedDelegate OnBlueTeamScoreChanged;

	FOnScoreChangedDelegate OnScoreChanged;
	FOnDefeatsChangedDelegate OnDefeatsChanged;

	FOnAnnouncementCountdownChangedDelegate OnAnnouncementCountdownChanged;
	FOnAnnouncementTextChangedDelegate OnAnnouncementTextChanged;
	FOnAnnouncementInfoTextChangedDelegate OnAnnouncementInfoTextChanged;
	FOnAnnouncementVisibilityChangedDelegate OnAnnouncementVisibilityChanged;
	FOnCharacterOverlayRemovedDelegate OnCharacterOverlayRemoved;

	void CheckPing(float DeltaTime);

protected:
	UFUNCTION(Server, Reliable)
	void ServerReportPingStatus(bool bHighPing);
	void HighPingWarning();
	void StopHighPingWarning();


	float HighPingRunningTime = 0.f;
	UPROPERTY(EditAnywhere)
	float HighPingDuration = 5.f;
	UPROPERTY(EditAnywhere)
	float CheckPingFrequency = 20.f;
	UPROPERTY(EditAnywhere)
	float HighPingThreshold = 200.f;
	UPROPERTY(EditAnywhere)
	float PingAnimationRunningTime = 0.f;

private:
	UPROPERTY(ReplicatedUsing = OnRep_ShowTeamScores)
	bool bShowTeamScores = false;

	UFUNCTION()
	void OnRep_ShowTeamScores();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<ABlasterHUD> BlasterHUD;

public:
	void OnMatchStateSet(FName State, bool bTeamsMatch = false);
	void HandleMatchHasStarted(bool bTeamsMatch = false);
	void HandleCooldown();

	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();
private:
	void OnMatchStateSetFunc(bool bTeamsMatch = false);
	UFUNCTION()
	void OnRep_MatchState();



	UFUNCTION(Client, Reliable)
	void ClientJoinMidgame(FName StateOfMatch, float InWarmupTime, float InMatchTime, float StartingTime, float Cooldown);

	float LevelStartingTime = 0.f;
	float MatchTime = 0.f;
	float WarmupTime = 0.f;
	uint32 CountdownInt = 0;
	UPROPERTY(ReplicatedUsing = OnRep_MatchState, VisibleAnywhere, meta = (AllowPrivateAccess = true))
	FName MatchState;

	float CooldownTime = 0.f;


	// Check Initialize Once
private:
	uint8 bInitializedGrenades : 1 = false;
	uint8 bInitializedCarriedAmmo : 1 = false;
	uint8 bInitializedCurrentAmmo : 1 = false;
	uint8 bInitializedHealth : 1 = false;
	uint8 bInitializedShield : 1 = false;


public:
	float SingleTripTime = 0.f;

private:
	void ShowReturnToMainMenu();

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> ReturnToMainMenuWidget;

	UPROPERTY()
	class UReturnToMainMenu* ReturnToMainMenu;

	bool bReturnToMainMenuOpen = false;

	// input
private:


	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputAction> IA_Tab;

public:
	void BroadcastElim(APlayerState* Attacker, APlayerState* Victim);

protected:
	UFUNCTION(Client, Reliable)
	void ClientElimAnnouncement(APlayerState* Attacker, APlayerState* Victim);

	FString GetInfoText(const TArray<class ABlasterPlayerState*>& Players);
	FString GetTeamsInfoText(class ABlasterGameState* BlasterGameState);


	// Chatting
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UChatSystemComponent> ChatSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_Chat;


	// For hud
public:
	void PlayHitNoticeAnim(const FString& InPrefix);
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> HitNoticeClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UUserWidget* HitNotice;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TMap<FString, class UWidgetAnimation*> FindWidgetAnimation;


private:
	void CharacterOverlayReset();

	virtual void OnRep_PlayerState() override;


private:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UScoreBoardComponent> ScoreBoardComponent;
};
