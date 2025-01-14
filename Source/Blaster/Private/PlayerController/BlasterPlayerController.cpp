// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BlasterPlayerController.h"
#include "HUD/CharacterOverlay.h"
#include "HUD/BlasterHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/AttributeComponent.h"
#include "Components/CombatComponent.h"
#include "Characters/BlasterCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameMode/BlasterGameMode.h"
#include "HUD/Announcement.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CombatComponent.h"
#include "GameState/BlasterGameState.h"
#include "PlayerState/BlasterPlayerState.h"
#include "Item/Pickable/Weapon/Weapon_Gun.h"
#include "Components/Image.h"
#include "HUD/ReturnToMainMenu.h"
#include "BlasterTypes/AnnouncementTypes.h"
// EnhancedInput
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "HUD/ChattingUI/ChatBox.h"
#include "HUD/ChattingUI/ChatTextBlock.h"
#include "HUD/OverheadWidget.h"
#include "HUD/ScoreBoard/ScoreBoard.h"
#include "HUD/ScoreBoard/ScoreBoardText.h"

#include "Components/ScrollBox.h"

#include "Components/WidgetComponent.h"
#include "Components/EditableTextBox.h"
#include "Components/ChatSystemComponent.h"
#include "Components/ScoreBoardComponent.h"
#include "HUD/OverheadWidgetComponent.h"
#include "Blaster/Blaster.h"


ABlasterPlayerController::ABlasterPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.TickInterval = 0.01f;

	//static ConstructorHelpers::FClassFinder<UUserWidget> WBP_ScoreBoardClassRef(TEXT("/Game/A_Blaster/Blueprints/HUD/ScoreBoard/WBP_ScoreBoard.WBP_ScoreBoard_C"));
	//ensure(WBP_ScoreBoardClassRef.Class);
	//WBP_ScoreBoardClass = WBP_ScoreBoardClassRef.Class;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_TabActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/A_Blaster/Inputs/IA_Tab.IA_Tab'"));
	ensure(IA_TabActionRef.Object);
	IA_Tab = IA_TabActionRef.Object;

	ChatSystemComponent = CreateDefaultSubobject<UChatSystemComponent>(TEXT("ChatSystemComponent"));
	ScoreBoardComponent = CreateDefaultSubobject<UScoreBoardComponent>(TEXT("ScoreBoardComponent"));

	Tags.Add(TEXT("Player"));

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
}

void ABlasterPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, MatchState);
	DOREPLIFETIME(ThisClass, bShowTeamScores);

}

void ABlasterPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ABlasterPlayerController::BeginPlay()
{
	AB_LOG(LogABBeginPlay, Warning, TEXT("%s"), TEXT("Begin"));
	Super::BeginPlay();
	AB_LOG(LogABBeginPlay, Warning, TEXT("%s"), TEXT("End"));

	FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);

	//UE_LOG(LogTemp, Display, TEXT("%s BeginPlay"), *GetName());

	//PrimaryActorTick.TickInterval = 0.1f;

	//if (IsLocalController())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("LocalController : %s"), *GetName());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Not LocalController : %s"), *GetName());
	//}

	//if (IsLocalPlayerController())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("LocalPlayerController : %s"), *GetName());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Not LocalPlayerController : %s"), *GetName());
	//}

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("ABlasterPlayerController::BeginPlay")));
	//}

	BlasterHUD = Cast<ABlasterHUD>(GetHUD());
	//if (BlasterHUD)
	//{
	//	BlasterHUD->AddAnnouncement();
	//}

	ServerCheckMatchState();

	if (IsLocalPlayerController())
	{
		UChatBox* ChatBox = CreateWidget<UChatBox>(this, ChatSystemComponent->GetChatBoxClassToInit());
		if (ChatBox)
		{
			ChatSystemComponent->SetChatBox(ChatBox);
			ChatSystemComponent->AddChatBox();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ChatBox is not initialized"));
		}

	}

	if (IsLocalPlayerController())
	{
		if (HitNoticeClass)
		{
			HitNotice = CreateWidget<UUserWidget>(this, HitNoticeClass);
			if (!HitNotice)
			{
				UE_LOG(LogTemp, Error, TEXT("HitNotice not initialized"));

			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("HitNotice %s, %s"), *GetName(), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString());
				HitNotice->AddToViewport();
			}
		}


	}


	//UE_LOG(LogTemp, Warning, TEXT("%s : PlayerState : %x"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString(), GetPlayerState<APlayerState>());
}

void ABlasterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CharacterOverlayReset();

	PollInit(InPawn);
}

void ABlasterPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD.Get();
	if (BlasterHUD)
	{
		BlasterHUD->CharacterOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ABlasterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BlasterCharacter = BlasterCharacter == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : BlasterCharacter.Get();


	//if (IsLocalController())
	//{
	//	HUUD = GetHUD();

	//	UE_LOG(LogTemp, Warning, TEXT("HUD : %d"), HUUD.IsValid());

	//	BlasterHUUD = BlasterHUUD.Get() == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUUD.Get();
	//	UE_LOG(LogTemp, Warning, TEXT("BlasterHUD : %d"), BlasterHUUD.IsValid());

	//	if (BlasterHUUD.IsValid())
	//	{
	//		HUD_CO = BlasterHUUD->CharacterOverlay;
	//		HUD_A = BlasterHUUD->Announcement;
	//		UE_LOG(LogTemp, Display, TEXT("HUD_CO : %d, HUD_A : %d"), HUD_CO.IsValid(), HUD_A.IsValid());
	//	}
	//}




	/* 여기도 에러뜸 */
	if (IsLocalPlayerController()) // UI에 나타나는건 로컬플레이어만 나타나면되고 나머지 프록시들은 필요없어
	{
		SetHUDTime();
		CheckTimeSync(DeltaTime);
		CheckPing(DeltaTime);
	}
	/*  */


	if (BlasterGameMode == nullptr)
	{
		BlasterGameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));
		if (BlasterGameMode)
		{
			ServerCheckMatchState();
		}
	}


	// Bind UI
	if (IsLocalController())
	{
		//CheckBindWidgetTick();

		//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
		//if (BlasterHUD && GetPawn() && !CheckWidgetDelegateIsBound)
		//{
		//	IWidgetBindDelegateInterface* WBDI = Cast<IWidgetBindDelegateInterface>(GetPawn());
		//	if (WBDI)
		//	{
		//		WBDI->IBindWidget(BlasterHUD->CharacterOverlay);
		//		UE_LOG(LogTemp, Error, TEXT("OnPossess CharacterOverlay"));
		//		CheckWidgetDelegateIsBound = true;
		//	}
		//}
	}

	//UE_LOG(LogTemp, Display, TEXT("Pitch : %f"), GetControlRotation().Pitch);


	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	//if (BlasterHUD == nullptr)
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(3, 5, FColor::Magenta, FString::Printf(TEXT("BlasterHUD is NULL : %s, IsLocalPlayerController : %d"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString(), IsLocalController()));
	//	}
	//	UE_LOG(LogTemp, Display, TEXT("BlasterHUD is NULL!!!!!"));
	//}

	//if (GEngine)
	//{
	//	if (HasAuthority() && IsLocalController())
	//		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Magenta, FString::Printf(TEXT("BlasterHUD : %x : %s, IsLocalPlayerController : %d"), BlasterHUD, *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString(), IsLocalController()));

	//	if (HasAuthority() && !IsLocalController())
	//		GEngine->AddOnScreenDebugMessage(1, 1, FColor::Magenta, FString::Printf(TEXT("BlasterHUD : %x : %s, IsLocalPlayerController : %d"), BlasterHUD, *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString(), IsLocalController()));

	//	if (!HasAuthority() && IsLocalController())
	//		GEngine->AddOnScreenDebugMessage(2, 1, FColor::Magenta, FString::Printf(TEXT("BlasterHUD : %x : %s, IsLocalPlayerController : %d"), BlasterHUD, *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString(), IsLocalController()));

	//	if (!HasAuthority() && !IsLocalController())
	//		GEngine->AddOnScreenDebugMessage(3, 1, FColor::Magenta, FString::Printf(TEXT("BlasterHUD : %x : %s, IsLocalPlayerController : %d"), BlasterHUD, *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString(), IsLocalController()));
	//}

	//if (BlasterHUD && BlasterHUD->CharacterOverlay == nullptr)
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(3, 1, FColor::Magenta, FString::Printf(TEXT("BlasterHUD->CharacterOverlay is NULL : %s"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString()));
	//	}
	//	UE_LOG(LogTemp, Display, TEXT("BlasterHUD->CharacterOverlay is NULL!!!!!"));

	//}



	//if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy) UE_LOG(LogTemp, Display, TEXT("Controller: Simulated Proxy"));





	//BlasterCharacter = BlasterCharacter == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : BlasterCharacter.Get();
	//if (BlasterCharacter && BlasterCharacter->GetCombatComponent())
	{
		//if (!bInitializedGrenades)
		//{
		//	SetHUDGrenades(BlasterCharacter->GetGrenades());

		//}
		//if (!bInitializedCarriedAmmo)
		//{
		//	SetHUDCarriedAmmo(BlasterCharacter->GetCarriedAmmo());

		//}
		//if (!bInitializedCurrentAmmo && BlasterCharacter->GetEquippedWeapon())
		//{
		//	SetHUDWeaponAmmo(BlasterCharacter->GetEquippedWeapon()->GetAmmo());

		//}

	}


	//if (PlayerState)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("Ping : %f"), PlayerState->GetPingInMilliseconds());
	//}


	//if (HasAuthority() && IsLocalPlayerController())
	//{
	//	UE_LOG(LogTemp, Display, TEXT("bInitializedCurrentAmmo : %d"), bInitializedCurrentAmmo);
	//}


	//UpdateHUDAmmo();


	//if (!bInitializedHealth) UpdateHUDHealth();
	//if (!bInitializedShield) UpdateHUDShield();

	//if (IsLocalController())
	//{
	//	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	//	if (BlasterHUD == nullptr)
	//	{
	//		UE_LOG(LogTemp, Display, TEXT("%s, IsLocalController : %d, Get BlasterHUD Failed"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString(), IsLocalController());
	//		if (GEngine)
	//		{
	//			FString str = FString::Printf(TEXT("%s, IsLocalController : %d, Get BlasterHUD Failed"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString(), IsLocalController());
	//			GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Red, str);
	//		}
	//	}

	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(2, 1, FColor::Red, FString::Printf(TEXT("Tick : %f"), DeltaTime));
	//	}
	//}

}

void ABlasterPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	CharacterOverlayReset();
}

