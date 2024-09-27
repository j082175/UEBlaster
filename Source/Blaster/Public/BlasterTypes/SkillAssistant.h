#pragma once

UENUM(BlueprintType)
enum class ESkillAnimType : uint8
{
	CoolTime = 0 UMETA(DisplayName = "CoolTime"),
	Maintain UMETA(DisplayName = "Maintain"),

	ESA_MAX UMETA(DisplayName = "Default_MAX")
};


UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Slide = 0 UMETA(DisplayName = "Slide"),
	Dash UMETA(DisplayName = "Dash"),
	Dodge UMETA(DisplayName = "Dodge"),

	Spawn UMETA(DisplayName = "Spawn"),
	Active UMETA(DisplayName = "Active"),
	Passive UMETA(DisplayName = "Passive"),
	Ultimate UMETA(DisplayName = "Ultimate"),

	ESA_MAX UMETA(DisplayName = "Default_MAX")
};

