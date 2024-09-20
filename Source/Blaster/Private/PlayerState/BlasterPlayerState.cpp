// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/BlasterPlayerState.h"
#include "Characters/BlasterCharacter.h"
#include "PlayerController/BlasterPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "HUD/OverheadWidget.h"
#include "Blaster.h"
#include "HUD/OverheadWidgetComponent.h"
#include "GameState/BlasterGameState.h"

#include "Components/ScoreBoardComponent.h"

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
		}
	}
}

void ABlasterPlayerState::OnRep_Team()
{
	ABlasterCharacter* BCharacter = Cast<ABlasterCharacter>(GetPawn());
	if (BCharacter)
	{
		BCharacter->SetTeamColor(Team);
	}
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

void ABlasterPlayerState::Search(const TArray<ABlasterPlayerState*>& Players)
{


	//for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ABlasterPlayerController* MyController = Cast<ABlasterPlayerController>(GetOwningController());

		//BlasterGameState = GetWorld()->GetGameState<ABlasterGameState>();


		for (size_t i = 0; i < Players.Num(); i++)
		{
			//if (Players[i] == this)
			//{
			//	continue;
			//}
			FScoreBoardTextStruct S;
			S.PlayerName = FText::FromString(Players[i]->GetPlayerName());
			S.Score = Players[i]->GetScore();
			S.Elims = Players[i]->GetDefeats();
			S.Latency = Players[i]->GetPingInMilliseconds();

			if (!PlayerCheck.Contains(Players[i]))
			{
				PlayerCheck.Add(Players[i]);
				ClientSearch(S, Players[i]->IGetTeam());

				//UScoreBoardComponent* ScoreBoard = GetOwningController()->GetComponentByClass<UScoreBoardComponent>();
				//ScoreBoard->AddScoreBoard(Players[i]->IGetTeam(), S);
			}
		}


		//for (size_t i = 0; i < RedTeam.Num(); i++)
		//{
		//	//ABlasterPlayerController* SubP = Cast<ABlasterPlayerController>(RedTeam[i]->GetOwningController());
		//	if (RedTeam[i] == nullptr) continue;

		//	AB_LOG(LogABDisplay, Warning, TEXT("%s : AddRed"), *MyController->GetName());

		//	FScoreBoardTextStruct S;
		//	S.PlayerName = FText::FromString(RedTeam[i]->GetPlayerName());
		//	S.Score = RedTeam[i]->GetScore();
		//	S.Elims = RedTeam[i]->GetDefeats();
		//	S.Latency = RedTeam[i]->GetPingInMilliseconds();

		//	if (!PlayerCheck.Contains(RedTeam[i]->GetPlayerName()))
		//	{
		//		MyController->AddScoreBoard(ETeam::RedTeam, S);
		//		PlayerCheck.Add(RedTeam[i]->GetPlayerName());
		//	}

		//	ClientSearch(InController, S, ETeam::RedTeam);

		//}

		//for (size_t i = 0; i < BlueTeam.Num(); i++)
		//{
		//	if (BlueTeam[i] == nullptr) continue;

		//	//ABlasterPlayerController* SubP = Cast<ABlasterPlayerController>(BlueTeam[i]->GetOwningController());

		//	AB_LOG(LogABDisplay, Warning, TEXT("%s : AddBlue"), *MyController->GetName());

		//	FScoreBoardTextStruct S;
		//	S.PlayerName = FText::FromString(BlueTeam[i]->GetPlayerName());
		//	S.Score = BlueTeam[i]->GetScore();
		//	S.Elims = BlueTeam[i]->GetDefeats();
		//	S.Latency = BlueTeam[i]->GetPingInMilliseconds();

		//	if (!PlayerCheck.Contains(BlueTeam[i]->GetPlayerName()))
		//	{
		//		MyController->AddScoreBoard(ETeam::BlueTeam, S);
		//		PlayerCheck.Add(BlueTeam[i]->GetPlayerName());
		//	}

		//	ClientSearch(InController, S, ETeam::BlueTeam);
		//}
	}

}




void ABlasterPlayerState::ClientSearch_Implementation(const FScoreBoardTextStruct& Struct, ETeam InTeam)
{
	ABlasterPlayerController* MyController = Cast<ABlasterPlayerController>(GetOwningController());
	//UScoreBoardComponent* ScoreBoard = GetOwningController()->GetComponentByClass<UScoreBoardComponent>();
	//MyController->AddScoreBoard(InTeam, Struct);
}

void ABlasterPlayerState::RemoveFromScoreBoard(ABlasterPlayerState* RemoveTarget)
{
	if (PlayerCheck.Contains(RemoveTarget))
	{
		ClientRemoveFromScoreBoard(RemoveTarget);
		PlayerCheck.Remove(RemoveTarget);
	}
}


void ABlasterPlayerState::ClientRemoveFromScoreBoard_Implementation(ABlasterPlayerState* RemoveTarget)
{
	//ABlasterPlayerController* MyController = Cast<ABlasterPlayerController>(GetOwningController());
	UScoreBoardComponent* ScoreBoard = GetOwningController()->GetComponentByClass<UScoreBoardComponent>();
	ScoreBoard->RemoveScoreBoard(RemoveTarget->GetPlayerName());
}
