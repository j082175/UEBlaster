// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu_Characters.h"
#include "Components/GridPanel.h"
#include "Components/TextBlock.h"
#include "UI/CharactersItem.h"

#include "Kismet/DataTableFunctionLibrary.h"


UMainMenu_Characters::UMainMenu_Characters(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterDTRef(TEXT("/Script/Engine.DataTable'/MultiplayerSessions/DT_CharacterData.DT_CharacterData'"));
	ensure(CharacterDTRef.Object);
	CharacterDT = CharacterDTRef.Object;

	static ConstructorHelpers::FClassFinder<UCharactersItem> CharactersItemClassRef(TEXT("/MultiplayerSessions/Widgets/MainMenu/Characters/WBP_Characters_Item.WBP_Characters_Item_C"));
	ensure(CharactersItemClassRef.Class);
	CharactersItemClass = CharactersItemClassRef.Class;
}

void UMainMenu_Characters::NativePreConstruct()
{
	Super::NativePreConstruct();

	FillGrid();
	GetCharacterInfo();
}

void UMainMenu_Characters::FillGrid()
{

	check(CharacterDT->GetRowMap().Num() > 0);

	TArray<uint8*> ValueArray;
	CharacterDT->GetRowMap().GenerateValueArray(ValueArray);
	TArray<FName> CharacterArray;
	CharacterDT->GetRowMap().GenerateKeyArray(CharacterArray);

	CharacterDataMap.Reserve(ValueArray.Num());
	for (size_t i = 0; i < ValueArray.Num(); i++)
	{
		CharacterDataMap.Emplace(CharacterArray[i], *reinterpret_cast<FCharacterData*>(ValueArray[i]));
	}


	for (const auto& CharacterData : CharacterDataMap)
	{
		CharactersItem = CreateWidget<UCharactersItem>(this, CharactersItemClass);
		if (CharactersItem)
		{
			CharactersItem->SetCharacterData(CharacterData.Value);
			Grid->AddChildToGrid(CharactersItem, CurrRow, CurrColumn);
			++CurrColumn;
		}

		if (CurrColumn == MaxColumn)
		{
			++CurrRow;
			CurrColumn = 0;
		}
	}
}

void UMainMenu_Characters::GetCharacterInfo()
{
	//CharacterName_Text->SetText()
}
