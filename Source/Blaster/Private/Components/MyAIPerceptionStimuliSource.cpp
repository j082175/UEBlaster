// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MyAIPerceptionStimuliSource.h"
#include "Blaster/Blaster.h"

UMyAIPerceptionStimuliSource::UMyAIPerceptionStimuliSource(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickInterval = 0.1f;

	bAutoRegisterAsSource = true;
}

void UMyAIPerceptionStimuliSource::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AB_SUBLOG(LogABDisplay, Warning, TEXT(""));
}
