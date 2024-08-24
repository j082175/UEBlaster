// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MyAIPerceptionStimuliSource.h"

UMyAIPerceptionStimuliSource::UMyAIPerceptionStimuliSource(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;

	bAutoRegisterAsSource = true;
}