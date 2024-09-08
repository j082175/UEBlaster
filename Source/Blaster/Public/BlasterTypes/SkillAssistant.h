#pragma once

UENUM(BlueprintType)
enum class ESkillAssistant : uint8
{
	ESA_Dash = 0 UMETA(DisplayName = "Dash"),
	ESA_Dodge UMETA(DisplayName = "Dodge"),
	ESA_Slide UMETA(DisplayName = "Slide"),

	ESA_HealArea UMETA(DisplayName = "HealArea"),
	ESA_ShieldRecovery UMETA(DisplayName = "ShieldRecovery"),
	ESA_Supporter UMETA(DisplayName = "Supporter"),
	ESA_Ultimate UMETA(DisplayName = "Ultimate"),

	ESA_MAX UMETA(DisplayName = "Default_MAX")
};
