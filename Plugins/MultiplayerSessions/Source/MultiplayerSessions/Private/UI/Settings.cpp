// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Settings.h"
#include "UI/LobbySelector.h"

#include "Components/Button.h"
#include "GameFramework/GameUserSettings.h"

void USettings::NativeConstruct()
{
	Super::NativeConstruct();

	GameUserSettings = GEngine->GetGameUserSettings();
	GameUserSettings->ValidateSettings();
	GameUserSettings->LoadSettings(true);
	//GameUserSettings->GetDefaultWindowMode();

	WBP_Selector_WindowMode->RightArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::OnNextClickedWindowMode);
	WBP_Selector_WindowMode->LeftArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::OnPreviousClickedWindowMode);

	WBP_Selector_Resolution->RightArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::OnNextClickedResolution);
	WBP_Selector_Resolution->LeftArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::OnPreviousClickedResolution);

	WBP_Selector_Graphics->RightArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::OnNextClickedGraphics);
	WBP_Selector_Graphics->LeftArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::OnPreviousClickedGraphics);

	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	

	ResolutionArr.Add(UserSettings->GetScreenResolution());
	ResolutionArr.Add(FIntPoint(1280, 720));
	ResolutionArr.Add(FIntPoint(1280, 960));
	ResolutionArr.Add(FIntPoint(1280, 1080));
	ResolutionArr.Add(FIntPoint(1600, 900));
	ResolutionArr.Add(FIntPoint(1600, 1200));
	ResolutionArr.Add(FIntPoint(1920, 1080));
	ResolutionArr.Add(FIntPoint(1920, 1200));
	ResolutionArr.Add(FIntPoint(2560, 1440));
	ResolutionArr.Add(FIntPoint(2560, 1600));


	GameUserSettings->SetFullscreenMode(static_cast<EWindowMode::Type>(0));
	GameUserSettings->SetScreenResolution(ResolutionArr[0]);
	GameUserSettings->SetOverallScalabilityLevel(3);
	
	WBP_Selector_Resolution->SetOptionIndex(0);
	WBP_Selector_Graphics->SetOptionIndex(3);
	GameUserSettings->ApplySettings(true);

	WBP_Selector_WindowMode->SetText(*UEnum::GetDisplayValueAsText(EWindowMode::Fullscreen).ToString());

	FIntPoint CurrentIntPoint = GameUserSettings->GetScreenResolution();
	WBP_Selector_Resolution->SetText(FString::Printf(TEXT("%d / %d"), CurrentIntPoint.X, CurrentIntPoint.Y));
	GetGraphicsQuality();
}

void USettings::OnNextClickedWindowMode()
{
	int32 Index = WBP_Selector_WindowMode->Switch(true, EWindowMode::NumWindowModes);
	GameUserSettings->SetFullscreenMode(static_cast<EWindowMode::Type>(Index));
	GameUserSettings->ApplySettings(true);

	EWindowMode::Type CurrentWindowMode = GetWindowMode();
	WBP_Selector_WindowMode->SetText(*UEnum::GetDisplayValueAsText(CurrentWindowMode).ToString());

}

void USettings::OnPreviousClickedWindowMode()
{
	int32 Index = WBP_Selector_WindowMode->Switch(false, EWindowMode::NumWindowModes);
	GameUserSettings->SetFullscreenMode(static_cast<EWindowMode::Type>(Index));
	GameUserSettings->ApplySettings(true);

	EWindowMode::Type CurrentWindowMode = GetWindowMode();
	WBP_Selector_WindowMode->SetText(*UEnum::GetDisplayValueAsText(CurrentWindowMode).ToString());

}

void USettings::OnNextClickedResolution()
{
	int32 Index = WBP_Selector_Resolution->Switch(true, ResolutionArr.Num());
	FIntPoint CurrentIntPoint = ResolutionArr[Index];

	GameUserSettings->SetScreenResolution(CurrentIntPoint);
	GameUserSettings->ApplySettings(true);

	WBP_Selector_Resolution->SetText(FString::Printf(TEXT("%d / %d"), CurrentIntPoint.X, CurrentIntPoint.Y));
}

void USettings::OnPreviousClickedResolution()
{
	int32 Index = WBP_Selector_Resolution->Switch(false, ResolutionArr.Num());
	FIntPoint CurrentIntPoint = ResolutionArr[Index];

	GameUserSettings->SetScreenResolution(CurrentIntPoint);
	GameUserSettings->ApplySettings(true);

	WBP_Selector_Resolution->SetText(FString::Printf(TEXT("%d / %d"), CurrentIntPoint.X, CurrentIntPoint.Y));
}

void USettings::OnNextClickedGraphics()
{
	int32 Index = WBP_Selector_Graphics->Switch(true, 5);

	GameUserSettings->SetOverallScalabilityLevel(Index);
	GameUserSettings->ApplySettings(true);

	GetGraphicsQuality();
}

void USettings::OnPreviousClickedGraphics()
{
	int32 Index = WBP_Selector_Graphics->Switch(false, 5);

	GameUserSettings->SetOverallScalabilityLevel(Index);
	GameUserSettings->ApplySettings(true);

	GetGraphicsQuality();
}

EWindowMode::Type USettings::GetWindowMode()
{
	EWindowMode::Type WM = GameUserSettings->GetFullscreenMode();

	switch (WM)
	{
	case EWindowMode::Fullscreen:
		WBP_Selector_Resolution->SetIsEnabled(true);
		break;
	case EWindowMode::WindowedFullscreen:
		WBP_Selector_Resolution->SetIsEnabled(false);
		break;
	case EWindowMode::Windowed:
		WBP_Selector_Resolution->SetIsEnabled(true);
		break;
	case EWindowMode::NumWindowModes:
		break;
	default:
		break;
	}

	return WM;
}

FIntPoint USettings::GetResoultion()
{
	FIntPoint IntPoint = GameUserSettings->GetScreenResolution();

	//FString Str = FString::Printf(TEXT("%d x %d"), IntPoint.X, IntPoint.Y);
	//WBP_Selector_Resolution->SetText(Str);

	return IntPoint;
}

int32 USettings::GetGraphicsQuality()
{
	int32 Option = GameUserSettings->GetOverallScalabilityLevel();

	FString CurrentOption;

	switch (Option)
	{
	case 0:
		// low
		CurrentOption = TEXT("Low");
		break;
	case 1:
		CurrentOption = TEXT("Medium");
		break;
	case 2:
		CurrentOption = TEXT("High");
		break;
	case 3:
		CurrentOption = TEXT("Epic");
		break;
	case 4:
		CurrentOption = TEXT("Ultra");
		break;
	default:
		break;
	}

	WBP_Selector_Graphics->SetText(CurrentOption);

	return Option;
}

EWindowMode::Type USettings::ParseWindowMode(EWindowMode::Type InCurrentWindowMode)
{
	FIntPoint DesktopResoultion = GameUserSettings->GetDesktopResolution();

	switch (InCurrentWindowMode)
	{
	case EWindowMode::Fullscreen:
		break;
	case EWindowMode::WindowedFullscreen:
		break;
	case EWindowMode::Windowed:
		break;
	case EWindowMode::NumWindowModes:
		break;
	default:
		break;
	}


	return EWindowMode::Type();
}
