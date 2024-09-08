#pragma once

#define TRACE_LENGTH 80000.f
#define CUSTOM_DEPTH_PURPLE 250
#define CUSTOM_DEPTH_BLUE 251
#define CUSTOM_DEPTH_TAN 252

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssaultRifle = 0 UMETA(DisplayName = "AssaultRifle"),
	EWT_RocketLauncher UMETA(DisplayName = "RocketLauncher"),
	EWT_Pistol UMETA(DisplayName = "Pistol"),
	EWT_SMG UMETA(DisplayName = "SMG"),
	EWT_Shotgun UMETA(DisplayName = "Shotgun"),
	EWT_SniperRifle UMETA(DisplayName = "SniperRifle"),
	EWT_GrenadeLauncher UMETA(DisplayName = "GrenadeLauncher"),
	EWT_Flag UMETA(DisplayName = "Flag"),
	EWT_OneHanded UMETA(DisplayName = "OneHanded"),
	EWT_TwoHanded UMETA(DisplayName = "TwoHanded"),
	EWT_MAX UMETA(DisplayName = "MAX")

};

UENUM(BlueprintType)
enum class EWeaponName : uint8
{
	M416 = 0 UMETA(DisplayName = "M416"),
	RocketLauncher UMETA(DisplayName = "RocketLauncher"),
	M1911 UMETA(DisplayName = "M1911"),
	MAC10 UMETA(DisplayName = "MAC10"),
	Sawed_Off UMETA(DisplayName = "Sawed_Off"),
	PSG1 UMETA(DisplayName = "PSG1"),
	M32_Grenade_Launcher UMETA(DisplayName = "M32_Grenade_Launcher"),
	M4A1 UMETA(DisplayName = "M4A1"),
	AS_VAL UMETA(DisplayName = "AS_VAL"),
	AKM UMETA(DisplayName = "AKM"),
	AKS_74U UMETA(DisplayName = "AKS_74U"),

	AI_Pistol UMETA(DisplayName = "AI_Pistol"),

	RailGun UMETA(DisplayName = "RailGun"),
	MAX UMETA(DisplayName = "MAX")

};