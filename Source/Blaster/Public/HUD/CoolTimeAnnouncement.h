// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/MyUserWidget.h"
#include "BlasterTypes/SkillAssistant.h"
#include "CoolTimeAnnouncement.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UCoolTimeAnnouncement : public UMyUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetCoolTimeAnnouncementText(ESkillAssistant InSkillAssistant);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CoolTimeAnnouncementText;


private:
	uint8 bIsActive : 1;
	float DisableTimeCount = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float DisableTimeThreshold = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> PressedSound;
};
