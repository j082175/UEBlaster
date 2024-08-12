// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighPingDelegate, bool, bPingTooHigh);


USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	FHUDPackage()
		: CrosshairSpread(0.f), CrosshairsColor(FLinearColor::Black)
	{
	}
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> CrosshairCenter;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> CrosshairLeft;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> CrosshairRight;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> CrosshairTop;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> CrosshairBottom;
	UPROPERTY(EditAnywhere)
	float CrosshairSpread;
	UPROPERTY(EditAnywhere)
	FLinearColor CrosshairsColor;
};

/**
 *
 */
UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()
public:
	ABlasterHUD();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;


public:
	virtual void DrawHUD() override;
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	UPROPERTY()
	class UAnnouncement* Announcement;

	void AddAnnouncement();
	void AddElimAnnouncement(FString Attacker, FString Victim);
	void AddCharacterOverlay();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class ABlasterCharacter> BlasterCharacter;

	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

public:
	UFUNCTION(BlueprintCallable)
	void DrawCrosshairs();
private:
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = true))
	void DrawCrosshair(class UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor InCrosshairsColor);

private:
	UFUNCTION()
	void ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove);

	UPROPERTY()
	class APlayerController* OwingPlayer;

	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FHUDPackage HUDPackage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float CrosshairSpreadMax = 16.f;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> AnnouncementClass;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UElimAnnouncement> ElimAnnouncementClass;

	UPROPERTY(EditAnywhere)
	float ElimAnnouncementTime = 3.f;

	UPROPERTY()
	TArray<TObjectPtr<UElimAnnouncement>> ElimMessages;



//	void SetHUDTime();
//	float LevelStartingTime = 0.f;
//	float MatchTime = 0.f;
//	float WarmupTime = 0.f;
//	uint32 CountdownInt = 0;
//
//
//	UFUNCTION()
//	void OnRep_MatchState();
//	void OnMatchStateSetFunc(bool bTeamsMatch = false);
//
//	void HandleMatchHasStarted(bool bTeamsMatch);
//	void HandleCooldown();
//
//
//	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
//	FName MatchState;
//
//	float CooldownTime = 0.f;
//
//	float GetServerTime();
//
//	float ClientServerDelta = 0.f; // Difference between client and server time
//	UPROPERTY(EditAnywhere, Category = "Time")
//	float TimeSyncFrequency = 5.f;
//	float TimeSyncRunningTime = 0.f;
//
//	void CheckTimeSync(float DeltaTime);
//	void CheckPing(float DeltaTime);
//
//
//	// Requests the current server time, passing in the client's time when the request was sent
//	UFUNCTION(Server, Reliable)
//	void ServerRequestServerTime(float TimeOfClientRequest);
//
//	// Reports the current server time to the client in response to ServerRequestServerTime
//	UFUNCTION(Client, Reliable)
//	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);
//
//	float SingleTripTime = 0.f;
//
//	float HighPingRunningTime = 0.f;
//	UPROPERTY(EditAnywhere)
//	float HighPingDuration = 5.f;
//	UPROPERTY(EditAnywhere)
//	float CheckPingFrequency = 20.f;
//	UPROPERTY(EditAnywhere)
//	float HighPingThreshold = 50.f;
//	UPROPERTY(EditAnywhere)
//	float PingAnimationRunningTime = 0.f;
//
//	UFUNCTION(Server, Reliable)
//	void ServerReportPingStatus(bool bHighPing);
//
//	UPROPERTY(ReplicatedUsing = OnRep_ShowTeamScores)
//	bool bShowTeamScores = false;
//
//	UFUNCTION()
//	void OnRep_ShowTeamScores();
//
//	FString GetTeamsInfoText(class ABlasterGameState* BlasterGameState);
//	FString GetInfoText(const TArray<class ABlasterPlayerState*>& Players);
//
//public:
//	FHighPingDelegate HighPingDelegate;
//
//
//	UFUNCTION(Client, Reliable)
//	void ClientJoinMidgame(FName StateOfMatch, float InWarmupTime, float InMatchTime, float StartingTime, float Cooldown);
//
//	UFUNCTION(Server, Reliable)
//	void ServerCheckMatchState();
//
//	void OnMatchStateSet(FName State, bool bTeamsMatch = false);
};
