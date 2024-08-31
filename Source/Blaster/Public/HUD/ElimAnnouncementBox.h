// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ElimAnnouncementBox.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UElimAnnouncementBox : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetElimAnnouncementText(FString AttackerName, FString VictimName);
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> AnnouncementBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> AttackerText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> VictimText;
};
