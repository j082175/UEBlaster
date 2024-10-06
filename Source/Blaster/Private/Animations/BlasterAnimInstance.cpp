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

UBlasterAnimInstance::UBlasterAnimInstance()
{
	AB_CALLLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
}

void UBlasterAnimInstance::NativeInitializeAnimation()
{
	AB_CALLLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::NativeInitializeAnimation();
	AB_CALLLOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));

	BlasterCharacterOwner = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!BlasterCharacterOwner.IsValid())
	{
		BlasterCharacterOwner = Cast<ABlasterCharacter>(TryGetPawnOwner());
	}
	else
	{
	}
}
