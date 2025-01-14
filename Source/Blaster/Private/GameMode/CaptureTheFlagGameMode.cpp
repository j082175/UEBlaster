// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CaptureTheFlagGameMode.h"
#include "Item/Pickable/Weapon/Flag.h"
#include "CaptureTheFlag/FlagZone.h"
#include "GameState/BlasterGameState.h"

void ACaptureTheFlagGameMode::PostLogin(APlayerController* PlayerController)
{
	Super::PostLogin(PlayerController);

	//GetWorld()->ServerTravel(TEXT("/Game/A_Blaster/Maps/Lobby_Test?listen"));

}

void ACaptureTheFlagGameMode::PlayerEliminated(ACharacterBase* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	ABlasterGameMode::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);


}

void ACaptureTheFlagGameMode::FlagCaptured(AFlag* Flag, AFlagZone* Zone)
{
	bool bValidCapture = Flag->IGetTeam() != Zone->Team;
	ABlasterGameState* BGameState = GetGameState<ABlasterGameState>();
	if (BGameState)
	{
		if (Zone->Team == ETeam::BlueTeam)
		{
			BGameState->BlueTeamScores();
		}

		if (Zone->Team == ETeam::RedTeam)
		{
			BGameState->RedTeamScores();
		}
	}
}
