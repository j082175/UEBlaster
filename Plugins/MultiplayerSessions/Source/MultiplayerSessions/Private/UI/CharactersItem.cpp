// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharactersItem.h"
#include "Components/Button.h"
#include "Engine/Texture2D.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

ECharacterTypes UCharactersItem::SelectedCharacterType(ECharacterTypes::Wraith);

void UCharactersItem::NativeConstruct()
{
	Super::NativeConstruct();

	//CharacterData.CharacterImage->size

	Button->OnClicked.AddUniqueDynamic(this, &ThisClass::ButtonClicked);
	Button->OnHovered.AddUniqueDynamic(this, &ThisClass::ButtonHovered);

	FSlateBrush SlateBrush = UWidgetBlueprintLibrary::MakeBrushFromTexture(CharacterData.CharacterImage, 200, 200);

	FButtonStyle ButtonStyle = Button->GetStyle();
	ButtonStyle.Normal = SlateBrush;
	ButtonStyle.Hovered = SlateBrush;
	ButtonStyle.Pressed = SlateBrush;
	ButtonStyle.Disabled = SlateBrush;

	Button->SetStyle(ButtonStyle);
}

void UCharactersItem::ButtonClicked()
{
	if (CharacterData.DisplayName.ToString() == TEXT("Wraith"))
	{
		SelectedCharacterType = ECharacterTypes::Wraith;
	}
	else if (CharacterData.DisplayName.ToString() == TEXT("Lt.Belica"))
	{
		SelectedCharacterType = ECharacterTypes::Belica;
	}

	OnCharacterChanged.ExecuteIfBound(*UEnum::GetDisplayValueAsText(SelectedCharacterType).ToString());
	UE_LOG(LogTemp, Display, TEXT("SelectedCharacter : %s"), *UEnum::GetDisplayValueAsText(SelectedCharacterType).ToString());
}

void UCharactersItem::ButtonHovered()
{
	//UE_LOG(LogTemp, Display, TEXT("CharactersItem Hovering"));
}
