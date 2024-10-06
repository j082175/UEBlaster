#pragma once

UENUM(BlueprintType)
enum class EAnimState : uint8
{
	Combat = 0 UMETA(DisplayName = "Combat"),
	UnEquipped UMETA(DisplayName = "UnEquipped"),
	Equipped UMETA(DisplayName = "Equipped"),

	Shield UMETA(DisplayName = "Shield"),
	SecondaryWeapon UMETA(DisplayName = "SecondaryWeapon"),

	MAX UMETA(DisplayName = "Default_MAX")
};

//UENUM(BlueprintType)
//enum class ECharacterState_Weapon : uint8
//{
//	UnEquipped = 0 UMETA(DisplayName = "UnEquipped"),
//	EAS_Pistol UMETA(DisplayName = "Pistol"),
//	EAS_Rifle UMETA(DisplayName = "Rifle"),
//	MAX UMETA(DisplayName = "Default_MAX")
//};

//UENUM(BlueprintType)
//enum class ECombatState : uint8
//{
//	Unequipped = 0 UMETA(DisplayName = "Unequipped"),
//	EquippedOnehandedWeapon UMETA(DisplayName = "EquippedOnehandedWeapon"),
//	EquippedTwoHandedWeapon UMETA(DisplayName = "EquippedTwoHandedWeapon"),
//	EquippedGun UMETA(DisplayName = "EquippedGun"),
//	EquippedCapsuleWeapon UMETA(DisplayName = "EquippedCapsuleWeapon"),
//	ECS_MAX UMETA(DisplayName = "MAX")
//};
