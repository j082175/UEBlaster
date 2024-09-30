#pragma once

#define TRACE_LENGTH 80000.f
#define CUSTOM_DEPTH_PURPLE 250
#define CUSTOM_DEPTH_BLUE 251
#define CUSTOM_DEPTH_TAN 252

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AssaultRifle = 0 UMETA(DisplayName = "AssaultRifle"),
	RocketLauncher UMETA(DisplayName = "RocketLauncher"),
	Pistol UMETA(DisplayName = "Pistol"),
	SMG UMETA(DisplayName = "SMG"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	SniperRifle UMETA(DisplayName = "SniperRifle"),
	GrenadeLauncher UMETA(DisplayName = "GrenadeLauncher"),
	Flag UMETA(DisplayName = "Flag"),
	OneHanded UMETA(DisplayName = "OneHanded"),
	TwoHanded UMETA(DisplayName = "TwoHanded"),
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
	CTAR_21 UMETA(DisplayName = "CTAR_21"),

	MAX UMETA(DisplayName = "MAX")

};