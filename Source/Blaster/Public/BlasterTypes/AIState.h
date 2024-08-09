#pragma once

UENUM(BlueprintType)
enum class EAIState : uint8
{
	EAI_Passive = 0 UMETA(DisplayName = "Passive"),
	EAI_Attacking UMETA(DisplayName = "Attacking"),
	EAI_Frozen UMETA(DisplayName = "Frozen"),
	EAI_Investigating UMETA(DisplayName = "Investigating"),
	EAI_Dead UMETA(DisplayName = "Dead"),

	EAI_MAX UMETA(DisplayName = "Default_MAX")
};