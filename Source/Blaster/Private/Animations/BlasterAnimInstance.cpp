// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/BlasterAnimInstance.h"
#include "Characters/BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Item/Pickable/Weapon/Weapon_Gun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "BlasterTypes/CombatState.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/Blaster.h"

//void UBlasterAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(ThisClass, bUseFABRIK);
//}

void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlasterCharacter = Cast<ABlasterCharacter>(CharacterOwner);
}

void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BlasterCharacter == nullptr)
	{
		BlasterCharacter = Cast<ABlasterCharacter>(CharacterOwner);
	}
	else
	{
		
		//TurningInPlace = BlasterCharacter->GetTurningInPlace();
		//bRotateRootBone = BlasterCharacter->ShouldRotateRootBone();
		//bDisableGameplay = BlasterCharacter->GetDisableGameplay();
		//AO_Yaw = BlasterCharacter->GetAO_Yaw();
		//AO_Pitch = BlasterCharacter->GetAO_Pitch();

		//bUseAimOffsets = CharacterOwner->GetCombatState() != ECombatState::ECS_Reloading && !bDisableGameplay;

	}
}
