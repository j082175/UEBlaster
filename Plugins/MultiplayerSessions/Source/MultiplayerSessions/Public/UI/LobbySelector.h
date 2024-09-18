// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/MapNames.h"

#include "Components/Button.h"
#include "LobbySelector.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLAYERSESSIONS_API ULobbySelector : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetText(const FString& InStr);
	FORCEINLINE void SetOptionIndex(int32 InCurrentIndex) { CurrentIndex = InCurrentIndex; }


	int32 Switch(bool IsRight, int32 Max);
	

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> LeftArrow;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> RightArrow;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Option_Text;
private:

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	int32 CurrentIndex;
};
