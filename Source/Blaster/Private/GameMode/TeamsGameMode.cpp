// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/TeamsGameMode.h"
#include "GameState/BlasterGameState.h"
#include "PlayerState/BlasterPlayerState.h"
#include "Kismet/GameplayStatics.h"
//#include "Characters/BlasterCharacter.h"
#include "Characters/CharacterBase.h"


#include "PlayerState/BlasterPlayerState.h"
#include "PlayerController/BlasterPlayerController.h"
#include "GameFramework/PlayerStart.h"


ATeamsGameMode::ATeamsGameMode()
{
	//UE_LOG(LogTemp, Display, TEXT("ATeamsGameMode Constructor"));
	bTeamsMatch = true;
}

void ATeamsGameMode::PostLogin(APlayerController* PlayerController)
{
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Error, TEXT("TeamsGameMode : PostLogin's PlayerController is NULL"));
		return;
	}

	Super::PostLogin(PlayerController);

	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	if (BGameState)
	{
		ABlasterPlayerState* BPState = PlayerController->GetPlayerState<ABlasterPlayerState>();
		if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
		{
			if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
			{
				BGameState->RedTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_RedTeam);
			}
			else
			{
				BGameState->BlueTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_BlueTeam);
			}
		}
	}
}

void ATeamsGameMode::Logout(AController* Exiting)
{
	if (!IsValid(Exiting))
	{
		UE_LOG(LogTemp, Error, TEXT("TeamsGameMode : LogOut's Exiting is NULL"));
		return;
	}

	Super::Logout(Exiting);

	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	ABlasterPlayerState* BPState = Exiting->GetPlayerState<ABlasterPlayerState>();
	if (BGameState && BPState)
	{
		if (BGameState->RedTeam.Contains(BPState))
		{
			BGameState->RedTeam.Remove(BPState);
		}

		if (BGameState->BlueTeam.Contains(BPState))
		{
			BGameState->BlueTeam.Remove(BPState);
		}
	}
}

float ATeamsGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	if (!IsValid(Attacker) || !IsValid(Victim))
	{
		UE_LOG(LogTemp, Error, TEXT("TeamsGameMode : CalculateDamage's Attacker or Victim is NULL"));
		return 0.f;
	}

	ABlasterPlayerState* AttackerPState = Attacker->GetPlayerState<ABlasterPlayerState>();
	ABlasterPlayerState* VictimPState = Victim->GetPlayerState<ABlasterPlayerState>();

	if (AttackerPState == nullptr || VictimPState == nullptr) return BaseDamage;
	if (AttackerPState == VictimPState) return BaseDamage;
	if (AttackerPState->GetTeam() == VictimPState->GetTeam()) return 0.f;

	return BaseDamage;
}

void ATeamsGameMode::PlayerEliminated(ACharacterBase* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	if (!IsValid(ElimmedCharacter) || !IsValid(VictimController) || !IsValid(AttackerController))
	{
		UE_LOG(LogTemp, Error, TEXT("TeamsGameMode : PlayerEliminted not executed!"));
		return;
	}

	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);

	ABlasterGameState* BGameState = Super::GetGameState<ABlasterGameState>();
	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	if (BGameState && AttackerPlayerState)
	{
		if (AttackerPlayerState->GetTeam() == ETeam::ET_BlueTeam)
		{
			BGameState->BlueTeamScores();
		}
		else if (AttackerPlayerState->GetTeam() == ETeam::ET_RedTeam)
		{
			BGameState->RedTeamScores();
		}
	}
}

//void ATeamsGameMode::HandleMatchHasStarted()
//{
//	Super::HandleMatchHasEnded();
//	UE_LOG(LogTemp, Display, TEXT("HandleMatchHasStarted"));
//
//	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
//	if (BGameState)
//	{
//		for (const auto& PState : BGameState->PlayerArray)
//		{
//			ABlasterPlayerState* BPState = Cast<ABlasterPlayerState>(PState.Get());
//			if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
//			{
//				if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
//				{
//					BGameState->RedTeam.AddUnique(BPState);
//					BPState->SetTeam(ETeam::ET_RedTeam);
//				}
//				else
//				{
//					BGameState->BlueTeam.AddUnique(BPState);
//					BPState->SetTeam(ETeam::ET_BlueTeam);
//				}
//			}
//		}
//	}
//}
//
