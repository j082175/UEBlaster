// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MyCharacterMovementComponent.h"
#include "Blaster.h"

UMyCharacterMovementComponent::UMyCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.01f;
	//bUseAccelerationForPaths = true; // 이거 true 로 설정하니까 AI Move to 시 Jitter 발생
}

void UMyCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//AB_SUBLOG(LogABDisplay, Warning, TEXT("TickType : %s"), *UEnum::GetDisplayValueAsText(TickType).ToString());
	//AB_SUBLOG(LogABDisplay, Warning, TEXT(""));
}

void UMyCharacterMovementComponent::PostLoad()
{
	Super::PostLoad();

	SetWalkableFloorAngle(70.f);
	MaxStepHeight = 45.f;
}
