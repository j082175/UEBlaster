// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ScoreBoardComponent.h"
#include "HUD/ScoreBoard/ScoreBoard.h"
#include "PlayerState/BlasterPlayerState.h"
#include "PlayerController/BlasterPlayerController.h"
#include "GameState/BlasterGameState.h"
#include "GameMode/TeamsGameMode.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UScoreBoardComponent::UScoreBoardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	// ...
	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_ScoreBoardClassRef(TEXT("/Game/A_Blaster/Blueprints/HUD/ScoreBoard/WBP_ScoreBoard.WBP_ScoreBoard_C"));
	ensure(WBP_ScoreBoardClassRef.Class);
	WBP_ScoreBoardClass = WBP_ScoreBoardClassRef.Class;

	SetWidgetClass(WBP_ScoreBoardClass);
}

void UScoreBoardComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, WBP_ScoreBoard);
	DOREPLIFETIME(ThisClass, OwingController);

}

void UScoreBoardComponent::PostLoad()
{
	Super::PostLoad();
	//SetIsReplicated(true);


}


// Called when the game starts
void UScoreBoardComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	UE_LOG(LogTemp, Warning, TEXT("UScoreBoardComponent::BeginPlay, Authority : %d"), GetOwner()->HasAuthority());

	if (!GetOwner()->HasAuthority())
	{
		int a = 1;
	}

	OwingController = Cast<ABlasterPlayerController>(GetOwner());
	
	//WBP_ScoreBoard = WBP_ScoreBoard == nullptr ? CreateWidget<UScoreBoard>(this, WBP_ScoreBoardClass) : WBP_ScoreBoard.Get();
	WBP_ScoreBoard = Cast<UScoreBoard>(GetWidget());
	WBP_ScoreBoard->AddToViewport();
	WBP_ScoreBoard->SetVisibility(ESlateVisibility::Collapsed);

	/*if (!GetOwner()->HasAuthority())*/ SetAllControllerScoreBoard();
}


// Called every frame
void UScoreBoardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UScoreBoardComponent::InitScoreBoard()
{
	//UE_LOG(LogTemp, Display, TEXT("InitScoreBoard"));
	ABlasterPlayerState* BlasterPlayerState = OwingController->GetPlayerState<ABlasterPlayerState>();

	if (!BlasterPlayerState)
	{
		return;
	}

	if (ABlasterGameState* BlasterGameState = GetWorld()->GetGameState<ABlasterGameState>())
	{
		//OnRedTeamScoreChanged.ExecuteIfBound(BlasterGameState->GetRedTeamScore());
		//OnBlueTeamScoreChanged.ExecuteIfBound(BlasterGameState->GetBlueTeamScore());

		FScoreBoardTextStruct S;
		S.PlayerName = FText::FromString(BlasterPlayerState->GetPlayerName());
		S.Score = BlasterPlayerState->GetScore();
		S.Elims = BlasterPlayerState->GetDefeats();
		S.Latency = BlasterPlayerState->GetPingInMilliseconds();

		if (BlasterPlayerState && BlasterPlayerState->IGetTeam() == ETeam::RedTeam)
		{
			//AddScoreBoard(ETeam::RedTeam, S);
		}
		else if (BlasterPlayerState && BlasterPlayerState->IGetTeam() == ETeam::BlueTeam)
		{
			//AddScoreBoard(ETeam::BlueTeam, S);
		}

	}
}

void UScoreBoardComponent::AddScoreBoard_Implementation(ETeam InTeam, const FScoreBoardTextStruct& InStruct, APlayerController* InPC)
{
	if (!GetOwner()->HasAuthority())
	{
		int a = 1;
	}

	UE_LOG(LogTemp, Display, TEXT("AddScoreBoard : %s, WBP_ScoreBoard : %x, Name : %s"), *UEnum::GetDisplayValueAsText(GetOwner()->GetLocalRole()).ToString(), WBP_ScoreBoard.Get(), *GetOwner()->GetName());

	if (WBP_ScoreBoard)
	{

		if (InTeam == ETeam::RedTeam)
		{
			WBP_ScoreBoard->AddRedTeam(InStruct);

		}
		else if (InTeam == ETeam::BlueTeam)
		{
			WBP_ScoreBoard->AddBlueTeam(InStruct);
		}

		WBP_ScoreBoard->ScoreBoardText->SetOwningPlayer(InPC);
		IWidgetBindDelegateInterface* WBD = Cast<IWidgetBindDelegateInterface>(InPC);
		if (WBD) WBD->IBindWidget(WBP_ScoreBoard->ScoreBoardText);
	}


}

void UScoreBoardComponent::RemoveScoreBoard(const FString& InPlayerName)
{
	if (WBP_ScoreBoard) WBP_ScoreBoard->RemoveTeam(InPlayerName);
}

void UScoreBoardComponent::ShowScoreBoard()
{
	WBP_ScoreBoard->SetVisibility(ESlateVisibility::Visible);
}

void UScoreBoardComponent::ReleaseScoreBoard()
{
	if (WBP_ScoreBoard && WBP_ScoreBoard->IsInViewport())
	{
		WBP_ScoreBoard->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UScoreBoardComponent::OnRep_ScoreBoard()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_ScoreBoard"));
}

void UScoreBoardComponent::SetAllControllerScoreBoard_Implementation()
{
	if (!GetOwner()->HasAuthority()) return;

	UE_LOG(LogTemp, Display, TEXT("SetAllControllerScoreBoard"));

	ATeamsGameMode* TeamsGameMode = GetWorld()->GetAuthGameMode<ATeamsGameMode>();

	ABlasterPlayerState* ThisPlayerState = OwingController->GetPlayerState<ABlasterPlayerState>();

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{

		ABlasterPlayerState* BlasterPlayerState = Iterator->Get()->GetPlayerState<ABlasterPlayerState>();

		FScoreBoardTextStruct S;
		S.PlayerName = FText::FromString(BlasterPlayerState->GetPlayerName());

		//if (!CheckPC.Contains(Iterator->Get()))
		{
			AddScoreBoard(BlasterPlayerState->IGetTeam(), S, Iterator->Get());
			CheckPC.Add(Iterator->Get());
		}


		if (Iterator->Get() == OwingController)
		{
			continue;
		}

		S.PlayerName = FText::FromString(ThisPlayerState->GetPlayerName());

		Iterator->Get()->GetComponentByClass<UScoreBoardComponent>()->AddScoreBoard(ThisPlayerState->IGetTeam(), S, OwingController.Get());
	}
}