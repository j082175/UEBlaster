// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/MyUserWidget.h"
#include "DamageScroll.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UDamageScroll : public UMyUserWidget
{
	GENERATED_BODY()
public:
	UDamageScroll(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	void AddDamageText(int32 InDamage, const FLinearColor& InColor);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> DamageScroll;


	TWeakObjectPtr<class UDamageText> DamageText;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDamageText> DamageTextClass;



private:
	void Invisible();

	FTimerHandle InvisibleTimer;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float InvisibleTime = 1.f;
};
