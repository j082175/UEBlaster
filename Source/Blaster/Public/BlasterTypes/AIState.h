#pragma once

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Passive = 0 UMETA(DisplayName = "Passive"),
	Attacking UMETA(DisplayName = "Attacking"),
	Frozen UMETA(DisplayName = "Frozen"),
	Investigating UMETA(DisplayName = "Investigating"),
	Dead UMETA(DisplayName = "Dead"),

	MAX UMETA(DisplayName = "Default_MAX")
};