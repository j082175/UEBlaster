// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "GameData/WeaponStat.h"
#include "GameData/WeaponData.h"

#include "GameData/SkillStat.h"
#include "GameData/SkillData.h"

// Enum
#include "Item/Pickable/Weapon/WeaponTypes.h"
#include "BlasterTypes/SkillAssistant.h"

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

	FWeaponStat GetWeaponName(EWeaponName InWeaponType) const;
	FWeaponData GetWeaponData(EWeaponName InWeaponType) const;

	FSkillStat GetSkillStat(ESkillAssistant InWeaponType) const;
	FSkillData GetSkillData(ESkillAssistant InWeaponType) const;


	UPROPERTY()
	int32 CharacterMaxLevel;

private:

	TMap<FName, FWeaponStat> WeaponStatMap;
	TMap<FName, FWeaponData> WeaponDataMap;

	TMap<FName, FSkillStat> SkillStatMap;
	TMap<FName, FSkillData> SkillDataMap;
};
