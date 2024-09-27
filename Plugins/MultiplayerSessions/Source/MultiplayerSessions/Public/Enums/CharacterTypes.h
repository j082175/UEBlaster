#pragma once

UENUM(BlueprintType)
enum class ECharacterTypes : uint8
{
	Wraith = 0 UMETA(DisplayName = "Wraith"),
	Belica UMETA(DisplayName = "Belica"),
	Murdock UMETA(DisplayName = "Murdock"),

	MAX UMETA(DisplayName = "DefaultMAX")
};

