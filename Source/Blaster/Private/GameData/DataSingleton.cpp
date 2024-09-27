// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/DataSingleton.h"
#include "Blaster.h"

DEFINE_LOG_CATEGORY(LogABGameSingleton);

//template<typename DataStruct, typename EnumType>
//UDataSingleton<DataStruct, EnumType>::UDataSingleton()
//{
//	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponStatTableRef(TEXT("/Script/Engine.DataTable'/Game/A_Blaster/GameData/DT_WeaponStat.DT_WeaponStat'"));
//	if (nullptr != WeaponStatTableRef.Object)
//	{
//		const UDataTable* DataTable = WeaponStatTableRef.Object;
//		check(DataTable->GetRowMap().Num() > 0);
//
//		TArray<uint8*> ValueArray;
//		DataTable->GetRowMap().GenerateValueArray(ValueArray);
//		TArray<FName> WeaponTypeArray;
//		DataTable->GetRowMap().GenerateKeyArray(WeaponTypeArray);
//
//		WeaponStatMap.Reserve(ValueArray.Num());
//		for (size_t i = 0; i < ValueArray.Num(); i++)
//		{
//			WeaponStatMap.Emplace(WeaponTypeArray[i], *reinterpret_cast<FWeaponStat*>(ValueArray[i]));
//		}
//	}
//
//	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponDataTableRef(TEXT("/Script/Engine.DataTable'/Game/A_Blaster/GameData/DT_WeaponData.DT_WeaponData'"));
//	if (nullptr != WeaponDataTableRef.Object)
//	{
//		const UDataTable* DataTable = WeaponDataTableRef.Object;
//		check(DataTable->GetRowMap().Num() > 0);
//
//		TArray<uint8*> ValueArray;
//		DataTable->GetRowMap().GenerateValueArray(ValueArray);
//		TArray<FName> WeaponTypeArray;
//		DataTable->GetRowMap().GenerateKeyArray(WeaponTypeArray);
//
//		WeaponDataMap.Reserve(ValueArray.Num());
//		for (size_t i = 0; i < ValueArray.Num(); i++)
//		{
//			WeaponDataMap.Emplace(WeaponTypeArray[i], *reinterpret_cast<FWeaponData*>(ValueArray[i]));
//		}
//	}
//
//	static ConstructorHelpers::FObjectFinder<UDataTable> SkillStatTableRef(TEXT("/Script/Engine.DataTable'/Game/A_Blaster/GameData/DT_SkillStat.DT_SkillStat'"));
//	if (nullptr != SkillStatTableRef.Object)
//	{
//		const UDataTable* DataTable = SkillStatTableRef.Object;
//		check(DataTable->GetRowMap().Num() > 0);
//
//		TArray<uint8*> ValueArray;
//		DataTable->GetRowMap().GenerateValueArray(ValueArray);
//		TArray<FName> WeaponTypeArray;
//		DataTable->GetRowMap().GenerateKeyArray(WeaponTypeArray);
//
//		SkillStatMap.Reserve(ValueArray.Num());
//		for (size_t i = 0; i < ValueArray.Num(); i++)
//		{
//			SkillStatMap.Emplace(WeaponTypeArray[i], *reinterpret_cast<FSkillStat*>(ValueArray[i]));
//		}
//	}
//
//	static ConstructorHelpers::FObjectFinder<UDataTable> SkillDataTableRef(TEXT("/Script/Engine.DataTable'/Game/A_Blaster/GameData/DT_SkillData.DT_SkillData'"));
//	if (nullptr != SkillDataTableRef.Object)
//	{
//		const UDataTable* DataTable = SkillDataTableRef.Object;
//		check(DataTable->GetRowMap().Num() > 0);
//
//		TArray<uint8*> ValueArray;
//		DataTable->GetRowMap().GenerateValueArray(ValueArray);
//		TArray<FName> WeaponTypeArray;
//		DataTable->GetRowMap().GenerateKeyArray(WeaponTypeArray);
//
//		SkillDataMap.Reserve(ValueArray.Num());
//		for (size_t i = 0; i < ValueArray.Num(); i++)
//		{
//			SkillDataMap.Emplace(WeaponTypeArray[i], *reinterpret_cast<FSkillData*>(ValueArray[i]));
//		}
//	}
//
//
//
//
//	CharacterMaxLevel = WeaponStatMap.Num();
//	ensure(CharacterMaxLevel > 0);
//
//	AB_CALLLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
//}


