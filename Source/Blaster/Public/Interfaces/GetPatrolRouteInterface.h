// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GetPatrolRouteInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGetPatrolRouteInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLASTER_API IGetPatrolRouteInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual class ASplineActor* IGetPatrolRoute() = 0;
	virtual void ISetPatrolRoute(class ASplineActor* InSplineActor) = 0;

};
