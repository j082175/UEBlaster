#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped = 0 UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOnehandedWeapon UMETA(DisplayName = "EquippedOnehandedWeapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "EquippedTwoHandedWeapon"),
	ECS_EquippedGun UMETA(DisplayName = "EquippedGun"),
	ECS_EquippedCapsuleWeapon UMETA(DisplayName = "EquippedCapsuleWeapon"),
	ECS_MAX UMETA(DisplayName = "MAX")
};