#pragma once

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Unoccupied = 0 UMETA(DisplayName = "Unoccupied"),
	HitReact UMETA(DisplayName = "HitReact"),
	Attacking UMETA(DisplayName = "Attacking"),
	Reloading UMETA(DisplayName = "Reloading"),
	ThrowingGrenade UMETA(DisplayName = "ThrowingGrenade"),
	SwappingWeapon UMETA(DisplayName = "SwappingWeapon"),
	Dodging UMETA(DisplayName = "Dodging"),
	VaultOrMantle UMETA(DisplayName = "VaultOrMantle"),
	Sliding UMETA(DisplayName = "Sliding"),
	MeleeAttack UMETA(DisplayName = "MeleeAttack"),
	Parried UMETA(DisplayName = "Parried"),
	Stun UMETA(DisplayName = "Stun"),
	Dash UMETA(DisplayName = "Dash"),
	Ragdoll UMETA(DisplayName = "Ragdoll"),
	PhysicsRecover UMETA(DisplayName = "PhysicsRecover"),
	RagdollForMulti UMETA(DisplayName = "RagdollForMulti"),
	PhysicsRecoverForMulti UMETA(DisplayName = "PhysicsRecoverForMulti"),
	SkillCasting UMETA(DisplayName = "SkillCasting"),
	UltimateMode UMETA(DisplayName = "UltimateMode"),
	Dead UMETA(DisplayName = "Dead"),

	ECS_MAX UMETA(DisplayName = "MAX")
};