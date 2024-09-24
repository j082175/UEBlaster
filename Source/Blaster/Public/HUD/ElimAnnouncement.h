// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Blueprint/UserWidget.h"
#include "HUD/MyUserWidget.h"
#include "ElimAnnouncement.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UElimAnnouncement : public UMyUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetElimAnnouncementText(FString AttackerName, FString VictimName);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> AnnouncementScroll;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UElimAnnouncementBox> ElimAnnouncementBox;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UElimAnnouncementBox> ElimAnnouncementBoxClass;

	UPROPERTY(EditAnywhere)
	float ElimAnnouncementTime = 3.f;

private:
	void RemoveScrollChild();
};