UDataSingleton::UDataSingleton()
{

	Init<FWeaponStat>(WeaponStatMap, TEXT("DT_WeaponStat"));
	Init<FWeaponData>(WeaponDataMap, TEXT("DT_WeaponData"));
	Init<FSkillStat>(SkillStatMap, TEXT("DT_SkillStat_Wraith"));
	Init<FSkillData>(SkillDataMap, TEXT("DT_SkillData_Wraith"));
	CharacterStatTable.Add(SkillStatMap);
	CharacterDataTable.Add(SkillDataMap);
	//Init<FSkillStat>(SkillStatMap, TEXT("DT_SkillStat_Belica"));
	//Init<FSkillData>(SkillDataMap, TEXT("DT_SkillData_Belica"));
	//CharacterStatTable.Add(SkillStatMap);
	//CharacterDataTable.Add(SkillDataMap);
	//Init<FSkillStat>(SkillStatMap, TEXT("DT_SkillStat_Murdock"));
	//Init<FSkillData>(SkillDataMap, TEXT("DT_SkillData_Murdock"));
	//CharacterStatTable.Add(SkillStatMap);
	//CharacterDataTable.Add(SkillDataMap);

	CharacterMaxLevel = WeaponStatMap.Num();
	ensure(CharacterMaxLevel > 0);

	AB_CALLLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
}


UDataSingleton& UDataSingleton::Get()
{
	if (GEngine)
	{
		UDataSingleton* Singleton = CastChecked<UDataSingleton>(GEngine->GameSingleton);
		if (Singleton)
		{
			return *Singleton;
		}
	}


	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UDataSingleton>();
}



//template<typename DataStruct, typename EnumType>
//UDataSingleton& UDataSingleton<DataStruct, EnumType>::Get()
//{
//	if (GEngine)
//	{
//		UDataSingleton* Singleton = CastChecked<UDataSingleton>(GEngine->GameSingleton);
//		if (Singleton)
//		{
//			return *Singleton;
//		}
//	}
//
//
//	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));
//	return *NewObject<UDataSingleton>();
//}


//UDataSingleton& UDataSingleton::Get()
//{
//	if (GEngine)
//	{
//		UDataSingleton* Singleton = CastChecked<UDataSingleton>(GEngine->GameSingleton);
//		if (Singleton)
//		{
//			return *Singleton;
//		}
//	}
//
//
//	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));
//	return *NewObject<UDataSingleton>();
//}

FWeaponStat UDataSingleton::GetWeaponName(EWeaponName InWeaponType) const
{
	FName EnumName = FName(UEnum::GetDisplayValueAsText(InWeaponType).ToString());

	return WeaponStatMap.Contains(EnumName) ? WeaponStatMap[EnumName] : FWeaponStat();
}

FWeaponData UDataSingleton::GetWeaponData(EWeaponName InWeaponType) const
{
	FName EnumName = FName(UEnum::GetDisplayValueAsText(InWeaponType).ToString());

	return WeaponDataMap.Contains(EnumName) ? WeaponDataMap[EnumName] : FWeaponData();
}

FSkillStat UDataSingleton::GetSkillStat(ECharacterTypes InCharacterType, const FName& Index) const
{
	TMap<FName, FSkillStat> Map = CharacterStatTable[static_cast<int32>(InCharacterType)];
	
	return Map.Contains(Index) ? Map[Index] : FSkillStat();

	//FName EnumName = FName(UEnum::GetDisplayValueAsText(InCharacterType).ToString());

	//return SkillStatMap.Contains(EnumName) ? SkillStatMap[EnumName] : FSkillStat();
}

FSkillData UDataSingleton::GetSkillData(ECharacterTypes InCharacterType, const FName& Index) const
{
	TMap<FName, FSkillData> Map = CharacterDataTable[static_cast<int32>(InCharacterType)];
	return Map.Contains(Index) ? Map[Index] : FSkillData();

	//FName EnumName = FName(UEnum::GetDisplayValueAsText(InCharacterType).ToString());

	//return SkillDataMap.Contains(EnumName) ? SkillDataMap[EnumName] : FSkillData();
}



