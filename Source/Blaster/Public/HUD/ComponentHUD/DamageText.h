// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageText.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UDamageText : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

public:
	void SetDamageText(int32 InDamage, const FLinearColor& InColor);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DamageText;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 1.f;

	FTimerHandle DestroyTimer;
};
