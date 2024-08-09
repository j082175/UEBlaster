// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/Golem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

AGolem::AGolem()
{
}

void AGolem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGolem::IAttack(FAttackEndedDelegate Delegate, const FString& InAttackType)
{
	if (!IsValid(AttackMontage)) return;

	OnAttackEnded = Delegate;

	FString AttackTypeStr = UEnum::GetDisplayValueAsText(AttackType).ToString();

	PlayMontage(GetMesh()->GetAnimInstance(), AttackMontage, FName(AttackTypeStr), -1);

}
