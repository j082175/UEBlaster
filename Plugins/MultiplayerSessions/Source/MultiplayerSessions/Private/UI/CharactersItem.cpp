// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharactersItem.h"
#include "Components/Button.h"
#include "Engine/Texture2D.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

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

}

void UCharactersItem::ButtonHovered()
{
	
}
