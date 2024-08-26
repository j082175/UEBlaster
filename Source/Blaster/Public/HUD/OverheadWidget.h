// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DisplayText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DisplayText2;

	void SetDisplayText(FString TextToDisplay);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* InPawn);

	void ShowPlayerName(APlayerState* InPlayerState);

protected:
	virtual void NativeDestruct() override;
};
