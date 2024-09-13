// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/MyWidgetComponent.h"
#include "ReceiveDamageHUDComponent.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UReceiveDamageHUDComponent : public UMyWidgetComponent
{
	GENERATED_BODY()
	UReceiveDamageHUDComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetDamageInfo(int32 InDamage, class AController* InController, const FLinearColor& InColor);
private:

	TWeakObjectPtr<class UDamageScroll> DamageScroll;

};
