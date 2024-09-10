#pragma once

UENUM(BlueprintType)
enum class ETeam : uint8
{
	NoTeam UMETA(DisplayName = "NoTeam"),
	RedTeam UMETA(DisplayName = "RedTeam"),
	BlueTeam UMETA(DisplayName = "BlueTeam"),
	ET_MAX UMETA(DisplayName = "DefaultMAX")
};