#pragma once

UENUM(BlueprintType)
enum class EMapNames : uint8
{
	GameMap_Space = 0 UMETA(DisplayName = "GameMap_Space"),
	GameMap_Tokyo UMETA(DisplayName = "GameMap_Tokyo"),
	GameMap_Neoncity UMETA(DisplayName = "GameMap_Neoncity"),
	GameMap_Lobby UMETA(DisplayName = "GameMap_Lobby"),

	MAX UMETA(DisplayName = "DefaultMAX"),

	StartupMap UMETA(DisplayName = "StartupMap"),
	StartupMap_SciFi_Dynamic UMETA(DisplayName = "StartupMap_SciFi_Dynamic")

};