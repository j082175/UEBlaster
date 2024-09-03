// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Components/WidgetComponent.h"
#include "HUD/MyWidgetComponent.h"
#include "OverheadWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UOverheadWidgetComponent : public UMyWidgetComponent
{
	GENERATED_BODY()
	UOverheadWidgetComponent();
	virtual void BeginPlay() override;

public:
	FORCEINLINE class UOverheadWidget* GetOverheadWidget() const { return OverheadWidget; }

private:
	void InitTextColor();

	FTimerHandle InitHandle;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UOverheadWidget> OverheadWidget;
};
