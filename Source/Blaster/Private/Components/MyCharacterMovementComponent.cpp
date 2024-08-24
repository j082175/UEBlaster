// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MyCharacterMovementComponent.h"

UMyCharacterMovementComponent::UMyCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.01f;
	//bUseAccelerationForPaths = true; // 이거 true 로 설정하니까 AI Move to 시 Jitter 발생
}
