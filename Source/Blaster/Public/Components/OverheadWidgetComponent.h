// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Components/WidgetComponent.h"
#include "HUD/MyWidgetComponent.h"
#include "OverheadWidgetComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FOnLocalRoleTextChangedDelegate, const FString&, const FLinearColor&);
DECLARE_DELEGATE_TwoParams(FOnPlayerIDTextChangedDelegate, const FString&, const FLinearColor&);

DECLARE_DELEGATE_OneParam(FOnLocalRoleTextVisibilityChangedDelegate, ESlateVisibility);
DECLARE_DELEGATE_OneParam(FOnPlayerIDTextVisibilityChangedDelegate, ESlateVisibility);

/**
 * 
 */
UCLASS()
class BLASTER_API UOverheadWidgetComponent : public UMyWidgetComponent
{
	GENERATED_BODY()
	
public:
	FOnLocalRoleTextChangedDelegate OnLocalRoleTextChanged;
	FOnPlayerIDTextChangedDelegate OnPlayerIDTextChanged;

	FOnLocalRoleTextVisibilityChangedDelegate OnLocalRoleTextVisibilityChanged;
	FOnPlayerIDTextVisibilityChangedDelegate OnPlayerIDTextVisibilityChanged;
};
