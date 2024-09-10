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
	virtual void PostLoad() override;
	virtual void InitWidget() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE class UOverheadWidget* GetOverheadWidget() const { return OverheadWidget; }

private:
	void InitTextColor();
	void InitTextColorFunc();


	FTimerHandle InitHandle;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UOverheadWidget> OverheadWidget;
};
