// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"
#include "PlayerController/LobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Enums/MapNames.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("ALobbyGameMode::BeginPlay Called")));
	//}

	//bUseSeamlessTravel = true;
}

//void ALobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
//{
//	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
//
//	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
//	if (NumberOfPlayers >= 2)
//	{
//		if (GetWorld())
//		{
//			bUseSeamlessTravel = true;
//			GetWorld()->ServerTravel(TEXT("/Game/A_Blaster/Maps/GameMap_Hanamura?listen"));
//		}
//	}
//}
//
//APlayerController* ALobbyGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
//{
//	APlayerController* Controller = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
//
//	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
//	if (NumberOfPlayers >= 2)
//	{
//		if (GetWorld())
//		{
//			bUseSeamlessTravel = true;
//			GetWorld()->ServerTravel(TEXT("/Game/A_Blaster/Maps/GameMap_Hanamura?listen"));
//		}
//	}
//
//	return Controller;
//}

void ALobbyGameMode::PostLogin(APlayerController* PlayerController)
{
	Super::PostLogin(PlayerController);

	if (GetWorld())
	{
		if (PlayerController->HasAuthority())
		{
			if (ALobbyPlayerController* LPC = Cast<ALobbyPlayerController>(PlayerController))
			{
				LPC->CreateStartButton();
			}
		}
	}

	//int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	//UGameInstance* GameInstance = GetGameInstance();
	//if (GameInstance)
	//{
	//	UMultiplayerSessionsSubsystem* SubSystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();


	//	if (NumberOfPlayers == SubSystem->DesiredNumPublicConnections)
	//	{
	//		if (GetWorld())
	//		{
	//			bUseSeamlessTravel = true;

	//			FString MatchType = SubSystem->DesiredMatchType;
	//			FString DesiredMap = SubSystem->DesiredMap;


	//			//UGameplayStatics::OpenLevel(this, *DesiredMap, true, TEXT("listen"));

	//			for (size_t i = 0; i < static_cast<size_t>(ETeamMaps::MAX); i++)
	//			{
	//				FString Path = DEFAULT_GAMEMAP_PATH;
	//				Path.Append(*UEnum::GetDisplayValueAsText(static_cast<ETeamMaps>(i)).ToString());
	//				FFilePath FilePath{ Path };
	//				MapPath.Add(FilePath);
	//			}
	//			for (size_t i = 0; i < static_cast<size_t>(ECaptureTheFlagMaps::MAX); i++)
	//			{
	//				FString Path = DEFAULT_GAMEMAP_PATH;
	//				Path.Append(*UEnum::GetDisplayValueAsText(static_cast<ECaptureTheFlagMaps>(i)).ToString());
	//				FFilePath FilePath{ Path };
	//				MapPath.Add(FilePath);
	//			}


	//			for (size_t i = 0; i < MapPath.Num(); i++)
	//			{
	//				if (MapPath[i].FilePath.Contains(DesiredMap))
	//				{
	//					FString P = MapPath[i].FilePath.Append(TEXT("?listen"));
	//					GetWorld()->ServerTravel(P);
	//					return;
	//				}
	//			}

	//			ensure(false);
	//			return;

	//		}
	//	}
	//}
}