// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/GolemAnimInstance.h"
#include "Characters/Enemy/Golem.h"

#include "GameFramework/CharacterMovementComponent.h"

void UGolemAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<AGolem>(TryGetPawnOwner());
}

void UGolemAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Owner == nullptr)
	{
		Owner = Cast<AGolem>(TryGetPawnOwner());
	}
	else
	{

		Speed = Owner->GetVelocity().Size2D();
		bIsInAir = Owner->GetCharacterMovement()->IsFalling();
		bIsJumping = bIsInAir == true && Owner->GetVelocity().Z >= 0.f;
		bIsAccelerating = Owner->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
		bIsElimmed = Owner->IsElimmed();

	}
}
