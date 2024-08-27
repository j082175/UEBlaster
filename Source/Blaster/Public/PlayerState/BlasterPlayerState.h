// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BlasterTypes/Team.h"
#include "BlasterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void OnRep_Score() override;

	FORCEINLINE ETeam GetTeam() const { return Team; }
	void SetTeam(ETeam TeamToSlot);

	void AddToScore(float ScoreAmount);
	UFUNCTION()
	virtual void OnRep_Defeats();
	void AddToDefeats(int32 DefeatsAmount);
private:
	void SetHUDScore(float InScore);
	void SetDefeatsScore(int32 DefeatsAmount);
private:
	UPROPERTY()
	class ABlasterCharacter* Character;
	UPROPERTY()
	class ABlasterPlayerController* Controller;
	

	UPROPERTY(ReplicatedUsing = OnRep_Defeats, VisibleAnywhere, Category = "Score", meta = (AllowPrivateAccess = true))
	int32 Defeats;

	UFUNCTION()
	void OnRep_Team();

	UPROPERTY(ReplicatedUsing = OnRep_Team, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	ETeam Team = ETeam::ET_NoTeam;
};
