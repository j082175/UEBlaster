// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BlasterTypes/Team.h"
#include "OverheadWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void Init();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> LocaleRoleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PlayerIDText;

	void SetDisplayText(FString TextToDisplay);
	void SetTextColor(FLinearColor InColor);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* InPawn);

	void ShowPlayerName(APlayerState* InPlayerState);
	void ShowPlayerName(const FString& InName);

	UFUNCTION(Server, Reliable)
	void ServerShowPlayerName(APlayerState* InPlayerState);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastShowPlayerName(APlayerState* InPlayerState);

protected:
	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class ACharacter> ActorToFind;

	FTimerHandle InitTimerHandle;
};
