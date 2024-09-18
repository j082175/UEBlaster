// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbySelector.h"

#include "Components/TextBlock.h"

void ULobbySelector::NativeConstruct()
{
	Super::NativeConstruct();

}

void ULobbySelector::SetText(const FString& InStr)
{
	Option_Text->SetText(FText::FromString(InStr));
}


int32 ULobbySelector::Switch(bool IsRight, int32 Max)
{
	if (IsRight)
	{
		++CurrentIndex;
		if (CurrentIndex >= Max) CurrentIndex = 0;
	}
	else
	{
		--CurrentIndex;
		if (CurrentIndex < 0) CurrentIndex = Max - 1;
	}

	return CurrentIndex;

	//else
	//{
	//	if (IsRight)
	//	{
	//		++CurrentIndex;
	//		if (CurrentIndex >= Options.Num()) CurrentIndex = 0;
	//	}
	//	else
	//	{
	//		--CurrentIndex;
	//		if (CurrentIndex < 0) CurrentIndex = Options.Num() - 1;
	//	}

	//	SelectedOption = Options[CurrentIndex];
	//	Option_Text->SetText(FText::FromString(SelectedOption));
	//	return CurrentIndex;
	//}


}