//void ABlasterPlayerController::IBindWidget(UUserWidget* InUserWidget)
//{
//	UE_LOG(LogTemp, Warning, TEXT("ABlasterPlayerController::IBindWidget"));
//	if (BlasterCharacter && !CheckWidgetDelegateIsBound)
//	{
//		CheckWidgetDelegateIsBound = true;
//		BlasterCharacter->IBindWidget(InUserWidget);
//	}
//}


void ABlasterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent == nullptr) return;

	//InputComponent->BindAction(TEXT("Quit"), IE_Pressed, this, &ThisClass::ShowReturnToMainMenu);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		//EnhancedInputComponent->BindAction(IA_QuitAction, ETriggerEvent::Triggered, this, &ThisClass::ShowReturnToMainMenu);
		//EnhancedInputComponent->BindAction(IA_QuitAction, ETriggerEvent::Triggered, this, &ThisClass::ShowPauseMenu);

		EnhancedInputComponent->BindAction(IA_Chat, ETriggerEvent::Triggered, ChatSystemComponent.Get(), &UChatSystemComponent::ChatButtonPressed);
		EnhancedInputComponent->BindAction(IA_Tab, ETriggerEvent::Started, ScoreBoardComponent.Get(), &UScoreBoardComponent::ShowScoreBoard);
		EnhancedInputComponent->BindAction(IA_Tab, ETriggerEvent::Completed, ScoreBoardComponent.Get(), &UScoreBoardComponent::ReleaseScoreBoard);

	}
}

void ABlasterPlayerController::UpdateHUDHealth()
{

	//BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(GetController()) : BlasterPlayerController;

	// 아래걸로 설정하면 안됨. GetFirstPlayerController() => 서버의 컨트롤러를 말하는거임.
	//BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController()) : BlasterPlayerController;
	if (BlasterCharacter == nullptr) return;
	UAttributeComponent* AttributeComponent = BlasterCharacter->GetAttributeComponent();
	if (AttributeComponent == nullptr) return;

	//UE_LOG(LogTemp, Display, TEXT("UpdateHUDHealth, CurrentHp : %f"), AttributeComponent->GetCurrentHp());
	SetHUDHealth(AttributeComponent->GetCurrentHp(), AttributeComponent->GetMaxHp());
	bInitializedHealth = true;
}

void ABlasterPlayerController::UpdateHUDShield()
{

	if (BlasterCharacter == nullptr) return;
	UAttributeComponent* AttributeComponent = BlasterCharacter->GetAttributeComponent();

	if (AttributeComponent == nullptr) return;

	//UE_LOG(LogTemp, Display, TEXT("%s : UpdateHUDShield, GetCurrentShield : %f"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString(), AttributeComponent->GetCurrentShield());
	SetHUDShield(AttributeComponent->GetCurrentShield(), AttributeComponent->GetMaxShield());
	bInitializedShield = true;
}

void ABlasterPlayerController::UpdateHUDAmmo()
{
	if (BlasterCharacter == nullptr) return;

	//if (BlasterCharacter->GetEquippedWeapon())
	//{
	//	SetHUDCarriedAmmo(BlasterCharacter->GetCarriedAmmo());
	//	SetHUDWeaponAmmo(Cast<AWeapon_Gun>(BlasterCharacter->GetEquippedWeapon())->GetAmmo());
	//}

}

void ABlasterPlayerController::SetHUDHealth(float Health, float MaxHealth)
{

	////BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	//bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay && BlasterHUD->CharacterOverlay->HealthBar && BlasterHUD->CharacterOverlay->HealthText;

	//if (bHUDValid)
	//{
	//	const float HealthPercent = Health / MaxHealth;
	//	BlasterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
	//	FString HealthText = FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
	//	BlasterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	//}
}

void ABlasterPlayerController::SetHUDShield(float Shield, float MaxShield)
{
	//UE_LOG(LogTemp, Display, TEXT("SetHUDShield, LocalRole : %s"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString());

	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay;

	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->SetShieldBar(Shield, MaxShield);
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("SetHUDShield Failed!"));
		//UE_LOG(LogTemp, Error, TEXT("BlasterHUD : %x"), BlasterHUD);
		//if (BlasterHUD) UE_LOG(LogTemp, Error, TEXT("BlasterHUD->CharacterOverlay : %x"), BlasterHUD->CharacterOverlay);

	}
}

void ABlasterPlayerController::SetHUDSp(float InSp, float InMaxSp)
{

	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay;

	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->SetSpBar(InSp, InMaxSp);
	}
}

