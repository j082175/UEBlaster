// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/DataSingleton.h"

DEFINE_LOG_CATEGORY(LogABGameSingleton);

UDataSingleton::UDataSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/A_Blaster/GameData/DA_WeaponData.DA_WeaponData'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
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

		//Algo::Transform(ValueArray, WeaponDataMap,
		//	[](uint8* Value)
		//	{
		//		return *reinterpret_cast<FWeaponData*>(Value);
		//	}
		//);
	}

	CharacterMaxLevel = WeaponDataMap.Num();
	ensure(CharacterMaxLevel > 0);
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

FWeaponData UDataSingleton::GetWeaponName(EWeaponName InWeaponType) const
{
	FName EnumName = FName(UEnum::GetDisplayValueAsText(InWeaponType).ToString());

	return WeaponDataMap.Contains(EnumName) ? WeaponDataMap[EnumName] : FWeaponData();
}
