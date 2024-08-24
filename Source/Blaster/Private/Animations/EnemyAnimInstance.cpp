// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/CharacterBase.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//if (CharacterOwner) UE_LOG(LogTemp, Warning, TEXT("BaseAnimInstance tick, owner : %s"), *CharacterOwner->GetName());


	//UE_LOG(LogTemp, Display, TEXT("bIsAccelerating : %d"), bIsAccelerating);
}