void ABlasterPlayerController::SetHUDParryGauge(float InP, float InMaxP)
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay;

	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->SetParryGaugeBar(InP, InMaxP);
	}
}

void ABlasterPlayerController::SetHUDScore(float Score)
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay && BlasterHUD->CharacterOverlay->ScoreAmount;
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		BlasterHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
}

void ABlasterPlayerController::SetHUDDefeats(int32 Defeats)
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay && BlasterHUD->CharacterOverlay->DefeatsAmount;
	if (bHUDValid)
	{
		FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
		BlasterHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(DefeatsText));
	}
}

void ABlasterPlayerController::SetHUDWeaponAmmo(int32 CurrentAmmo)
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay;
	if (bHUDValid)
	{
		//if (HasAuthority() && IsLocalPlayerController())
		//{
		//	UE_LOG(LogTemp, Display, TEXT("SetHUDWeaponAmmo : %d"), CurrentAmmo);

		//}
		//BlasterHUD->CharacterOverlay->SetHUDWeaponAmmo(CurrentAmmo);
		bInitializedCurrentAmmo = true;
	}
}

void ABlasterPlayerController::SetHUDCarriedAmmo(int32 CarriedAmmo)
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay;
	if (bHUDValid)
	{
		//BlasterHUD->CharacterOverlay->SetHUDCarriedAmmo(CarriedAmmo);
		bInitializedCarriedAmmo = true;
	}
}

void ABlasterPlayerController::SetHUDWeaponType(EWeaponName InWeaponType)
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay;
	if (bHUDValid)
	{
		//BlasterHUD->CharacterOverlay->SetHUDWeaponType(InWeaponType);
	}
}

void ABlasterPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay && BlasterHUD->CharacterOverlay->MatchCountdownText;
	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			BlasterHUD->CharacterOverlay->MatchCountdownText->SetText(FText());
			return;
		}

		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Red, FString::Printf(TEXT("SetHUDMatchCountdown : %f"), CountdownTime));
		//}

		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);
		BlasterHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));
	}
}

void ABlasterPlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->Announcement && BlasterHUD->Announcement->WarmupTime;
	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			BlasterHUD->Announcement->WarmupTime->SetText(FText());
			return;
		}

		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);
		BlasterHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));
	}
}

void ABlasterPlayerController::SetHUDGrenades(int32 Grenades)
{

	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay;
	if (bHUDValid)
	{
		//UE_LOG(LogTemp, Warning, TEXT("SetHUDGrenades"));
		//if (GEngine) GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Magenta, FString::Printf(TEXT("SetHUDGrenades")));

		//BlasterHUD->CharacterOverlay->SetHUDGrenades(Grenades);
		bInitializedGrenades = true;
	}
}

void ABlasterPlayerController::SetHUDRedTeamScore(int32 RedScore)
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay && BlasterHUD->CharacterOverlay->RedTeamScore;
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), RedScore);

		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText::FromString(ScoreText));
	}

	OnRedTeamScoreChanged.ExecuteIfBound(RedScore);
}

void ABlasterPlayerController::SetHUDBlueTeamScore(int32 BlueScore)
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay && BlasterHUD->CharacterOverlay->BlueTeamScore;
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), BlueScore);

		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText::FromString(ScoreText));
	}

	OnBlueTeamScoreChanged.ExecuteIfBound(BlueScore);
}

void ABlasterPlayerController::HideTeamScores()
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay && BlasterHUD->CharacterOverlay->RedTeamScore && BlasterHUD->CharacterOverlay->ScoreSpacerText && BlasterHUD->CharacterOverlay->BlueTeamScore;
	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText());
		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText());
		BlasterHUD->CharacterOverlay->ScoreSpacerText->SetText(FText());
	}
}

void ABlasterPlayerController::InitTeamScores()
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->CharacterOverlay && BlasterHUD->CharacterOverlay->RedTeamScore && BlasterHUD->CharacterOverlay->ScoreSpacerText && BlasterHUD->CharacterOverlay->BlueTeamScore;
	if (bHUDValid)
	{
		FString Zero(TEXT("0"));
		FString Spacer(TEXT("|"));

		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText::FromString(Zero));
		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText::FromString(Zero));
		BlasterHUD->CharacterOverlay->ScoreSpacerText->SetText(FText::FromString(Spacer));
	}
}

void ABlasterPlayerController::PollInit(APawn* InPawn)
{
	BlasterCharacter = Cast<ABlasterCharacter>(InPawn);
	if (BlasterCharacter)
	{
		UAttributeComponent* Att = BlasterCharacter->GetAttributeComponent();

		if (Att == nullptr) return;

		Att->OnHpChanged.Broadcast(Att->GetCurrentHp(), Att->GetMaxHp());
		SetHUDHealth(Att->GetCurrentHp(), Att->GetMaxHp());
		SetHUDShield(Att->GetCurrentShield(), Att->GetMaxShield());
		SetHUDSp(Att->GetCurrentSp(), Att->GetMaxSp());
		SetHUDParryGauge(0.f, Att->GetMaxParryGauge());
		//SetHUDGrenades(BlasterCharacter->GetGrenades());
		//SetHUDCarriedAmmo(BlasterCharacter->GetCarriedAmmo());
		//if (BlasterCharacter->GetEquippedWeapon())
		//{
		//	SetHUDWeaponAmmo(Cast<AWeapon_Gun>(BlasterCharacter->GetEquippedWeapon())->GetAmmo());
		//	SetHUDWeaponType(BlasterCharacter->GetEquippedWeapon()->GetWeaponName());
		//}
	}

}

