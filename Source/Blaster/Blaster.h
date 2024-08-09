// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define ECC_SkeletalMesh ECollisionChannel::ECC_GameTraceChannel1
#define ECC_HitCapsule ECollisionChannel::ECC_GameTraceChannel2
#define ECC_CanDamagedByWeapon ECollisionChannel::ECC_GameTraceChannel3
#define ECC_CanMorV ECollisionChannel::ECC_GameTraceChannel4
#define ECC_IsPickable ECollisionChannel::ECC_GameTraceChannel5


#define PRINT_DEBUG_LOCALROLE(Actor) UE_LOG(LogTemp, Display, TEXT("Actor's LocalRole : %s"), *UEnum::GetDisplayValueAsText(Actor->GetLocalRole()).ToString())