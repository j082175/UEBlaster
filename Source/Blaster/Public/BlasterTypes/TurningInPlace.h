#pragma once

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_NotTurning = 0 UMETA(DisplayName = "Not Turning"),
	ETIP_TurningLeft UMETA(DisplayName = "Turning Left"),
	ETIP_TurningRight UMETA(DisplayName = "Turning Right"),
	ETIP_MAX UMETA(DisplayName = "DefaultMAX")
};