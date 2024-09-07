// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/MyWidgetComponent.h"
#include "WeaponHUDComponent.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UWeaponHUDComponent : public UMyWidgetComponent
{
	GENERATED_BODY()
public:
	UWeaponHUDComponent();



private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWeaponStatus> WeaponStatus;
};
