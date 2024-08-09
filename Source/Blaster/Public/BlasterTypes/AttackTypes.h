#pragma once

UENUM(BlueprintType)
enum class EBossAttackType : uint8
{
	EBAT_Melee = 0 UMETA(DisplayName = "Melee"),
	EBAT_Fireball UMETA(DisplayName = "Fireball"),
	EBAT_Spawn UMETA(DisplayName = "Spawn"),
	EBAT_Ground UMETA(DisplayName = "Ground"),
	EBAT_Rush UMETA(DisplayName = "Rush"),
	EBAT_Magic UMETA(DisplayName = "Magic"),
	EBAT_Teleport UMETA(DisplayName = "Teleport"),
	 
	EBAT_MAX UMETA(DisplayName = "Default_MAX")
};

//UENUM(BlueprintType)
//enum class EAttackType : uint8
//{
//	EAT_Melee = 0 UMETA(DisplayName = "Melee"),
//	EAT_Magic UMETA(DisplayName = "Magic"),
//	EAT_Guard UMETA(DisplayName = "Guard"),
//
//	EAT_MAX UMETA(DisplayName = "Default_MAX")
//};
//
//UENUM(BlueprintType)
//enum class EMeleeType : uint8
//{
//	EMT_NoWeapon = 0 UMETA(DisplayName = "NoWeapon"),
//	EMT_Sword UMETA(DisplayName = "Sword"),
//
//	EMT_MAX UMETA(DisplayName = "Default_MAX")
//};
//
//UENUM(BlueprintType)
//enum class EMagicType : uint8
//{
//	EMT_Fireball = 0 UMETA(DisplayName = "Fireball"),
//	EMT_Lightening UMETA(DisplayName = "Lightening"),
//	EMT_Blizzard UMETA(DisplayName = "Blizzard"),
//	EMT_Meteor UMETA(DisplayName = "Meteor"),
//	EMT_Teleport UMETA(DisplayName = "Teleport"),
//
//
//	EMT_MAX UMETA(DisplayName = "Default_MAX")
//};
//
//UENUM(BlueprintType)
//enum class EGuardType : uint8
//{
//	EGT_ArmGuard = 0 UMETA(DisplayName = "ArmGuard"),
//	EGT_Evasion UMETA(DisplayName = "Evasion"),
//
//	EGT_MAX UMETA(DisplayName = "Default_MAX")
//};