void ABlasterPlayerController::SetHUDTime()
{
	//UE_LOG(LogTemp, Display, TEXT("GetServerTime() : %f"), GetServerTime());

	float TimeLeft = 0.f;
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::Cooldown) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime + CooldownTime;
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);

	//if (HasAuthority() && IsLocalController())
	//{
	//	UE_LOG(LogTemp, Display, TEXT("WarmupTime : %f"), WarmupTime);

	//}

	//UE_LOG(LogTemp, Display, TEXT("CountdownInt : %d"), CountdownInt);
	//UE_LOG(LogTemp, Display, TEXT("SecondsLeft : %d"), SecondsLeft);

	//if (HasAuthority())
	//{
	//	BlasterGameMode = BlasterGameMode == nullptr ? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
	//	if (BlasterGameMode)
	//	{
	//		SecondsLeft = FMath::CeilToInt(BlasterGameMode->GetCountdownTime() + LevelStartingTime);
	//	}
	//}

	if (CountdownInt != SecondsLeft || HasAuthority())
	{
		//UE_LOG(LogTemp, Display, TEXT("MatchState : %s"), *MatchState.ToString());
		if (MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
		{
			//if (GEngine)
			//{
			//	GEngine->AddOnScreenDebugMessage(6, 2.f, FColor::Red, FString::Printf(TEXT("MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown")));
			//}
			
			//SetHUDAnnouncementCountdown(TimeLeft);
			OnAnnouncementCountdownChanged.ExecuteIfBound(TimeLeft);
		}
		else if (MatchState == MatchState::InProgress)
		{
			//if (GEngine)
			//{
			//	GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Red, FString::Printf(TEXT("MatchState::InProgress\n ServerTime : %f\n TimeLeft : %f\n WarmupTime : %f\n, MatchTime : %f\n, LevelStartingTime : %f"), GetServerTime(), TimeLeft, WarmupTime, MatchTime, LevelStartingTime));
			//}




			//SetHUDMatchCountdown(TimeLeft);
			MatchCountdown.Broadcast(TimeLeft);
		}

	}



	CountdownInt = SecondsLeft;
}

float ABlasterPlayerController::GetServerTime()
{
	if (HasAuthority())
	{
		return GetWorld()->GetTimeSeconds();
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("ClientServerDelta : %f"), ClientServerDelta);
		return GetWorld()->GetTimeSeconds() + ClientServerDelta;
	}
}

void ABlasterPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	//UE_LOG(LogTemp, Display, TEXT("ReceivedPlayer"));

	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void ABlasterPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void ABlasterPlayerController::IBindWidget(UUserWidget* InUserWidget)
{
	if (BlasterCharacter) BlasterCharacter->IBindWidget(InUserWidget);

	//UE_LOG(LogTemp, Warning, TEXT("ABlasterPlayerController::IBindWidget Called"));

	if (!IsLocalPlayerController()) return;

	if (UCharacterOverlay* CO = Cast<UCharacterOverlay>(InUserWidget))
	{
		//UE_LOG(LogTemp, Warning, TEXT("ABlasterPlayerController::IBindWidget"));
		MatchCountdown.AddUObject(CO, &UCharacterOverlay::SetHUDMatchCountdown);
		OnPingChanged.AddUObject(CO, &UCharacterOverlay::UpdatePing);
		OnPingAnimStarted.BindUObject(CO, &UCharacterOverlay::HighPingWarning);
		OnPingAnimStopped.BindUObject(CO, &UCharacterOverlay::StopHighPingWarning);

		OnScoreChanged.AddUObject(CO, &UCharacterOverlay::SetHUDScore);
		OnDefeatsChanged.AddUObject(CO, &UCharacterOverlay::SetHUDDefeats);

		OnCharacterOverlayRemoved.BindUObject(CO, &UCharacterOverlay::RemoveFromParent);
		//OnTeamScoreChanged.BindUObject(CO, &UCharacterOverlay::)
	}
	else if (UScoreBoard* SB = Cast<UScoreBoard>(InUserWidget))
	{
		OnRedTeamScoreChanged.BindUObject(SB, &UScoreBoard::SetRedTeamScore);
		OnBlueTeamScoreChanged.BindUObject(SB, &UScoreBoard::SetBlueTeamScore);
		//ScoreBoardComponent->InitScoreBoard();

	}
	else if (UScoreBoardText* SBT = Cast<UScoreBoardText>(InUserWidget))
	{
		UE_LOG(LogTemp, Display, TEXT("UScoreBoardText"));
		//OnPingChanged.AddUObject(SBT, &UScoreBoardText::SetLatency);
		OnScoreChanged.AddUObject(SBT, &UScoreBoardText::SetScore);
		OnDefeatsChanged.AddUObject(SBT, &UScoreBoardText::SetElims);
	}
	else if (UAnnouncement* UA = Cast<UAnnouncement>(InUserWidget))
	{
		OnAnnouncementCountdownChanged.BindUObject(UA, &UAnnouncement::SetHUDAnnouncementCountdown);
		OnAnnouncementTextChanged.BindUObject(UA, &UAnnouncement::SetAnnouncementText);
		OnAnnouncementInfoTextChanged.BindUObject(UA, &UAnnouncement::SetInfoText);
		OnAnnouncementVisibilityChanged.BindUObject(UA, &UAnnouncement::SetVisibility);

	}
}

