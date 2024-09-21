// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/BlasterGameMode.h"
#include "BlasterTypes/Team.h"

#include "TeamsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ATeamsGameMode : public ABlasterGameMode
{
	GENERATED_BODY()
public:
	ATeamsGameMode();
	//virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* PlayerController) override;
	virtual void Logout(AController* Exiting) override;
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage) override;
	virtual void PlayerEliminated(class ACharacterBase* ElimmedCharacter, class ABlasterPlayerController* VictimController, class ABlasterPlayerController* AttackerController) override;

	void ChangeScoreBoard(const FString& InPlayerName, int32 InValue, bool IsScore);
protected:

	virtual void HandleMatchHasStarted() override; // error occured!


	void RemoveAllControllerScoreBoard(const FString& InRemoveTarget);

};
