// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BlasterGameState.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterGameState : public AGameState
{
	GENERATED_BODY()
public:
	ABlasterGameState();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE uint32 GetRedTeamScore() const { return RedTeamScore; }
	FORCEINLINE uint32 GetBlueTeamScore() const { return BlueTeamScore; }


	void UpdateTopScore(class ABlasterPlayerState* ScoringPlayer);

	UPROPERTY(Replicated)
	TArray<TObjectPtr<class ABlasterPlayerState>> TopScoringPlayers;

	// Team
	void RedTeamScores();
	void BlueTeamScores();

	UFUNCTION()
	void OnRep_RedTeamScore();

	UFUNCTION()
	void OnRep_BlueTeamScore();

	TArray<ABlasterPlayerState*> RedTeam;
	TArray<ABlasterPlayerState*> BlueTeam;

	UPROPERTY(ReplicatedUsing = OnRep_RedTeamScore)
	uint32 RedTeamScore = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_BlueTeamScore)
	uint32 BlueTeamScore = 0.f;


	void ChangeScoreBoard(const FString& InPlayerName, int32 InValue, bool IsScore);


private:


	uint32 TopScore = 0.f;


	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UObjectPoolComponent> ObjectPoolComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AActor> PlayableActor;

private:
};
