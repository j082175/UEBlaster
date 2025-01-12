// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterData.h"

#include "MainMenu_Characters.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMainMenu_Characters : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainMenu_Characters(const FObjectInitializer& ObjectInitializer);
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UGridPanel> Grid;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> CharacterName_Text;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> CharacterDescription_Text;

private:
	void FillGrid();
	void GetCharacterInfo();

	void SetCharacterInfo(const FString& InStr);

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UDataTable> CharacterDT;

	int32 CurrRow;
	int32 CurrColumn;
	int32 MaxColumn = 3;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCharactersItem> CharactersItemClass;

	UPROPERTY()
	TArray<TObjectPtr<class UCharactersItem>> CharactersItemArr;

	TMap<FName, FCharacterData> CharacterDataMap;
};
