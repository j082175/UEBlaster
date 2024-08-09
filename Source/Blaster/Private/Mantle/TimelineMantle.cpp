// Fill out your copyright notice in the Description page of Project Settings.


#include "Mantle/TimelineMantle.h"
#include "Components/MantleSystem_Demo.h"

// Sets default values
ATimelineMantle::ATimelineMantle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimelineMantle::BeginPlay()
{
	Super::BeginPlay();
	
	MantleSystem = GetComponentByClass<UMantleSystem_Demo>();
}

// Called every frame
void ATimelineMantle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

