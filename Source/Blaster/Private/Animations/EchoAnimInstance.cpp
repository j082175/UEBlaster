// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/EchoAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "Characters/Echo.h"

void UEchoAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ACharacter* Character = Cast<ACharacter>(GetOwningActor());
	if (Character)
	{
		CharacterMovementComponent = Character->GetCharacterMovement();
	}
}

void UEchoAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CharacterMovementComponent)
	{
		float Velocity2D = CharacterMovementComponent->Velocity.Size2D();
		float VelocityZ = CharacterMovementComponent->Velocity.Z;

		GroundSpeed = Velocity2D;

		bIsFalling = CharacterMovementComponent->IsFalling();

		//UE_LOG(LogTemp, Display, TEXT("jump : %f"), VelocityZ);
		//UE_LOG(LogTemp, Display, TEXT("jump : %d"), bIsJumping);


		bIsIdle = Velocity2D < MovingThreshold ? true : false;
		bIsJumping = (VelocityZ > JumpingThreshold && bIsFalling) ? true : false;


		//CharacterState = Cast<AEcho>(GetOwningActor())->GetCharacterState();
	}

}
