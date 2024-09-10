#pragma once

#define FORWARD TEXT("Forward")
#define BACKWARD TEXT("Backward")
#define LEFT TEXT("Left")
#define RIGHT TEXT("Right")

UENUM(BlueprintType)
enum class EKeyType : uint8
{
	Fwd = 0 UMETA(DisplayName = "Fwd"),
	Bwd UMETA(DisplayName = "Bwd"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
	EKT_MAX UMETA(DisplayName = "Default")
};
