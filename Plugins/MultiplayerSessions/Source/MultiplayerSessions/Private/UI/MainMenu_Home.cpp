// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu_Home.h"
#include "UI/LobbyButton.h"
#include "UI/LobbySelector.h"
#include "UI/Loading.h"
#include "UI/ServerNameTextBox.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

//#include "Interfaces/CommonPCFuncInterface.h"
#include "Kismet/GameplayStatics.h"

//#include "AdvancedSessions/Classes/CreateSessionCallbackProxyAdvanced.h"
//#include "AdvancedSessions/Classes/BlueprintDataDefinitions.h"
//#include "AdvancedSessions/Classes/AdvancedSessionsLibrary.h"

#include "MultiplayerSessionsSubsystem.h"

UMainMenu_Home::UMainMenu_Home(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<ULoading> LoadingClassRef(TEXT("/MultiplayerSessions/Widgets/WBP_Loading.WBP_Loading_C"));
	ensure(LoadingClassRef.Class);
	LoadingClass = LoadingClassRef.Class;

	static ConstructorHelpers::FClassFinder<UServerNameTextBox> ServerNameTextBoxClassRef(TEXT("/MultiplayerSessions/Widgets/MainMenu/Home/WBP_CreateSession.WBP_CreateSession_C"));
	ensure(ServerNameTextBoxClassRef.Class);
	ServerNameTextBoxClass = ServerNameTextBoxClassRef.Class;
}

void UMainMenu_Home::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	//MapPreviewImage->SetBrushFromTexture

	//for (size_t i = 0; i < static_cast<size_t>(ETeamMaps::MAX); i++)
	//{
	//	MapPreviewImageMap.Add(*UEnum::GetDisplayValueAsText(static_cast<ETeamMaps>(i)).ToString(), )
	//}


	//WBP_Selector_Map->OnPreviousButtonClicked.AddUniqueDynamic(this, &ThisClass::LeftArrowClicked_Map);
	WBP_Selector_Map->LeftArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::LeftArrowClicked_Map);
	WBP_Selector_Map->RightArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::RightArrowClicked_Map);

	WBP_Selector_GameMode->LeftArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::LeftArrowClicked_GameMode);
	WBP_Selector_GameMode->RightArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::RightArrowClicked_GameMode);

	WBP_Selector_Players->LeftArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::LeftArrowClicked_Players);
	WBP_Selector_Players->RightArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::RightArrowClicked_Players);

	WBP_Button_Create->Button->OnClicked.AddUniqueDynamic(this, &ThisClass::CreateButtonClicked);

	WBP_Button_Create->ButtonText->SetText(FText::FromString(TEXT("Create")));

	//if (static_cast<uint8>(EFreeForAllMaps::MAX) == 0)
	//{
	//	WBP_Selector_Map->SetText(TEXT("None"));
	//}
	//else
	//{
	//	WBP_Selector_Map->SetText(*UEnum::GetDisplayValueAsText(static_cast<EFreeForAllMaps>(0)).ToString());
	//}

	PlayerArr = { 2, 4, 6, 8, 12, 16, 20 };

	WBP_Selector_Map->SetText(*UEnum::GetDisplayValueAsText(static_cast<ETeamMaps>(0)).ToString());
	WBP_Selector_Players->SetText(FString::FromInt(MaxPlayer));
	WBP_Selector_GameMode->SetText(*UEnum::GetDisplayValueAsText(CurrentMatchType).ToString());
	MapPreviewImage->SetBrushFromTexture(TeamsMapPreviewImageMap[CurrentTeamMap]);
}

void UMainMenu_Home::LeftArrowClicked_Map()
{
	SelectMap(false);
}

void UMainMenu_Home::RightArrowClicked_Map()
{
	SelectMap(true);
}

void UMainMenu_Home::LeftArrowClicked_GameMode()
{
	CurrentMatchType = static_cast<EMatchTypes>(WBP_Selector_Map->Switch(false, static_cast<uint8>(EMatchTypes::MAX)));

	WBP_Selector_GameMode->SetText(*UEnum::GetDisplayValueAsText(CurrentMatchType).ToString());
}

void UMainMenu_Home::RightArrowClicked_GameMode()
{
	CurrentMatchType = static_cast<EMatchTypes>(WBP_Selector_Map->Switch(true, static_cast<uint8>(EMatchTypes::MAX)));

	WBP_Selector_GameMode->SetText(*UEnum::GetDisplayValueAsText(CurrentMatchType).ToString());
}

void UMainMenu_Home::LeftArrowClicked_Players()
{
	int32 Index = WBP_Selector_Players->Switch(false, PlayerArr.Num());
	MaxPlayer = PlayerArr[Index];
	WBP_Selector_Players->SetText(FString::FromInt(PlayerArr[Index]));
}

void UMainMenu_Home::RightArrowClicked_Players()
{
	int32 Index = WBP_Selector_Players->Switch(true, PlayerArr.Num());
	MaxPlayer = PlayerArr[Index];
	WBP_Selector_Players->SetText(FString::FromInt(PlayerArr[Index]));
}

