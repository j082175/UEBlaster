// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/BlasterPlayerState.h"
#include "Characters/BlasterCharacter.h"
#include "PlayerController/BlasterPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "HUD/OverheadWidget.h"

void ABlasterPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("BlasterPlayerState tick"));
}

void ABlasterPlayerState::BeginPlay()
{
	Super::BeginPlay();

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
	UE_LOG(LogTemp, Error, TEXT("TeamToSlot : %d"), (int)TeamToSlot);
	Team = TeamToSlot;
	ABlasterCharacter* BCharacter = Cast<ABlasterCharacter>(GetPawn());
	if (BCharacter)
	{
		BCharacter->SetTeamColor(Team);
	}
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

void ABlasterPlayerState::SetHUDScore(float InScore)
{
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->GetController()) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(InScore);
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
			Controller->SetHUDDefeats(Defeats);
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
