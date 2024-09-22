// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void SpawnCharacter();

	uint8 bRestartGameCheck : 1 = false;
private:
	UPROPERTY()
	TObjectPtr<class AActor> LastPlayer;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSoftClassPtr<AActor> Player;


};