void ABlasterPlayerController::CheckPing(float DeltaTime)
{

	if (PlayerState)
	{
		OnPingChanged.Broadcast(PlayerState->GetPingInMilliseconds());
	}



	HighPingRunningTime += DeltaTime;
	if (HighPingRunningTime > CheckPingFrequency)
	{
		PlayerState = PlayerState == nullptr ? GetPlayerState<APlayerState>() : PlayerState.Get();
		if (PlayerState)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Ping: :%f"), PlayerState->GetPingInMilliseconds());


			if (PlayerState->GetPingInMilliseconds() > HighPingThreshold)
			{
				HighPingWarning();
				PingAnimationRunningTime = 0.f;
				ServerReportPingStatus(true);
			}
			else
			{
				ServerReportPingStatus(false);
			}
		}
		HighPingRunningTime = 0.f;
	}

	PingAnimationRunningTime += DeltaTime;
	if (PingAnimationRunningTime > HighPingDuration)
	{
		StopHighPingWarning();
	}
}

// Is the ping too high?
void ABlasterPlayerController::ServerReportPingStatus_Implementation(bool bHighPing)
{
	HighPingDelegate.Broadcast(bHighPing);
}

void ABlasterPlayerController::HighPingWarning()
{
	OnPingAnimStarted.ExecuteIfBound();
}

void ABlasterPlayerController::StopHighPingWarning()
{
	OnPingAnimStopped.ExecuteIfBound();
}

void ABlasterPlayerController::OnRep_ShowTeamScores()
{
	if (bShowTeamScores)
	{
		InitTeamScores();
	}
	else
	{
		HideTeamScores();
	}
}

void ABlasterPlayerController::OnMatchStateSet(FName State, bool bTeamsMatch)
{
	UE_LOG(LogTemp, Display, TEXT("OnMatchStateSet : %s"), *State.ToString());
	MatchState = State;
	OnMatchStateSetFunc(bTeamsMatch);
}

void ABlasterPlayerController::HandleMatchHasStarted(bool bTeamsMatch)
{
	if (HasAuthority()) bShowTeamScores = bTeamsMatch;
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if (BlasterHUD)
	{
		BlasterHUD->AddCharacterOverlay();

		if (BlasterHUD->Announcement)
		{
			//BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
			OnAnnouncementVisibilityChanged.ExecuteIfBound(ESlateVisibility::Hidden);
		}

		if (!HasAuthority()) return;

		if (bTeamsMatch)
		{
			InitTeamScores();
		}
		else
		{
			HideTeamScores();
		}
	}
}

void ABlasterPlayerController::HandleCooldown()
{

	//if (BlasterHUD && BlasterHUD->CharacterOverlay)
	//{
	//	BlasterHUD->CharacterOverlay->RemoveFromParent();
	//}

	if (!OnCharacterOverlayRemoved.ExecuteIfBound())
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("OnCharacterOverlayRemoved Failed")));
	}


	//BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Visible);

	if (!OnAnnouncementVisibilityChanged.ExecuteIfBound(ESlateVisibility::Visible))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("OnAnnouncementVisibilityChanged Failed")));
	}

	//FText AnnouncementText = FText::FromString(AnnouncementTypes::NewMatchStartsIn);
	//BlasterHUD->Announcement->AnnouncementText->SetText(AnnouncementText);


	//BlasterHUD->Announcement->SetAnnouncementText(AnnouncementTypes::NewMatchStartsIn);

	if (!OnAnnouncementTextChanged.ExecuteIfBound(AnnouncementTypes::NewMatchStartsIn))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("OnAnnouncementTextChanged Failed")));
	}

	ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
	if (BlasterGameState && BlasterPlayerState)
	{
		TArray<ABlasterPlayerState*> TopPlayers = BlasterGameState->TopScoringPlayers;
		FString InfoTextString = bShowTeamScores ? GetTeamsInfoText(BlasterGameState) : GetInfoText(TopPlayers);

		//BlasterHUD->Announcement->InfoText->SetText(FText::FromString(InfoTextString));
		//BlasterHUD->Announcement->SetInfoText(InfoTextString);

		if (!OnAnnouncementInfoTextChanged.ExecuteIfBound(InfoTextString))
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("OnAnnouncementInfoTextChanged Failed")));
		}
	}

	BlasterCharacter = BlasterCharacter == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : BlasterCharacter.Get();
	if (BlasterCharacter && BlasterCharacter->GetCombatComponent())
	{
		BlasterCharacter->SetDisableGameplay(true);
		BlasterCharacter->FireButtonPressed(false);
	}
}

