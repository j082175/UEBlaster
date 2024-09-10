#pragma once

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Default = 0 UMETA(DisplayName = "Default"),
	Fireball UMETA(DisplayName = "Fireball"),
	Spawn UMETA(DisplayName = "Spawn"),
	Ground UMETA(DisplayName = "Ground"),
	Rush UMETA(DisplayName = "Rush"),
	Magic UMETA(DisplayName = "Magic"),
	Teleport UMETA(DisplayName = "Teleport"),
	 
	EAT_MAX UMETA(DisplayName = "Default_MAX")
};

//UENUM(BlueprintType)
//enum class EAttackType : uint8
//{
//	Default = 0 UMETA(DisplayName = "Melee"),
//	Magic UMETA(DisplayName = "Magic"),
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