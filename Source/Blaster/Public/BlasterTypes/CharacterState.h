#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Unequipped = 0 UMETA(DisplayName = "Unequipped"),
	EquippedOnehandedWeapon UMETA(DisplayName = "EquippedOnehandedWeapon"),
	EquippedTwoHandedWeapon UMETA(DisplayName = "EquippedTwoHandedWeapon"),
	EquippedGun UMETA(DisplayName = "EquippedGun"),
	EquippedCapsuleWeapon UMETA(DisplayName = "EquippedCapsuleWeapon"),
	ECS_MAX UMETA(DisplayName = "MAX")
};