#pragma once

UENUM(BlueprintType)
enum class ELandState : uint8
{
	ELS_Normal = 0 UMETA(DisplayName = "Normal"),
	ELS_Soft UMETA(DisplayName = "Soft"),
	ELS_Heavy UMETA(DisplayName = "Heavy"),
	ELS_MAX UMETA(DisplayName = "Default_MAX")
};