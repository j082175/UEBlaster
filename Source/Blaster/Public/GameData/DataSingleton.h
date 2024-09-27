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
#include "Enums/CharacterTypes.h"

#include "DataSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogABGameSingleton, Error, All);

UENUM(BlueprintType)
enum class EDataType : uint8
{
	WeaponStat = 0 UMETA(DisplayName = "WeaponStat"),
	WeaponData UMETA(DisplayName = "WeaponData"),
	SkillStat UMETA(DisplayName = "SkillStat"),
	SkillData UMETA(DisplayName = "SkillData"),

	MAX UMETA(DisplayName = "Default_MAX")
};

/**
 *
 */
 //template<typename DataStruct, typename EnumType>
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


	FSkillStat GetSkillStat(ECharacterTypes InCharacterType, const FName& Index) const;
	FSkillData GetSkillData(ECharacterTypes InCharacterType, const FName& Index) const;


	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	template<typename DataStruct>
	void Init(TMap<FName, DataStruct>& OutMap, const FString& InName);


	TMap<FName, FWeaponStat> WeaponStatMap;
	TMap<FName, FWeaponData> WeaponDataMap;

	TMap<FName, FSkillStat> SkillStatMap;
	TMap<FName, FSkillData> SkillDataMap;

	TArray<TMap<FName, FSkillStat>> CharacterStatTable;
	TArray<TMap<FName, FSkillData>> CharacterDataTable;
};

template<typename DataStruct>
void UDataSingleton::Init(TMap<FName, DataStruct>& OutMap, const FString& InName)
{
	FString Path = TEXT("/Script/Engine.DataTable'/Game/A_Blaster/GameData/");
	FString New = FString::Printf(TEXT("%s.%s'"), *InName, *InName);
	Path.Append(New);

	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponStatTableRef(*Path);
	if (nullptr != WeaponStatTableRef.Object)
	{
		const UDataTable* DataTable = WeaponStatTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		TArray<FName> WeaponTypeArray;
		DataTable->GetRowMap().GenerateKeyArray(WeaponTypeArray);

		OutMap.Reserve(ValueArray.Num());
		for (size_t i = 0; i < ValueArray.Num(); i++)
		{
			OutMap.Emplace(WeaponTypeArray[i], *reinterpret_cast<DataStruct*>(ValueArray[i]));
		}
	}
	else
	{
		ensure(false);
	}
}
