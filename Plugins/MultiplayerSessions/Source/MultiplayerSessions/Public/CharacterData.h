#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Internationalization/Text.h"

#include "CharacterData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FCharacterData()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class UTexture2D> CharacterImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TSubclassOf<class ACharacter> PlayerCharacter;

};
