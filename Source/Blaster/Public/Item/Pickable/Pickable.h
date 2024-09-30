// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Engine/EngineTypes.h"

#include "Pickable.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API APickable : public AItem
{
	GENERATED_BODY()
public:
	UFUNCTION(NetMulticast, Reliable)
	void AttachToComponentMulticast(USceneComponent* Parent, FName SocketName);

};
