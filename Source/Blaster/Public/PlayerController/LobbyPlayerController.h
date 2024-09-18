// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/PlayerController.h"
#include "PlayerController/BasePlayerController.h"

#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ALobbyPlayerController : public ABasePlayerController
{
	GENERATED_BODY()
	ALobbyPlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;


	UFUNCTION(Server, Reliable)
	void ServerSpawnCharacter();

	// UI
private:




	// input
private:
	void ShowPauseMenu();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_QuitAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputMappingContext> IMC_Lobby;

private:

};
