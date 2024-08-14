// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
//#include "MultiplayerSessionsSubsystem.h"

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
//			GetWorld()->ServerTravel(TEXT("/Game/A_Blaster/Maps/GameMap_Tokyo?listen"));
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
//			GetWorld()->ServerTravel(TEXT("/Game/A_Blaster/Maps/GameMap_Tokyo?listen"));
//		}
//	}
//
//	return Controller;
//}

void ALobbyGameMode::PostLogin(APlayerController* PlayerController)
{
	Super::PostLogin(PlayerController);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		//UMultiplayerSessionsSubsystem* SubSystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();


		//if (NumberOfPlayers == SubSystem->DesiredNumPublicConnections)
		//{
		//	if (GetWorld())
		//	{
		//		bUseSeamlessTravel = true;

		//		FString MatchType = SubSystem->DesiredMatchType;
		//		if (MatchType == TEXT("FreeForAll"))
		//		{
		//			//GetWorld()->ServerTravel(TEXT("/Game/A_Blaster/Maps/GameMap_Tokyo?listen"));

		//			FTimerHandle H;
		//			GetWorldTimerManager().SetTimer(H, FTimerDelegate::CreateLambda([&]()
		//				{
		//					GetWorld()->ServerTravel(TEXT("/Game/A_Blaster/Maps/Lobby_Test?listen"));

		//				}), 10.f, false);

		//			

		//		}
		//		else if (MatchType == TEXT("Teams"))
		//		{
		//			GetWorld()->ServerTravel(TEXT("/Game/A_Blaster/Maps/GameMap_Tokyo?listen"));

		//		}
		//		else if (MatchType == TEXT("CaptureTheFlag"))
		//		{
		//			GetWorld()->ServerTravel(TEXT("/Game/A_Blaster/Maps/GameMap_Tokyo?listen"));

		//		}
		//	}
		//}
	}


}
