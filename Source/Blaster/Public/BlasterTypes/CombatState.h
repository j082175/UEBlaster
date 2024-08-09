#pragma once

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied = 0 UMETA(DisplayName = "Unoccupied"),
	ECS_HitReact UMETA(DisplayName = "HitReact"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_ThrowingGrenade UMETA(DisplayName = "ThrowingGrenade"),
	ECS_SwappingWeapon UMETA(DisplayName = "SwappingWeapon"),
	ECS_Dodging UMETA(DisplayName = "Dodging"),
	ECS_VaultOrMantle UMETA(DisplayName = "VaultOrMantle"),
	ECS_Sliding UMETA(DisplayName = "Sliding"),
	ECS_Attacking UMETA(DisplayName = "Attacking"),
	ECS_MeleeAttack UMETA(DisplayName = "MeleeAttack"),
	ECS_Parried UMETA(DisplayName = "Parried"),
	ECS_Stun UMETA(DisplayName = "Stun"),
	ECS_Dash UMETA(DisplayName = "Dash"),
	ECS_Ragdoll UMETA(DisplayName = "Ragdoll"),
	ECS_PhysicsRecover UMETA(DisplayName = "PhysicsRecover"),
	ECS_MAX UMETA(DisplayName = "MAX")
};