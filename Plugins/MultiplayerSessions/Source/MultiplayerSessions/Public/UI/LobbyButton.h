// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyButton.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API ULobbyButton : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ButtonText;
};
