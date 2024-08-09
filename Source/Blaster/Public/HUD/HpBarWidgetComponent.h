// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HpBarWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UHpBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UHpBarWidgetComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	virtual void InitWidget() override;
	virtual void BeginPlay() override;

	void SetHpBar(float Percentage);
	void SetShieldBar(float Percentage);
	void SetParryBar(float InCurrentP, float InMaxP);

	void ParryGaugeAnimStart(bool InCheck);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UHpBarWidget> HpBarWidget;
};
