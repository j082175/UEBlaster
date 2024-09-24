#pragma once

//UENUM(BlueprintType)
//enum class EFreeForAllMaps : uint8
//{
//	MAX UMETA(DisplayName = "DefaultMAX")
//};

#define DEFAULT_GAMEMAP_PATH TEXT("/Game/A_Blaster/Maps/GameMaps/")

UENUM(BlueprintType)
enum class ETeamMaps : uint8
{
	GameMap_Hanamura = 0 UMETA(DisplayName = "GameMap_Hanamura"),
	GameMap_Neoncity UMETA(DisplayName = "GameMap_Neoncity"),
	//GameMap_Lobby UMETA(DisplayName = "GameMap_Lobby"),
	MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class ECaptureTheFlagMaps : uint8
{
	GameMap_Space = 0 UMETA(DisplayName = "GameMap_Space"),
	MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EDefaultMaps : uint8
{
	StartupMap = 0 UMETA(DisplayName = "StartupMap"),
	StartupMap_SciFi_Dynamic UMETA(DisplayName = "StartupMap_SciFi_Dynamic"),
	LobbyMap_SciFi_Dynamic UMETA(DisplayName = "LobbyMap_SciFi_Dynamic"),
	MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EMatchTypes : uint8
{
	//FreeForAll = 0 UMETA(DisplayName = "FreeForAll"),
	Teams = 0 UMETA(DisplayName = "Teams"),
	CaptureTheFlag UMETA(DisplayName = "CaptureTheFlag"),
	MAX UMETA(DisplayName = "DefaultMAX")
};

