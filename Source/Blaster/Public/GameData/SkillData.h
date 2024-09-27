#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BlasterTypes/SkillAssistant.h"
#include "SkillData.generated.h"

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSkillData()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	ESkillType SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	ESkillAnimType SkillAnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TSubclassOf<AActor> SpawnActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class UTexture2D> SkillImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class USoundBase> SkillCastingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class UParticleSystem> SkillEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class UNiagaraSystem> SkillEffect_Sub; // If Effect exists, should be blank

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class UAnimMontage> SkillMontage; // If Effect exists, should be blank
};
