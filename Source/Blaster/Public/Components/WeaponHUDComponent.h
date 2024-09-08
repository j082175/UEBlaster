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
	virtual void PostLoad() override;
	virtual void InitWidget() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWeaponStatus> WeaponStatus;
};
