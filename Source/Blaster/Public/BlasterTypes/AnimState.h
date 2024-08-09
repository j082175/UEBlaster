#pragma once

UENUM(BlueprintType)
enum class EAnimState : uint8
{
	EAS_UnEquipped = 0 UMETA(DisplayName = "UnEquipped"),
	EAS_Equipped UMETA(DisplayName = "Equipped"),
	EAS_Combat UMETA(DisplayName = "Combat"),
	EAS_MAX UMETA(DisplayName = "Default_MAX")
};

//UENUM(BlueprintType)
//enum class ECharacterState_Weapon : uint8
//{
//	EAS_UnEquipped = 0 UMETA(DisplayName = "UnEquipped"),
//	EAS_Pistol UMETA(DisplayName = "Pistol"),
//	EAS_Rifle UMETA(DisplayName = "Rifle"),
//	EAS_MAX UMETA(DisplayName = "Default_MAX")
//};

//UENUM(BlueprintType)
//enum class ECombatState : uint8
//{
//	ECS_Unequipped = 0 UMETA(DisplayName = "Unequipped"),
//	ECS_EquippedOnehandedWeapon UMETA(DisplayName = "EquippedOnehandedWeapon"),
//	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "EquippedTwoHandedWeapon"),
//	ECS_EquippedGun UMETA(DisplayName = "EquippedGun"),
//	ECS_EquippedCapsuleWeapon UMETA(DisplayName = "EquippedCapsuleWeapon"),
//	ECS_MAX UMETA(DisplayName = "MAX")
//};
