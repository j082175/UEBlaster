// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "MyWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UMyWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UMyWidgetComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void InitWidget() override;
};
