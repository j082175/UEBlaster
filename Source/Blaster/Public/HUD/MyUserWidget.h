// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 *
 */
UCLASS(meta = (DisableNativeTick))
class BLASTER_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FORCEINLINE void SetOwingActor(AActor* NewOwner) { OwingActor = NewOwner; }


protected:
	TWeakObjectPtr<AActor> OwingActor;

	void PollInit();

	TWeakObjectPtr<AActor> OPawn;
private:
};
