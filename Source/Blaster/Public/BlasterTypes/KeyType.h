#pragma once

#define FORWARD TEXT("Forward")
#define BACKWARD TEXT("Backward")
#define LEFT TEXT("Left")
#define RIGHT TEXT("Right")

UENUM(BlueprintType)
enum class EKeyType : uint8
{
	EKT_Fwd = 0 UMETA(DisplayName = "Fwd"),
	EKT_Bwd UMETA(DisplayName = "Bwd"),
	EKT_Left UMETA(DisplayName = "Left"),
	EKT_Right UMETA(DisplayName = "Right"),
	EKT_MAX UMETA(DisplayName = "Default")
};
