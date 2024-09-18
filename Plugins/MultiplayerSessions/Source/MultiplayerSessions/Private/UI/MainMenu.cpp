// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

#include "UI/MainMenu_Matches.h"

#include "Kismet/KismetSystemLibrary.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	SetActive();

	Home_Btn->OnClicked.AddUniqueDynamic(this, &ThisClass::HomeBtnClicked);
	Matches_Btn->OnClicked.AddUniqueDynamic(this, &ThisClass::MatchBtnClicked);
	Character_Btn->OnClicked.AddUniqueDynamic(this, &ThisClass::CharacterBtnClicked);
	Settings_Btn->OnClicked.AddUniqueDynamic(this, &ThisClass::SettingsBtnClicked);
	QuitGame_Btn->OnClicked.AddUniqueDynamic(this, &ThisClass::QuitBtnClicked);

}

void UMainMenu::HomeBtnClicked()
{
	Switcher->SetActiveWidgetIndex(0);
	SetActive();
}

void UMainMenu::MatchBtnClicked()
{
	Switcher->SetActiveWidgetIndex(1);
	SetActive();
	WBP_Matches->Refresh();
}

void UMainMenu::CharacterBtnClicked()
{
	Switcher->SetActiveWidgetIndex(2);
	SetActive();
}

void UMainMenu::SettingsBtnClicked()
{
	Switcher->SetActiveWidgetIndex(3);
	SetActive();
}

void UMainMenu::QuitBtnClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UMainMenu::SetActive()
{
	TArray<UButton*> BtnArr;
	BtnArr.Add(Home_Btn.Get());
	BtnArr.Add(Matches_Btn.Get());
	BtnArr.Add(Character_Btn);
	BtnArr.Add(Settings_Btn);
	BtnArr.Add(QuitGame_Btn);

	for (size_t i = 0; i < BtnArr.Num(); i++)
	{
		BtnArr[i]->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.5f));
		if (i == Switcher->GetActiveWidgetIndex())
		{
			BtnArr[i]->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
	}
}