void ABlasterPlayerController::OnMatchStateSetFunc(bool bTeamsMatch)
{
	if (MatchState == MatchState::WaitingToStart)
	{
		if (BlasterHUD) BlasterHUD->AddAnnouncement();
	}
	else if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted(bTeamsMatch);
	}
	else if (MatchState == MatchState::Cooldown)
	{
		if (BlasterHUD) BlasterHUD->AddAnnouncement();
		HandleCooldown();
	}
}

void ABlasterPlayerController::OnRep_MatchState()
{
	//UE_LOG(LogTemp, Display, TEXT("OnRep_MatchState : %d"), bTeamsMatch);
	OnMatchStateSetFunc(true);
}

void ABlasterPlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float InWarmupTime, float InMatchTime, float StartingTime, float Cooldown)
{
	if (HasAuthority())
	{
		//UE_LOG(LogTemp, Display, TEXT("ClientJoinMidgame_Implementation"));
	}


	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Magenta, FString::Printf(TEXT("ClientJoinMidgame_Implementation")));
	//}

	WarmupTime = InWarmupTime;
	MatchTime = InMatchTime;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;
	CooldownTime = Cooldown;
	OnMatchStateSet(MatchState);

	//if (HasAuthority())
	//{
	//	UE_LOG(LogTemp, Error, TEXT("WarmupTime : %f"), WarmupTime);
	//}

	//if (BlasterHUD && MatchState == MatchState::WaitingToStart)
	//{
	//	BlasterHUD->AddAnnouncement();
	//}
}

void ABlasterPlayerController::ServerCheckMatchState_Implementation()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Magenta, FString::Printf(TEXT("ServerCheckMatchState_Implementation")));
	//}

	BlasterGameMode = BlasterGameMode == nullptr ? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode.Get();
	if (BlasterGameMode)
	{

		ClientJoinMidgame(BlasterGameMode->GetMatchState(), BlasterGameMode->GetWarmupTime(), BlasterGameMode->GetMatchTime(), BlasterGameMode->GetLevelStartingTime(), BlasterGameMode->GetCooldownTime());
	}

	//ABlasterGameMode* GameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));
	//if (GameMode)
	//{
	//	//WarmupTime = GameMode->GetWarmupTime();
	//	//MatchTime = GameMode->GetMatchTime();
	//	//LevelStartingTime = GameMode->GetLevelStartingTime();
	//	//MatchState = GameMode->GetMatchState();
	//	//CooldownTime = GameMode->GetCooldownTime();
	//	//ClientJoinMidgame(MatchState, WarmupTime, MatchTime, LevelStartingTime, CooldownTime);
	//	ClientJoinMidgame(GameMode->GetMatchState(), GameMode->GetWarmupTime(), GameMode->GetMatchTime(), GameMode->GetLevelStartingTime(), GameMode->GetCooldownTime());


	//	//if (BlasterHUD && MatchState == MatchState::WaitingToStart)
	//	//{
	//	//	BlasterHUD->AddAnnouncement();
	//	//}
	//}
	//else
	//{
	//	//if (GEngine)
	//	//{
	//	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Recall ServerCheckMatchState because ABlasterGameMode is null")));
	//	//}


	//	//// 서버에서 BeginPlay 가 호출이 먼저 안될때
	//	//FTimerHandle TimerHandle;
	//	//GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
	//	//	{
	//	//		GameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));
	//	//		if (GameMode)
	//	//		{
	//	//			WarmupTime = GameMode->GetWarmupTime();
	//	//			MatchTime = GameMode->GetMatchTime();
	//	//			LevelStartingTime = GameMode->GetLevelStartingTime();
	//	//			MatchState = GameMode->GetMatchState();
	//	//			CooldownTime = GameMode->GetCooldownTime();
	//	//			//ClientJoinMidgame(MatchState, WarmupTime, MatchTime, LevelStartingTime);

	//	//			//if (BlasterHUD && MatchState == MatchState::WaitingToStart)
	//	//			//{
	//	//			//	BlasterHUD->AddAnnouncement();
	//	//			//}
	//	//		}
	//	//	}), 0.1f, true);


	//}
}

void ABlasterPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	//UE_LOG(LogTemp, Display, TEXT("ServerRequestServerTime_Implementation"));
	float ServerTimeOfRecepit = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfRecepit);
}

void ABlasterPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	//if (HasAuthority() && IsLocalController())
	//{
	//	UE_LOG(LogTemp, Error, TEXT("ClientReportServerTime_Implementation"));
	//}
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest; // 클라가 Request를 보내고 받는데 걸린 시간
	SingleTripTime = 0.5f * RoundTripTime;
	float CurrentServerTime = TimeServerReceivedClientRequest + (SingleTripTime);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

