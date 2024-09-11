#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillStat.generated.h"

USTRUCT(BlueprintType)
struct FSkillStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSkillStat()
		: CoolTime(0.0f), MaintainTime(0.f), RequiredPoint(0.0f), bCanExecute(true), bSkillPointEnough(true)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float CoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaintainTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float RequiredPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bCanExecute : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bSkillPointEnough : 1;


	FSkillStat operator+(const FSkillStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FSkillStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FSkillStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
};
