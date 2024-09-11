// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/DataSingleton.h"
#include "Blaster.h"

DEFINE_LOG_CATEGORY(LogABGameSingleton);

UDataSingleton::UDataSingleton()
{

	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponStatTableRef(TEXT("/Script/Engine.DataTable'/Game/A_Blaster/GameData/DA_WeaponStat.DA_WeaponStat'"));
	if (nullptr != WeaponStatTableRef.Object)
	{
		const UDataTable* DataTable = WeaponStatTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		TArray<FName> WeaponTypeArray;
		DataTable->GetRowMap().GenerateKeyArray(WeaponTypeArray);

		WeaponStatMap.Reserve(ValueArray.Num());
		for (size_t i = 0; i < ValueArray.Num(); i++)
		{
			WeaponStatMap.Emplace(WeaponTypeArray[i], *reinterpret_cast<FWeaponStat*>(ValueArray[i]));
		}
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponDataTableRef(TEXT("/Script/Engine.DataTable'/Game/A_Blaster/GameData/DA_WeaponData.DA_WeaponData'"));
	if (nullptr != WeaponDataTableRef.Object)
	{
		const UDataTable* DataTable = WeaponDataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		TArray<FName> WeaponTypeArray;
		DataTable->GetRowMap().GenerateKeyArray(WeaponTypeArray);

		WeaponDataMap.Reserve(ValueArray.Num());
		for (size_t i = 0; i < ValueArray.Num(); i++)
		{
			WeaponDataMap.Emplace(WeaponTypeArray[i], *reinterpret_cast<FWeaponData*>(ValueArray[i]));
		}
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> SkillStatTableRef(TEXT("/Script/Engine.DataTable'/Game/A_Blaster/GameData/DA_SkillStat.DA_SkillStat'"));
	if (nullptr != SkillStatTableRef.Object)
	{
		const UDataTable* DataTable = SkillStatTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		TArray<FName> WeaponTypeArray;
		DataTable->GetRowMap().GenerateKeyArray(WeaponTypeArray);

		SkillStatMap.Reserve(ValueArray.Num());
		for (size_t i = 0; i < ValueArray.Num(); i++)
		{
			SkillStatMap.Emplace(WeaponTypeArray[i], *reinterpret_cast<FSkillStat*>(ValueArray[i]));
		}
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> SkillDataTableRef(TEXT("/Script/Engine.DataTable'/Game/A_Blaster/GameData/DA_SkillData.DA_SkillData'"));
	if (nullptr != SkillDataTableRef.Object)
	{
		const UDataTable* DataTable = SkillDataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		TArray<FName> WeaponTypeArray;
		DataTable->GetRowMap().GenerateKeyArray(WeaponTypeArray);

		SkillDataMap.Reserve(ValueArray.Num());
		for (size_t i = 0; i < ValueArray.Num(); i++)
		{
			SkillDataMap.Emplace(WeaponTypeArray[i], *reinterpret_cast<FSkillData*>(ValueArray[i]));
		}
	}




	CharacterMaxLevel = WeaponStatMap.Num();
	ensure(CharacterMaxLevel > 0);

	AB_CALLLOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
}

UDataSingleton& UDataSingleton::Get()
{
	UDataSingleton* Singleton = CastChecked<UDataSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UDataSingleton>();
}

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

FSkillStat UDataSingleton::GetSkillStat(ESkillAssistant InWeaponType) const
{
	FName EnumName = FName(UEnum::GetDisplayValueAsText(InWeaponType).ToString());

	return SkillStatMap.Contains(EnumName) ? SkillStatMap[EnumName] : FSkillStat();
}

FSkillData UDataSingleton::GetSkillData(ESkillAssistant InWeaponType) const
{
	FName EnumName = FName(UEnum::GetDisplayValueAsText(InWeaponType).ToString());

	return SkillDataMap.Contains(EnumName) ? SkillDataMap[EnumName] : FSkillData();
}