void ABlasterPlayerController::ShowReturnToMainMenu()
{
	// TODO show the Return to Main Menu widget
	//UE_LOG(LogTemp, Display, TEXT("ShowReturnToMainMenu"));

	if (ReturnToMainMenuWidget == nullptr) return;
	if (ReturnToMainMenu == nullptr)
	{
		ReturnToMainMenu = CreateWidget<UReturnToMainMenu>(this, ReturnToMainMenuWidget);
	}

	if (ReturnToMainMenu)
	{
		bReturnToMainMenuOpen = !bReturnToMainMenuOpen;
		if (bReturnToMainMenuOpen)
		{
			//ReturnToMainMenu->MenuSetup();
		}
		else
		{
			//ReturnToMainMenu->MenuTearDown();
		}
	}

}

void ABlasterPlayerController::BroadcastElim(APlayerState* Attacker, APlayerState* Victim)
{
	ClientElimAnnouncement(Attacker, Victim);
}

FString ABlasterPlayerController::GetInfoText(const TArray<class ABlasterPlayerState*>& Players)
{
	ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
	if (BlasterPlayerState == nullptr) return FString();
	FString InfoTextString;

	if (Players.Num() == 0)
	{
		InfoTextString = AnnouncementTypes::ThereIsNoWinner;
	}
	else if (Players.Num() == 1 && Players[0] == BlasterPlayerState)
	{
		InfoTextString = AnnouncementTypes::YouAreTheWinner;
	}
	else if (Players.Num() == 1)
	{
		InfoTextString = FString::Printf(TEXT("Winnter : \n%s"), *Players[0]->GetPlayerName());
	}
	else if (Players.Num() > 1)
	{
		InfoTextString = AnnouncementTypes::PlayersTiedForTheWin;
		for (const auto& TiedPlayer : Players)
		{
			InfoTextString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
		}
	}

	return InfoTextString;
}

FString ABlasterPlayerController::GetTeamsInfoText(ABlasterGameState* BlasterGameState)
{
	if (BlasterGameState == nullptr) return FString();

	FString InfoTextString;

	const int32 RedTeamScore = BlasterGameState->RedTeamScore;
	const int32 BlueTeamScore = BlasterGameState->BlueTeamScore;

	if (RedTeamScore == 0 && BlueTeamScore == 0)
	{
		InfoTextString = AnnouncementTypes::ThereIsNoWinner;
	}
	else if (RedTeamScore == BlueTeamScore)
	{
		InfoTextString = FString::Printf(TEXT("%s\n"), *AnnouncementTypes::TeamsTiedForTheWin);
		InfoTextString.Append(AnnouncementTypes::RedTeam + TEXT("\n"));
		InfoTextString.Append(AnnouncementTypes::BlueTeam + TEXT("\n"));
	}
	else if (RedTeamScore > BlueTeamScore)
	{
		InfoTextString = AnnouncementTypes::RedTeamWins + TEXT("\n");
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *AnnouncementTypes::RedTeam, RedTeamScore));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *AnnouncementTypes::BlueTeam, BlueTeamScore));
	}
	else if (BlueTeamScore > RedTeamScore)
	{
		InfoTextString = AnnouncementTypes::BlueTeamWins + TEXT("\n");
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *AnnouncementTypes::BlueTeam, BlueTeamScore));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *AnnouncementTypes::RedTeam, RedTeamScore));
	}

	return InfoTextString;
}

void ABlasterPlayerController::PlayHitNoticeAnim(const FString& InPrefix)
{
	WidgetAnimHelper::FindWidgetAnimationName(HitNotice, FindWidgetAnimation);
	WidgetAnimHelper::StartAnimation(InPrefix, TEXT("HitNotice"), -1, 1.f, HitNotice, FindWidgetAnimation);
}

void ABlasterPlayerController::CharacterOverlayReset()
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD.Get();
	if (BlasterHUD && BlasterHUD->CharacterOverlay)
	{
		BlasterHUD->CharacterOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABlasterPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (UOverheadWidgetComponent* OWC = GetComponentByClass<UOverheadWidgetComponent>())
	{
		//Cast<UOverheadWidget>(OWC->GetWidget())->ShowPlayerName(GetPawn());
	}
}

void ABlasterPlayerController::ClientElimAnnouncement_Implementation(APlayerState* Attacker, APlayerState* Victim)
{
	APlayerState* Self = GetPlayerState<APlayerState>();

	//UE_LOG(LogTemp, Display, TEXT("Attacker : %s, Victim : %s"), *Attacker->GetPlayerName(), *Victim->GetPlayerName());

	if (Attacker && Victim && Self)
	{
		//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
		if (BlasterHUD)
		{
			if (Attacker == Self && Victim != Self)
			{
				BlasterHUD->AddElimAnnouncement(TEXT("You"), Victim->GetPlayerName());
				return;
			}
			else if (Attacker != Self && Victim == Self)
			{
				BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), TEXT("You"));
				return;
			}
			else if (Attacker == Victim && Attacker == Self)
			{
				BlasterHUD->AddElimAnnouncement(TEXT("You"), TEXT("You"));
				return;
			}
			else if (Attacker == Victim && Attacker != Self)
			{
				BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), TEXT("Themselves"));
				return;
			}
			BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), Victim->GetPlayerName());
		}
	}

}
