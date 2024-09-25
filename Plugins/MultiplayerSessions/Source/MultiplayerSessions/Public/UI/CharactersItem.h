// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterData.h"
#include "CharactersItem.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UCharactersItem : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	FORCEINLINE void SetCharacterData(const FCharacterData& InCharacterData) { CharacterData = InCharacterData; }

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button;

private:
	UFUNCTION()
	void ButtonClicked();
	UFUNCTION()
	void ButtonHovered();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FCharacterData CharacterData;
};
