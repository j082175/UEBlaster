#pragma once

UENUM(BlueprintType)
enum class ESkillAnimType : uint8
{
	CoolTime = 0 UMETA(DisplayName = "CoolTime"),
	Maintain UMETA(DisplayName = "Maintain"),

	ESA_MAX UMETA(DisplayName = "Default_MAX")
};


UENUM(BlueprintType)
enum class ESkillAssistant : uint8
{
	Slide = 0 UMETA(DisplayName = "Slide"),
	Dash UMETA(DisplayName = "Dash"),
	Dodge UMETA(DisplayName = "Dodge"),

	HealArea UMETA(DisplayName = "HealArea"),
	ShieldRecovery UMETA(DisplayName = "ShieldRecovery"),
	Supporter UMETA(DisplayName = "Supporter"),
	Ultimate UMETA(DisplayName = "Ultimate"),

	ESA_MAX UMETA(DisplayName = "Default_MAX")
};
