// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CanParryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanParryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLASTER_API ICanParryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool IIsParring() = 0;
	virtual bool ICheckParry(AActor* OtherActor) = 0;
};
