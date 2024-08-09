
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FWeaponData() : Damage(0.0f), Recoil(0.0f), Range(0.0f), FireDelay(0.0f), MagCapacity(0.f){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Recoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float FireDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MagCapacity;

	FWeaponData operator+(const FWeaponData& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FWeaponData Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FWeaponData) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
};
