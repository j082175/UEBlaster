// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MyCharacterMovementComponent.h"

UMyCharacterMovementComponent::UMyCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.01f;
	//bUseAccelerationForPaths = true; // �̰� true �� �����ϴϱ� AI Move to �� Jitter �߻�
}
