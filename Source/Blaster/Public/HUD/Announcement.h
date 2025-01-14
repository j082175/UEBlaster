// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD/MyUserWidget.h"
#include "Announcement.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UAnnouncement : public UMyUserWidget
{
	GENERATED_BODY()
public:

	void SetHUDAnnouncementCountdown(float CountdownTime);

	void SetAnnouncementText(const FString& InString);
	void SetInfoText(const FString& InString);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> WarmupTime;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> AnnouncementText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> InfoText;

};
