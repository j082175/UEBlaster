// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/CommonPCFuncInterface.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABasePlayerController : public APlayerController, public ICommonPCFuncInterface
{
	GENERATED_BODY()
public:
	ABasePlayerController();
	
protected:
	virtual void SetupInputComponent() override;
	virtual void IShowLoading() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WBP_LoadingClass;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputAction> IA_QuitAction;

	void ShowPauseMenu();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WBP_PauseMenuClass;

	UPROPERTY()
	TObjectPtr<class UUserWidget> WBP_PauseMenu;
};
