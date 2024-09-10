#pragma once

UENUM(BlueprintType)
enum class ELandState : uint8
{
	Normal = 0 UMETA(DisplayName = "Normal"),
	Soft UMETA(DisplayName = "Soft"),
	Heavy UMETA(DisplayName = "Heavy"),
	ELS_MAX UMETA(DisplayName = "Default_MAX")
};