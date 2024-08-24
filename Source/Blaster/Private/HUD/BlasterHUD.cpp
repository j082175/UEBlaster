// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BlasterHUD.h"
#include "GameFramework/PlayerController.h"
#include "HUD/CharacterOverlay.h"
#include "HUD/Announcement.h"
#include "HUD/ElimAnnouncement.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "HUD/MyWidgetComponent.h"
#include "Characters/CharacterBase.h"
#include "GameMode/BlasterGameMode.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/BlasterGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Characters/BlasterCharacter.h"
#include "BlasterTypes/AnnouncementTypes.h"
#include "GameState/BlasterGameState.h"
#include "PlayerState/BlasterPlayerState.h"

ABlasterHUD::ABlasterHUD()
{
	
	static ConstructorHelpers::FClassFinder<UUserWidget> CORef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/A_Blaster/Blueprints/HUD/WBP_CharacterOverlay.WBP_CharacterOverlay_C'"));
	if (CORef.Succeeded()) CharacterOverlayClass = CORef.Class;
	else UE_LOG(LogTemp, Error, TEXT("No CORef"));

	static ConstructorHelpers::FClassFinder<UUserWidget> CORef2(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/A_Blaster/Blueprints/HUD/WBP_Announcement.WBP_Announcement_C'"));
	if (CORef2.Succeeded()) AnnouncementClass = CORef2.Class;
	else UE_LOG(LogTemp, Error, TEXT("No CORef2"));


	
}

//void ABlasterHUD::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(ThisClass, MatchState);
//	DOREPLIFETIME(ThisClass, bShowTeamScores);
//}


void ABlasterHUD::DrawHUD()
{
	Super::DrawHUD();


	//if (HUDPackage.CrosshairCenter)
	//{
	//	FVector2D Spread(0.f, 0.f);
	//	DrawCrosshair(HUDPackage.CrosshairCenter, FVector2D(640.f, 480.f), Spread, HUDPackage.CrosshairsColor);
	//}

	DrawCrosshairs();
}

void ABlasterHUD::AddAnnouncement()
{
	if (Announcement)
	{
		if (!Announcement->IsInViewport()) Announcement->AddToViewport();
	}
}

void ABlasterHUD::AddElimAnnouncement(FString Attacker, FString Victim)
{
	OwingPlayer = OwingPlayer == nullptr ? GetOwningPlayerController() : OwingPlayer;
	if (OwingPlayer && ElimAnnouncementClass)
	{
		ElimAnnouncementWidget = CreateWidget<UElimAnnouncement>(OwingPlayer, ElimAnnouncementClass);
		if (ElimAnnouncementWidget.IsValid())
		{
			ElimAnnouncementWidget->SetElimAnnouncementText(Attacker, Victim);
			ElimAnnouncementWidget->AddToViewport();

			for (const auto& Msg : ElimMessages)
			{
				if (Msg && Msg->AnnouncementBox)
				{
					UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Cast<UWidget>(Msg->AnnouncementBox));
					FVector2D Position = CanvasSlot->GetPosition();
					FVector2D NewPosition(CanvasSlot->GetPosition().X, Position.Y - CanvasSlot->GetSize().Y);
					CanvasSlot->SetPosition(NewPosition);
				}
			}


			ElimMessages.Add(ElimAnnouncementWidget.Get());

			//FTimerHandle ElimMsgTimer;
			//FTimerDelegate ElimMsgDelegate;
			//ElimMsgDelegate.BindUFunction(this, TEXT("ElimAnnouncementTimerFinished"), ElimAnnouncementWidget);
			//GetWorldTimerManager().SetTimer(ElimMsgTimer, ElimMsgDelegate, ElimAnnouncementTime, false);

			FTimerHandle ElimMsgTimer;
			GetWorldTimerManager().SetTimer(ElimMsgTimer, FTimerDelegate::CreateLambda([&]()
				{
					ElimAnnouncementTimerFinished(ElimAnnouncementWidget.Get());
				}), ElimAnnouncementTime, false);
		}
	}
}

void ABlasterHUD::AddCharacterOverlay()
{
	if (CharacterOverlay)
	{
		CharacterOverlay->AddToViewport();
	}
}

void ABlasterHUD::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Display, TEXT("ABlasterHUD::BeginPlay"));


	//AddCharacterOverlay();
	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("BlasterHUD's PlayerController is NULL!"));
	}

	if (CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterOverlayClass is null"));
	}

	if (AnnouncementClass)
	{
		Announcement = CreateWidget<UAnnouncement>(PlayerController, AnnouncementClass);
	}

	//ServerCheckMatchState();

}

void ABlasterHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(GetOwningPlayerController()) : BlasterPlayerController;
	//if (BlasterPlayerController)
	//{
	//	BlasterPlayerController->SetHUDTime();
	//	BlasterPlayerController->CheckTimeSync(DeltaTime);
	//	BlasterPlayerController->CheckPing(DeltaTime);
	//}


	//if (BlasterGameMode == nullptr)
	//{
	//	BlasterGameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));
	//	if (BlasterGameMode)
	//	{
	//		ServerCheckMatchState();
	//	}
	//}
}

void ABlasterHUD::DrawCrosshairs()
{
	FVector2D ViewportSize;
	if (GEngine)
	{

		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		float SpreadScale = CrosshairSpreadMax * HUDPackage.CrosshairSpread;
		//UE_LOG(LogTemp, Display, TEXT("SpreadScale : %f"), SpreadScale);

		if (HUDPackage.CrosshairCenter)
		{
			FVector2D Spread(0.f, 0.f);
			DrawCrosshair(HUDPackage.CrosshairCenter, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}

		if (HUDPackage.CrosshairLeft)
		{
			FVector2D Spread(-SpreadScale, 0.f);
			DrawCrosshair(HUDPackage.CrosshairLeft, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}

		if (HUDPackage.CrosshairRight)
		{
			FVector2D Spread(SpreadScale, 0.f);
			DrawCrosshair(HUDPackage.CrosshairRight, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}

		if (HUDPackage.CrosshairTop)
		{
			FVector2D Spread(0.f, -SpreadScale);
			DrawCrosshair(HUDPackage.CrosshairTop, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}

		if (HUDPackage.CrosshairBottom)
		{
			FVector2D Spread(0.f, SpreadScale);
			DrawCrosshair(HUDPackage.CrosshairBottom, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No GEngine"));
	}
}

void ABlasterHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor InCrosshairsColor)
{
	//UE_LOG(LogTemp, Display, TEXT("DrawCrosshair : %x, ViewportCenter : %f, %f / Spread : %f, %f / Color : %s"), Texture, ViewportCenter.X, ViewportCenter.Y, Spread.X, Spread.Y, *InCrosshairsColor.ToString());

	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(ViewportCenter.X - (TextureWidth / 2.f) + Spread.X, ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y);

	DrawTexture(Texture, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight, 0.f, 0.f, 1.f, 1.f, InCrosshairsColor);
}

void ABlasterHUD::ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove)
{
	if (MsgToRemove)
	{
		MsgToRemove->RemoveFromParent();
	}
}


//void ABlasterHUD::SetHUDTime()
//{
//	//UE_LOG(LogTemp, Display, TEXT("GetServerTime() : %f"), GetServerTime());
//
//	float TimeLeft = 0.f;
//	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
//	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
//	else if (MatchState == MatchState::Cooldown) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime + CooldownTime;
//	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);
//
//	//if (HasAuthority() && IsLocalController())
//	//{
//	//	UE_LOG(LogTemp, Display, TEXT("WarmupTime : %f"), WarmupTime);
//
//	//}
//
//	//UE_LOG(LogTemp, Display, TEXT("CountdownInt : %d"), CountdownInt);
//	//UE_LOG(LogTemp, Display, TEXT("SecondsLeft : %d"), SecondsLeft);
//
//	//if (HasAuthority())
//	//{
//	//	BlasterGameMode = BlasterGameMode == nullptr ? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
//	//	if (BlasterGameMode)
//	//	{
//	//		SecondsLeft = FMath::CeilToInt(BlasterGameMode->GetCountdownTime() + LevelStartingTime);
//	//	}
//	//}
//
//	if (CountdownInt != SecondsLeft || HasAuthority())
//	{
//		//UE_LOG(LogTemp, Display, TEXT("MatchState : %s"), *MatchState.ToString());
//		if (MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
//		{
//			//if (GEngine)
//			//{
//			//	GEngine->AddOnScreenDebugMessage(6, 2.f, FColor::Red, FString::Printf(TEXT("MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown")));
//			//}
//			Announcement->SetHUDAnnouncementCountdown(TimeLeft);
//			//SetHUDAnnouncementCountdown(TimeLeft);
//		}
//		else if (MatchState == MatchState::InProgress)
//		{
//			//if (GEngine)
//			//{
//			//	GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Red, FString::Printf(TEXT("MatchState::InProgress\n ServerTime : %f\n TimeLeft : %f\n WarmupTime : %f\n, MatchTime : %f\n, LevelStartingTime : %f"), GetServerTime(), TimeLeft, WarmupTime, MatchTime, LevelStartingTime));
//			//}
//
//
//
//
//			CharacterOverlay->SetHUDMatchCountdown(TimeLeft);
//		}
//
//	}
//
//
//
//	CountdownInt = SecondsLeft;
//}
//
//float ABlasterHUD::GetServerTime()
//{
//	if (HasAuthority())
//	{
//		return GetWorld()->GetTimeSeconds();
//	}
//	else
//	{
//		//UE_LOG(LogTemp, Display, TEXT("ClientServerDelta : %f"), ClientServerDelta);
//		return GetWorld()->GetTimeSeconds() + ClientServerDelta;
//	}
//}
//
//void ABlasterHUD::CheckTimeSync(float DeltaTime)
//{
//	TimeSyncRunningTime += DeltaTime;
//	if (TimeSyncRunningTime > TimeSyncFrequency)
//	{
//		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
//		TimeSyncRunningTime = 0.f;
//	}
//}
//
//void ABlasterHUD::CheckPing(float DeltaTime)
//{
//	HighPingRunningTime += DeltaTime;
//	if (HighPingRunningTime > CheckPingFrequency)
//	{
//		OwingPlayer->PlayerState = OwingPlayer->PlayerState == nullptr ? OwingPlayer->GetPlayerState<APlayerState>() : OwingPlayer->PlayerState.Get();
//		if (OwingPlayer->PlayerState)
//		{
//			//UE_LOG(LogTemp, Warning, TEXT("Ping: :%f"), PlayerState->GetPingInMilliseconds());
//
//			if (OwingPlayer->PlayerState->GetPingInMilliseconds() > HighPingThreshold)
//			{
//				CharacterOverlay->HighPingWarning();
//				PingAnimationRunningTime = 0.f;
//				ServerReportPingStatus(true);
//			}
//			else
//			{
//				ServerReportPingStatus(false);
//			}
//		}
//		HighPingRunningTime = 0.f;
//	}
//	bool bHighPingAnimationPlaying = CharacterOverlay && CharacterOverlay->HighPingAnimation && CharacterOverlay->IsAnimationPlaying(CharacterOverlay->HighPingAnimation);
//	if (bHighPingAnimationPlaying)
//	{
//		PingAnimationRunningTime += DeltaTime;
//		if (PingAnimationRunningTime > HighPingDuration)
//		{
//			CharacterOverlay->StopHighPingWarning();
//		}
//	}
//}
//
//void ABlasterHUD::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
//{
//	//UE_LOG(LogTemp, Display, TEXT("ServerRequestServerTime_Implementation"));
//	float ServerTimeOfRecepit = GetWorld()->GetTimeSeconds();
//	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfRecepit);
//}
//
//void ABlasterHUD::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
//{
//	//if (HasAuthority() && IsLocalController())
//	//{
//	//	UE_LOG(LogTemp, Error, TEXT("ClientReportServerTime_Implementation"));
//	//}
//	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest; // 클라가 Request를 보내고 받는데 걸린 시간
//	SingleTripTime = 0.5f * RoundTripTime;
//	float CurrentServerTime = TimeServerReceivedClientRequest + (SingleTripTime);
//	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
//}
//
//// Is the ping too high?
//void ABlasterHUD::ServerReportPingStatus_Implementation(bool bHighPing)
//{
//	HighPingDelegate.Broadcast(bHighPing);
//}
//
//void ABlasterHUD::OnRep_MatchState()
//{
//	//UE_LOG(LogTemp, Display, TEXT("OnRep_MatchState : %d"), bTeamsMatch);
//	OnMatchStateSetFunc(true);
//}
//
//void ABlasterHUD::OnMatchStateSetFunc(bool bTeamsMatch)
//{
//	if (MatchState == MatchState::WaitingToStart)
//	{
//		//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
//		AddAnnouncement();
//	}
//	else if (MatchState == MatchState::InProgress)
//	{
//		HandleMatchHasStarted(bTeamsMatch);
//	}
//	else if (MatchState == MatchState::Cooldown)
//	{
//		HandleCooldown();
//	}
//}
//
//void ABlasterHUD::HandleMatchHasStarted(bool bTeamsMatch)
//{
//	if (HasAuthority()) bShowTeamScores = bTeamsMatch;
//	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
//	AddCharacterOverlay();
//
//	if (Announcement)
//	{
//		Announcement->SetVisibility(ESlateVisibility::Hidden);
//	}
//
//	if (!HasAuthority()) return;
//
//	if (bTeamsMatch)
//	{
//		CharacterOverlay->InitTeamScores();
//	}
//	else
//	{
//		CharacterOverlay->HideTeamScores();
//	}
//}
//
//void ABlasterHUD::HandleCooldown()
//{
//	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
//	CharacterOverlay->RemoveFromParent();
//
//	bool bHUDValid = Announcement && Announcement->AnnouncementText && Announcement->InfoText;
//	if (bHUDValid)
//	{
//		Announcement->SetVisibility(ESlateVisibility::Visible);
//		FText AnnouncementText = FText::FromString(AnnouncementTypes::NewMatchStartsIn);
//		Announcement->AnnouncementText->SetText(AnnouncementText);
//
//		ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
//		ABlasterPlayerState* BlasterPlayerState = GetOwningPlayerController()->GetPlayerState<ABlasterPlayerState>();
//		if (BlasterGameState && BlasterPlayerState)
//		{
//			TArray<ABlasterPlayerState*> TopPlayers = BlasterGameState->TopScoringPlayers;
//			FString InfoTextString = bShowTeamScores ? GetTeamsInfoText(BlasterGameState) : GetInfoText(TopPlayers);
//
//			Announcement->InfoText->SetText(FText::FromString(InfoTextString));
//		}
//
//
//	}
//
//	BlasterCharacter = BlasterCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwningPawn()) : BlasterCharacter.Get();
//	if (BlasterCharacter && BlasterCharacter->GetCombatComponent())
//	{
//		BlasterCharacter->SetDisableGameplay(true);
//		BlasterCharacter->FireButtonPressed(false);
//	}
//}
//
//void ABlasterHUD::OnRep_ShowTeamScores()
//{
//	if (bShowTeamScores)
//	{
//		CharacterOverlay->InitTeamScores();
//	}
//	else
//	{
//		CharacterOverlay->HideTeamScores();
//	}
//}
//
//FString ABlasterHUD::GetTeamsInfoText(ABlasterGameState* BlasterGameState)
//{
//	if (BlasterGameState == nullptr) return FString();
//
//	FString InfoTextString;
//
//	const int32 RedTeamScore = BlasterGameState->RedTeamScore;
//	const int32 BlueTeamScore = BlasterGameState->BlueTeamScore;
//
//	if (RedTeamScore == 0 && BlueTeamScore == 0)
//	{
//		InfoTextString = AnnouncementTypes::ThereIsNoWinner;
//	}
//	else if (RedTeamScore == BlueTeamScore)
//	{
//		InfoTextString = FString::Printf(TEXT("%s\n"), *AnnouncementTypes::TeamsTiedForTheWin);
//		InfoTextString.Append(AnnouncementTypes::RedTeam + TEXT("\n"));
//		InfoTextString.Append(AnnouncementTypes::BlueTeam + TEXT("\n"));
//	}
//	else if (RedTeamScore > BlueTeamScore)
//	{
//		InfoTextString = AnnouncementTypes::RedTeamWins + TEXT("\n");
//		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *AnnouncementTypes::RedTeam, RedTeamScore));
//		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *AnnouncementTypes::BlueTeam, BlueTeamScore));
//	}
//	else if (BlueTeamScore > RedTeamScore)
//	{
//		InfoTextString = AnnouncementTypes::BlueTeamWins + TEXT("\n");
//		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *AnnouncementTypes::BlueTeam, BlueTeamScore));
//		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *AnnouncementTypes::RedTeam, RedTeamScore));
//	}
//
//	return InfoTextString;
//}
//
//FString ABlasterHUD::GetInfoText(const TArray<class ABlasterPlayerState*>& Players)
//{
//	ABlasterPlayerState* BlasterPlayerState = GetOwningPlayerController()->GetPlayerState<ABlasterPlayerState>();
//	if (BlasterPlayerState == nullptr) return FString();
//	FString InfoTextString;
//
//	if (Players.Num() == 0)
//	{
//		InfoTextString = AnnouncementTypes::ThereIsNoWinner;
//	}
//	else if (Players.Num() == 1 && Players[0] == BlasterPlayerState)
//	{
//		InfoTextString = AnnouncementTypes::YouAreTheWinner;
//	}
//	else if (Players.Num() == 1)
//	{
//		InfoTextString = FString::Printf(TEXT("Winnter : \n%s"), *Players[0]->GetPlayerName());
//	}
//	else if (Players.Num() > 1)
//	{
//		InfoTextString = AnnouncementTypes::PlayersTiedForTheWin;
//		for (const auto& TiedPlayer : Players)
//		{
//			InfoTextString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
//		}
//	}
//
//	return InfoTextString;
//}
//
//void ABlasterHUD::ClientJoinMidgame_Implementation(FName StateOfMatch, float InWarmupTime, float InMatchTime, float StartingTime, float Cooldown)
//{
//	UE_LOG(LogTemp, Display, TEXT("ClientJoinMidgame_Implementation"));
//	if (HasAuthority())
//	{
//	}
//
//
//	//if (GEngine)
//	//{
//	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Magenta, FString::Printf(TEXT("ClientJoinMidgame_Implementation")));
//	//}
//
//	WarmupTime = InWarmupTime;
//	MatchTime = InMatchTime;
//	LevelStartingTime = StartingTime;
//	MatchState = StateOfMatch;
//	CooldownTime = Cooldown;
//	OnMatchStateSet(MatchState);
//
//	//if (HasAuthority())
//	//{
//	//	UE_LOG(LogTemp, Error, TEXT("WarmupTime : %f"), WarmupTime);
//	//}
//
//	//if (BlasterHUD && MatchState == MatchState::WaitingToStart)
//	//{
//	//	BlasterHUD->AddAnnouncement();
//	//}
//}
//
//void ABlasterHUD::ServerCheckMatchState_Implementation()
//{
//	//if (GEngine)
//	//{
//	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Magenta, FString::Printf(TEXT("ServerCheckMatchState_Implementation")));
//	//}
//
//	BlasterGameMode = BlasterGameMode == nullptr ? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
//	if (BlasterGameMode)
//	{
//		ClientJoinMidgame(BlasterGameMode->GetMatchState(), BlasterGameMode->GetWarmupTime(), BlasterGameMode->GetMatchTime(), BlasterGameMode->GetLevelStartingTime(), BlasterGameMode->GetCooldownTime());
//	}
//
//	//ABlasterGameMode* GameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));
//	//if (GameMode)
//	//{
//	//	//WarmupTime = GameMode->GetWarmupTime();
//	//	//MatchTime = GameMode->GetMatchTime();
//	//	//LevelStartingTime = GameMode->GetLevelStartingTime();
//	//	//MatchState = GameMode->GetMatchState();
//	//	//CooldownTime = GameMode->GetCooldownTime();
//	//	//ClientJoinMidgame(MatchState, WarmupTime, MatchTime, LevelStartingTime, CooldownTime);
//	//	ClientJoinMidgame(GameMode->GetMatchState(), GameMode->GetWarmupTime(), GameMode->GetMatchTime(), GameMode->GetLevelStartingTime(), GameMode->GetCooldownTime());
//
//
//	//	//if (BlasterHUD && MatchState == MatchState::WaitingToStart)
//	//	//{
//	//	//	BlasterHUD->AddAnnouncement();
//	//	//}
//	//}
//	//else
//	//{
//	//	//if (GEngine)
//	//	//{
//	//	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Recall ServerCheckMatchState because ABlasterGameMode is null")));
//	//	//}
//
//
//	//	//// 서버에서 BeginPlay 가 호출이 먼저 안될때
//	//	//FTimerHandle TimerHandle;
//	//	//GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
//	//	//	{
//	//	//		GameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));
//	//	//		if (GameMode)
//	//	//		{
//	//	//			WarmupTime = GameMode->GetWarmupTime();
//	//	//			MatchTime = GameMode->GetMatchTime();
//	//	//			LevelStartingTime = GameMode->GetLevelStartingTime();
//	//	//			MatchState = GameMode->GetMatchState();
//	//	//			CooldownTime = GameMode->GetCooldownTime();
//	//	//			//ClientJoinMidgame(MatchState, WarmupTime, MatchTime, LevelStartingTime);
//
//	//	//			//if (BlasterHUD && MatchState == MatchState::WaitingToStart)
//	//	//			//{
//	//	//			//	BlasterHUD->AddAnnouncement();
//	//	//			//}
//	//	//		}
//	//	//	}), 0.1f, true);
//
//
//	//}
//}
//
//void ABlasterHUD::OnMatchStateSet(FName State, bool bTeamsMatch)
//{
//	//UE_LOG(LogTemp, Display, TEXT("OnMatchStateSet : %s"), *State.ToString());
//	MatchState = State;
//	OnMatchStateSetFunc(bTeamsMatch);
//}