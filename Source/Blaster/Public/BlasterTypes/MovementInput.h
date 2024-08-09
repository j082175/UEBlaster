#pragma once

UENUM(BlueprintType)
enum class EMovementInput : uint8
{
	EMI_Left = 0 UMETA(DisplayName = "Left"),
	EMI_Right UMETA(DisplayName = "Right"),
	EMI_Fwd UMETA(DisplayName = "Fwd"),
	EMI_Bwd UMETA(DisplayName = "Bwd"),

	EMI_MAX UMETA(DisplayName = "DefaultMAX")
};