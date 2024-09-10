#pragma once

UENUM(BlueprintType)
enum class EMovementInput : uint8
{
	Left = 0 UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
	Fwd UMETA(DisplayName = "Fwd"),
	Bwd UMETA(DisplayName = "Bwd"),

	EMI_MAX UMETA(DisplayName = "DefaultMAX")
};