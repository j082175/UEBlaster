// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	FORCEINLINE void SetOwingActor(AActor* NewOwner) { OwingActor = NewOwner; }


protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> OwingActor;
};
