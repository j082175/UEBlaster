// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/BlasterPlayerState.h"
#include "Characters/BlasterCharacter.h"
#include "PlayerController/BlasterPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "HUD/OverheadWidget.h"
#include "Blaster.h"
#include "Components/OverheadWidgetComponent.h"

ABlasterPlayerState::ABlasterPlayerState()
{
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

	GetWorldTimerManager().SetTimer(InitializeTimerHandle, FTimerDelegate::CreateLambda([&]()
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
		}), 0.1f, true);

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
