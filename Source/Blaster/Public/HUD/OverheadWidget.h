// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD/MyUserWidget.h"
#include "BlasterTypes/Team.h"
#include "OverheadWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UOverheadWidget : public UMyUserWidget
{
	GENERATED_BODY()
public:
	UOverheadWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetAllTextColor(FLinearColor InColor);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* InPawn);

	void ShowPlayerName(APawn* InPawn);
	void ShowPlayerName(const FString& InName);

protected:
	void SetDisplayText(FString TextToDisplay);

	virtual void NativeDestruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> LocaleRoleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PlayerIDText;


private:
	void ShowPlayerNameInitFunc();

	TWeakObjectPtr<APawn> PawnToInit;

	UPROPERTY()
	FTimerHandle InitHandle;
};