void UMainMenu_Home::CreateButtonClicked()
{
	//TArray<FSessionPropertyKeyPair> ExtraSettings;
	//ExtraSettings.Add(UAdvancedSessionsLibrary::MakeLiteralSessionPropertyString(TEXT("ServerName"), TEXT("MyFirstServer")));
	//ExtraSettings.Add(UAdvancedSessionsLibrary::MakeLiteralSessionPropertyString(TEXT("GameName"), TEXT("MyFirstGame")));


	//if (UCreateSessionCallbackProxyAdvanced* Result = UCreateSessionCallbackProxyAdvanced::CreateAdvancedSession(this, ExtraSettings, GetOwningPlayer(), MaxPlayer, 0, bUseLAN))
	//{
	//	Result->Activate();
	//	Result->OnSuccess.AddUniqueDynamic(this, &ThisClass::OnSuccess);
	//}

	ServerNameTextBox = CreateWidget<UServerNameTextBox>(this, ServerNameTextBoxClass);
	ServerNameTextBox->MainMenu_Home = this;
	ServerNameTextBox->AddToViewport();

}

void UMainMenu_Home::CreateButtonFinished(const FString& InServerName)
{
	if (MultiplayerSessionsSubsystem.IsValid())
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddUniqueDynamic(this, &ThisClass::OnCreateSessionFinished);
		MultiplayerSessionsSubsystem->CreateSession(MaxPlayer, *UEnum::GetDisplayValueAsText(CurrentMatchType).ToString(), SelectType(CurrentMatchType), InServerName);

		ServerNameTextBox->RemoveFromViewport();
	}
}

void UMainMenu_Home::OnCreateSessionFinished(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		LoadingWidget = CreateWidget<ULoading>(this, LoadingClass);
		LoadingWidget->AddToViewport();

		FTimerHandle H;
		GetWorld()->GetTimerManager().SetTimer(H, this, &ThisClass::CreateSession, 2.f, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Create Advanced Session Failure"));
	}

}

void UMainMenu_Home::CreateSession()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, *UEnum::GetDisplayValueAsText(EDefaultMaps::LobbyMap_SciFi_Dynamic).ToString());
	}
	UGameplayStatics::OpenLevel(this, *UEnum::GetDisplayValueAsText(EDefaultMaps::LobbyMap_SciFi_Dynamic).ToString(), true, TEXT("listen"));
	LoadingWidget->RemoveFromViewport();
}

void UMainMenu_Home::SelectMap(bool IsRight)
{
	switch (CurrentMatchType)
	{
		//case EMatchTypes::FreeForAll:
		//	if (static_cast<uint8>(EFreeForAllMaps::MAX) == 0)
		//	{
		//		FString Str = FString::Printf(TEXT("There is no maps about : %s"), *UEnum::GetDisplayValueAsText(EMatchTypes::FreeForAll).ToString());
		//		WBP_Selector_Map->SetText(Str);
		//	}
		//	else
		//	{
		//		CurrentFreeForAllMap = static_cast<EFreeForAllMaps>(WBP_Selector_Map->Switch(IsRight, static_cast<uint8>(EFreeForAllMaps::MAX)));
		//		WBP_Selector_Map->SetText(*UEnum::GetDisplayValueAsText(CurrentFreeForAllMap).ToString());
		//	}

		//	break;
	case EMatchTypes::Teams:
		if (static_cast<uint8>(ETeamMaps::MAX) == 0)
		{
			FString Str = FString::Printf(TEXT("There is no maps about : %s"), *UEnum::GetDisplayValueAsText(EMatchTypes::Teams).ToString());
			WBP_Selector_Map->SetText(Str);
		}
		else
		{
			CurrentTeamMap = static_cast<ETeamMaps>(WBP_Selector_Map->Switch(IsRight, static_cast<uint8>(ETeamMaps::MAX)));
			FString MapName = UEnum::GetDisplayValueAsText(CurrentTeamMap).ToString();
			WBP_Selector_Map->SetText(MapName);
			MapPreviewImage->SetBrushFromTexture(TeamsMapPreviewImageMap[CurrentTeamMap]);
		}
		break;
	case EMatchTypes::CaptureTheFlag:
		if (static_cast<uint8>(ECaptureTheFlagMaps::MAX) == 0)
		{
			FString Str = FString::Printf(TEXT("There is no maps about : %s"), *UEnum::GetDisplayValueAsText(EMatchTypes::CaptureTheFlag).ToString());
			WBP_Selector_Map->SetText(Str);
		}
		else
		{
			CurrentCaptureTheFlagMap = static_cast<ECaptureTheFlagMaps>(WBP_Selector_Map->Switch(IsRight, static_cast<uint8>(ECaptureTheFlagMaps::MAX)));
			FString MapName = UEnum::GetDisplayValueAsText(CurrentCaptureTheFlagMap).ToString();
			WBP_Selector_Map->SetText(MapName);
			MapPreviewImage->SetBrushFromTexture(CTFMapPreviewImageMap[CurrentCaptureTheFlagMap]);
		}
		break;
	case EMatchTypes::MAX:
		break;
	default:
		break;
	}
}

FString UMainMenu_Home::SelectType(EMatchTypes InCurrentMatchType)
{
	FString Result;

	switch (InCurrentMatchType)
	{
		//case EMatchTypes::FreeForAll:
		//	Result = *UEnum::GetDisplayValueAsText(CurrentFreeForAllMap).ToString();
		//	break;
	case EMatchTypes::Teams:
		Result = *UEnum::GetDisplayValueAsText(CurrentTeamMap).ToString();
		break;
	case EMatchTypes::CaptureTheFlag:
		Result = *UEnum::GetDisplayValueAsText(CurrentCaptureTheFlagMap).ToString();
		break;
	case EMatchTypes::MAX:
		break;
	default:
		break;
	}

	return Result;
}
