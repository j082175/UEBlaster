// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BlasterGameMode.h"
//#include "Characters/BlasterCharacter.h"
#include "PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerState/BlasterPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "GameState/BlasterGameState.h"
#include "Characters/CharacterBase.h"
#include "Actor/Casing.h"
#include "Components/ObjectPoolComponent.h"

namespace MatchState
{
	const FName Cooldown = FName(TEXT("Cooldown"));
}

ABlasterGameMode::ABlasterGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;
	bDelayedStart = true;

	//ObjectPoolComponent = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("ObjectPoolComponent"));

	//bUseSeamlessTravel = true;

	//UE_LOG(LogTemp, Display, TEXT("ABlasterGameMode Constructor"));

	//static ConstructorHelpers::FClassFinder<APawn> BlasterCharacterRef(TEXT("/Game/A_Blaster/Blueprints/Characters/BP_BlasterCharacter.BP_BlasterCharacter_C"));
	//if (BlasterCharacterRef.Class)
	//	DefaultPawnClass = BlasterCharacterRef.Class;
}

//void ABlasterGameMode::PostLogin(APlayerController* PlayerController)
//{
//	Super::PostLogin(PlayerController);
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

void ABlasterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("MatchState : %s"), *MatchState.ToString());

	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch(); // InProgress
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime + MatchTime;
		if (CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown); // 설정시 이 클래스에서 오버라이드한 OnMatchStateSet() 함수가 자동으로 호출된다.
		}
	}
	else if (MatchState == MatchState::Cooldown)
	{
		//UE_LOG(LogTemp, Display, TEXT("CountdownTime : %f"), CountdownTime);
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime + MatchTime + CooldownTime;
		if (CountdownTime <= 0.f)
		{
			Super::RestartGame();
		}
	}

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Cyan, FString::Printf(TEXT("ABlasterGameMode %f"), DeltaTime));
	//}


}

void ABlasterGameMode::PlayerLeftGame(ABlasterPlayerState* PlayerLeaving)
{
	// TODO Call elim, passing in true for bLeftGame
	if (PlayerLeaving == nullptr) return;

	ABlasterGameState* BlasterGameState = GetGameState<ABlasterGameState>();
	if (BlasterGameState && BlasterGameState->TopScoringPlayers.Contains(PlayerLeaving))
	{
		BlasterGameState->TopScoringPlayers.Remove(PlayerLeaving);
	}

	ACharacterBase* CharacterLeaving = Cast<ACharacterBase>(PlayerLeaving->GetPawn());
	if (CharacterLeaving)
	{
		CharacterLeaving->Elim(true);
	}
}

float ABlasterGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	return BaseDamage;
}

void ABlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Display, TEXT("BlasterGameMode BeginPlay"));

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("ABlasterGameMode::BeginPlay Called")));
	//}

	LevelStartingTime = GetWorld()->GetTimeSeconds();
	//LevelStartingTime = 0.f;
	//UE_LOG(LogTemp, Warning, TEXT("LevelStartingTime : %f"), LevelStartingTime);

}

void ABlasterGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayer)
		{
			BlasterPlayer->OnMatchStateSet(MatchState, bTeamsMatch);
		}
	}
}

void ABlasterGameMode::PlayerEliminated(ACharacterBase* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	ABlasterPlayerState* VictimPlayerState = VictimController ? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;

	ABlasterGameState* BlasterGameState = GetGameState<ABlasterGameState>();

	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState && BlasterGameState)
	{

		TArray<ABlasterPlayerState*> PlayersCurrentlyInTheLead;
		for (const auto& LeadPlayer : BlasterGameState->TopScoringPlayers)
		{
			PlayersCurrentlyInTheLead.Add(LeadPlayer);
		}


		AttackerPlayerState->AddToScore(1.f);
		BlasterGameState->UpdateTopScore(AttackerPlayerState);

		if (BlasterGameState->TopScoringPlayers.Contains(AttackerPlayerState))
		{
			ACharacterBase* Leader = Cast<ACharacterBase>(AttackerPlayerState->GetPawn());
			if (Leader)
			{
				Leader->MulticastGainedTheLead();
			}
		}

		for (int32 i = 0; i < PlayersCurrentlyInTheLead.Num(); i++)
		{
			if (!BlasterGameState->TopScoringPlayers.Contains(PlayersCurrentlyInTheLead[i]))
			{
				ACharacterBase* Loser = Cast<ACharacterBase>(PlayersCurrentlyInTheLead[i]->GetPawn());
				if (Loser)
				{
					Loser->MulticastLostTheLead();
				}
			}
		}
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}

	//UE_LOG(LogTemp, Display, TEXT("PlayerEliminated"));
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim(false);
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayer && AttackerPlayerState && VictimPlayerState)
		{
			BlasterPlayer->BroadcastElim(AttackerPlayerState, VictimPlayerState);
		}
	}
}

void ABlasterGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}

	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);

		UE_LOG(LogTemp, Display, TEXT("Dead"));
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}

void ABlasterGameMode::SendChatMsg(const FText& Text, const FString& PlayerName)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* TempBlasterPlayerController = Cast<ABlasterPlayerController>(*It);
		if (TempBlasterPlayerController)
		{
			TempBlasterPlayerController->ClientChatCommitted(Text, PlayerName);
		}
	}
}
