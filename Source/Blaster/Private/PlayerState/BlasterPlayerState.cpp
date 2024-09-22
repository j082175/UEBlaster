// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/BlasterPlayerState.h"
#include "Characters/BlasterCharacter.h"
#include "PlayerController/BlasterPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "HUD/OverheadWidget.h"
#include "Blaster.h"
#include "HUD/OverheadWidgetComponent.h"
#include "GameState/BlasterGameState.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScoreBoardComponent.h"
#include "GameMode/TeamsGameMode.h"

#include "HUD/ScoreBoard/ScoreBoard.h"
#include "HUD/ScoreBoard/ScoreBoardText.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "EngineUtils.h"

ABlasterPlayerState::ABlasterPlayerState()
{
	bReplicates = true;
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
}

void ABlasterPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("BlasterPlayerState tick"));
}

void ABlasterPlayerState::BeginPlay()
{
	AB_LOG(LogABBeginPlay, Warning, TEXT("%s"), TEXT("Begin"));
	Super::BeginPlay();
	AB_LOG(LogABBeginPlay, Warning, TEXT("%s"), TEXT("End"));

	//UE_LOG(LogTemp, Warning, TEXT("ABlasterPlayerState::BeginPlay"));

	Controller = Cast<ABlasterPlayerController>(GetOwner());
}

void ABlasterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Defeats);
	DOREPLIFETIME(ThisClass, Team);

}

void ABlasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	SetHUDScore(GetScore());
}

ETeam ABlasterPlayerState::IGetTeam() const
{
	return Team;
}

void ABlasterPlayerState::ISetTeam(ETeam TeamToSlot)
{
	if (TeamToSlot == ETeam::NoTeam)
	{
		ensure(false);
	}

	//UE_LOG(LogTemp, Error, TEXT("TeamToSlot : %d"), (int)TeamToSlot);
	Team = TeamToSlot;
	GetWorldTimerManager().SetTimer(InitializeTimerHandle, this, &ThisClass::SetTeamFunc, 0.3f, true);
}

void ABlasterPlayerState::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);
	//Score += ScoreAmount;

	SetHUDScore(GetScore());
}

void ABlasterPlayerState::OnRep_Defeats()
{
	SetDefeatsScore(Defeats);
}

void ABlasterPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;
	SetDefeatsScore(Defeats);
}



void ABlasterPlayerState::SetHUDScore(int32 InScore)
{
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->GetController()) : Controller;
		if (Controller)
		{
			//Controller->SetHUDScore(InScore);
			Controller->OnScoreChanged.Broadcast(InScore);

			if (HasAuthority() && GetWorld()->IsValidLowLevel())
			{
				if (ATeamsGameMode* GM = GetWorld()->GetAuthGameMode<ATeamsGameMode>())
				{
					GM->ChangeScoreBoard(GetPlayerName(), InScore, true);
				}
			}

		}
	}

	
}

void ABlasterPlayerState::SetDefeatsScore(int32 DefeatsAmount)
{
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->GetController()) : Controller;
		if (Controller)
		{
			//Controller->SetHUDDefeats(Defeats);
			Controller->OnDefeatsChanged.Broadcast(DefeatsAmount);


			if (HasAuthority() && GetWorld()->IsValidLowLevel())
			{
				if (ATeamsGameMode* GM = GetWorld()->GetAuthGameMode<ATeamsGameMode>())
				{
					GM->ChangeScoreBoard(GetPlayerName(), DefeatsAmount, false);
				}
			}

		}
	}
}

void ABlasterPlayerState::OnRep_Team()
{
	GetWorldTimerManager().SetTimer(InitializeTimerHandle, this, &ThisClass::SetTeamFunc, 0.3f, true);
}

void ABlasterPlayerState::SetTeamFunc()
{
	ITeamInterface* BCharacter = Cast<ITeamInterface>(GetPawn());
	if (BCharacter)
	{
		BCharacter->ISetTeam(Team);
		GetWorldTimerManager().ClearTimer(InitializeTimerHandle);
		InitializeTimerHandle.Invalidate();
	}
	else
	{
		AB_LOG(LogABDisplay, Log, TEXT("Initializing"));
	}

}



//void ABlasterPlayerState::RemoveFromScoreBoard(ABlasterPlayerState* RemoveTarget)
//{
//	if (PlayerCheck.Contains(RemoveTarget))
//	{
//		ClientRemoveFromScoreBoard(RemoveTarget);
//		PlayerCheck.Remove(RemoveTarget);
//	}
//}


//void ABlasterPlayerState::ClientRemoveFromScoreBoard_Implementation(ABlasterPlayerState* RemoveTarget)
//{
//	//ABlasterPlayerController* MyController = Cast<ABlasterPlayerController>(GetOwningController());
//	UScoreBoardComponent* ScoreBoard = GetOwningController()->GetComponentByClass<UScoreBoardComponent>();
//	ScoreBoard->RemoveScoreBoard(RemoveTarget->GetPlayerName());
//}
