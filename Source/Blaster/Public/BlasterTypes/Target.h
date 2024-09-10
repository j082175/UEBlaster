#pragma once
#include "BlasterTypes/BlackboardKeys.h"

UENUM(BlueprintType)
enum class ETarget : uint8
{
	TargetActor = 0 UMETA(DisplayName = TARGET_ACTOR),
	OwingActor UMETA(DisplayName = OWING_ACTOR),

	ET_MAX UMETA(DisplayName = "DefaultMAX")
};