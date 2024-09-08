// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define ECC_SkeletalMesh ECollisionChannel::ECC_GameTraceChannel1
#define ECC_HitCapsule ECollisionChannel::ECC_GameTraceChannel2
#define ECC_CanDamagedByWeapon ECollisionChannel::ECC_GameTraceChannel3
#define ECC_CanMorV ECollisionChannel::ECC_GameTraceChannel4
#define ECC_IsPickable ECollisionChannel::ECC_GameTraceChannel5
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel6

#define PROFILE_Pawn TEXT("Pawn")
#define PROFILE_CharacterMesh TEXT("CharacterMesh")
#define PROFILE_BlockAllDynamic TEXT("BlockAllDynamic")
#define PROFILE_Projectile TEXT("Projectile")
#define PROFILE_Destructible TEXT("Destructible")
#define PROFILE_BlockAllDynamic TEXT("BlockAllDynamic")
#define PROFILE_IgnoreOnlyPawn TEXT("IgnoreOnlyPawn")


#define PRINT_DEBUG_LOCALROLE(Actor) UE_LOG(LogTemp, Display, TEXT("Actor's LocalRole : %s"), *UEnum::GetDisplayValueAsText(Actor->GetLocalRole()).ToString())

#define LOG_LOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()))
#define LOG_REMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))
#define LOG_SUBLOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetLocalRole()))
#define LOG_SUBREMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetRemoteRole()))
#define LOG_NETMODEINFO ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), static_cast<int32>(GPlayInEditorID)) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER"))) 
#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)

#define AB_CALLLOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("%s %s [%s]"), LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__), *GetName())
#define AB_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][Local:%s/Remote:%s] %s %s [%s]"), LOG_NETMODEINFO, LOG_LOCALROLEINFO, LOG_REMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__), *GetName())

#define AB_SUBLOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][Local:%s/Remote:%s] %s %s [%s]<%s>"), LOG_NETMODEINFO, LOG_SUBLOCALROLEINFO, LOG_SUBREMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__), *GetName(), *GetOwner()->GetName())

DECLARE_LOG_CATEGORY_EXTERN(LogABDisplay, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogABBeginPlay, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogABNetwork, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogABTeleport, Log, All);

static class WidgetAnimHelper
{
public:
	static void FindWidgetAnimationName(class UUserWidget* InUserWidget, TMap<FString, class UWidgetAnimation*>& InWidgetAnimations);

	UFUNCTION(BlueprintCallable)
	static class UWidgetAnimation* StartAnimation(const FString& InPrefix, const FString& InName, int32 InIndex, float InPlaybackSpeed, class UUserWidget* InUserWidget, TMap<FString, class UWidgetAnimation*>& InWidgetAnimations);
};

