// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

namespace MatchState
{
	extern BLASTER_API const FName Cooldown; // Match duration has been reached. Display winner and begin cooldown timer.

}

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ABlasterGameMode();
	virtual void PostLogin(APlayerController* PlayerController) override;

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE float GetWarmupTime() const { return WarmupTime; }
	FORCEINLINE float GetMatchTime() const { return MatchTime; }
	FORCEINLINE float GetLevelStartingTime() const { return LevelStartingTime; }
	FORCEINLINE float GetCountdownTime() const { return CountdownTime; }
	FORCEINLINE float GetCooldownTime() const { return CooldownTime; }


	void PlayerLeftGame(class ABlasterPlayerState* PlayerLeaving);
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);

	void CameraShake();
protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleLeavingMap() override;
	virtual void HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver) override;
	virtual void Logout(AController* Exiting) override;
	virtual void Destroyed() override;
public:
	virtual void PlayerEliminated(class ACharacterBase* ElimmedCharacter, class ABlasterPlayerController* VictimController, class ABlasterPlayerController* AttackerController);
	virtual void RequestRespawn(class ACharacter* ElimmedCharacter, class AController* ElimmedController);

	void SendChatMsg(const FText& Text, const FString& PlayerName);

	bool bTeamsMatch = false;

private:
	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 5.f;
	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;
	float LevelStartingTime = 0.f;
	float CountdownTime = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.f;


	//UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	//TObjectPtr<class UObjectPoolComponent> ObjectPoolComponent;

};
