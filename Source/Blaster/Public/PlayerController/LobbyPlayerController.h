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
public:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;


	UFUNCTION(Server, Reliable)
	void ServerSpawnCharacter();

	// UI
public:
	void CreateStartButton();
private:
	UPROPERTY()
	TSubclassOf<class UGameMenu> GameStartButtonClass;
	UPROPERTY()
	TObjectPtr<class UGameMenu> GameStartButton;



	// input
	void PressStarted();
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputMappingContext> IMC_Lobby;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> IA_PressStart;

private:

};
