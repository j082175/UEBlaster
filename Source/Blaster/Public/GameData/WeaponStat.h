
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponStat.generated.h"

USTRUCT(BlueprintType)
struct FWeaponStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FWeaponStat() 
		: BodyDamage(0.0f), HeadDamage(0.f), DamageDeviation(0.f), RecoilPitch(0.0f), RecoilYaw(0.f), Range(0.0f), FireDelay(0.0f), MagCapacity(0.f)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float BodyDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float HeadDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float DamageDeviation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float RecoilPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float RecoilYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float FireDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MagCapacity;

	FWeaponStat operator+(const FWeaponStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FWeaponStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FWeaponStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
};
