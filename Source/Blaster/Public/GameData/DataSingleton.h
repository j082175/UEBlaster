// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameData/WeaponData.h"

// Enum
#include "Item/Pickable/Weapon/WeaponTypes.h"

#include "DataSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogABGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class BLASTER_API UDataSingleton : public UObject
{
	GENERATED_BODY()
public:
	UDataSingleton();
	static UDataSingleton& Get();

	// Character Stat Data Section
public:
	FWeaponData GetWeaponName(EWeaponName InWeaponType) const;

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TMap<FName, FWeaponData> WeaponDataMap;

};
