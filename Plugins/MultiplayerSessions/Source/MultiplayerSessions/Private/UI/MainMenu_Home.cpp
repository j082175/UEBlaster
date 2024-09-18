// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu_Home.h"
#include "UI/LobbyButton.h"
#include "UI/LobbySelector.h"
#include "UI/Loading.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

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
}

void UMainMenu_Home::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	MapLength = static_cast<int32>(EMapNames::MAX);
	MapArr.Reserve(MapLength);

	for (size_t i = 0; i < MapLength; i++)
	{
		MapArr.Add(static_cast<EMapNames>(i));
	}

	WBP_Selector_Map->SetText(*UEnum::GetDisplayValueAsText(MapArr[0]).ToString());


	//WBP_Selector_Map->OnPreviousButtonClicked.AddUniqueDynamic(this, &ThisClass::LeftArrowClicked_Map);
	WBP_Selector_Map->LeftArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::LeftArrowClicked_Map);
	WBP_Selector_Map->RightArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::RightArrowClicked_Map);

	WBP_Selector_LAN->LeftArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::LeftArrowClicked_LAN);
	WBP_Selector_LAN->RightArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::RightArrowClicked_LAN);

	WBP_Selector_Players->LeftArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::LeftArrowClicked_Players);
	WBP_Selector_Players->RightArrow->OnClicked.AddUniqueDynamic(this, &ThisClass::RightArrowClicked_Players);

	WBP_Button_Create->Button->OnClicked.AddUniqueDynamic(this, &ThisClass::CreateButtonClicked);

	WBP_Button_Create->ButtonText->SetText(FText::FromString(TEXT("Create")));

	WBP_Selector_Players->SetText(TEXT("0"));
	WBP_Selector_LAN->SetText(TEXT("Off"));
}

void UMainMenu_Home::LeftArrowClicked_Map()
{
	CurrentMap = static_cast<EMapNames>(WBP_Selector_Map->Switch(false, MapArr.Num()));
	WBP_Selector_Map->SetText(*UEnum::GetDisplayValueAsText(CurrentMap).ToString());
}

void UMainMenu_Home::RightArrowClicked_Map()
{
	CurrentMap = static_cast<EMapNames>(WBP_Selector_Map->Switch(true, MapArr.Num()));
	WBP_Selector_Map->SetText(*UEnum::GetDisplayValueAsText(CurrentMap).ToString());
}

void UMainMenu_Home::LeftArrowClicked_LAN()
{
	bUseLAN = bUseLAN == true ? false : true;
	FString Str = bUseLAN == true ? TEXT("On") : TEXT("Off");
	WBP_Selector_LAN->SetText(Str);
}

void UMainMenu_Home::RightArrowClicked_LAN()
{
	bUseLAN = bUseLAN == true ? false : true;
	FString Str = bUseLAN == true ? TEXT("On") : TEXT("Off");
	WBP_Selector_LAN->SetText(Str);
}

void UMainMenu_Home::LeftArrowClicked_Players()
{
	CurrentPlayer = WBP_Selector_Players->Switch(false, MaxPlayer);
	WBP_Selector_Players->SetText(FString::FromInt(CurrentPlayer));
}

void UMainMenu_Home::RightArrowClicked_Players()
{
	CurrentPlayer = WBP_Selector_Players->Switch(true, MaxPlayer);
	WBP_Selector_Players->SetText(FString::FromInt(CurrentPlayer));
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
	if (MultiplayerSessionsSubsystem.IsValid())
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddUniqueDynamic(this, &ThisClass::OnCreateSessionFinished);
		MultiplayerSessionsSubsystem->CreateSession(MaxPlayer, TEXT("FreeForAll"));
	}

}

void UMainMenu_Home::OnCreateSessionFinished(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		LoadingWidget = CreateWidget<ULoading>(this, LoadingClass);
		LoadingWidget->AddToViewport();



		FTimerHandle H;
		GetWorld()->GetTimerManager().SetTimer(H, FTimerDelegate::CreateLambda([&]()
			{
				UGameplayStatics::OpenLevel(this, *UEnum::GetDisplayValueAsText(CurrentMap).ToString(), true, TEXT("listen"));
				LoadingWidget->RemoveFromViewport();
			}), 2.f, false);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Create Advanced Session Failure"));
	}

}

