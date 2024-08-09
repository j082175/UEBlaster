// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "MyAIPerceptionStimuliSource.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UMyAIPerceptionStimuliSource : public UAIPerceptionStimuliSourceComponent
{
	GENERATED_BODY()
public:
	UMyAIPerceptionStimuliSource(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
