#pragma once

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	NotTurning = 0 UMETA(DisplayName = "Not Turning"),
	TurningLeft UMETA(DisplayName = "Turning Left"),
	TurningRight UMETA(DisplayName = "Turning Right"),
	ETIP_MAX UMETA(DisplayName = "DefaultMAX")
};