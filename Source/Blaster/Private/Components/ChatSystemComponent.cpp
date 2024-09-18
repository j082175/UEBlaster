// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ChatSystemComponent.h"
#include "HUD/ChattingUI/ChatBox.h"
#include "Components/EditableTextBox.h"

#include "GameMode/BlasterGameMode.h"
#include "PlayerState/BlasterPlayerState.h"

// Sets default values for this component's properties
UChatSystemComponent::UChatSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


// Called when the game starts
void UChatSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	OwingPlayerController = Cast<APlayerController>(GetOwner());
	if (!OwingPlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("ChatSystemComponent : Owner is null!"));
	}

}


// Called every frame
void UChatSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Display, TEXT("ChatSystemComponent Tick"));
	// ...
}

void UChatSystemComponent::AddChatBox()
{
	//UE_LOG(LogTemp, Display, TEXT("AddChatBox"));
	if (ChatBox)
	{
		//UE_LOG(LogTemp, Display, TEXT("ChatBox visibility : %s"), *UEnum::GetDisplayValueAsText(ChatBox->GetVisibility()).ToString());

		//ChatBox = OwingPlayerController->CreateWidget<UChatBox>(this, ChatBoxClass);
		if (ChatBox && ChatBox->ChatInput)
		{
			ChatBox->AddToViewport();
			ChatBox->ChatInput->SetHintText(FText(FText::FromString("-")));
			ChatBox->SetVisibility(ESlateVisibility::Collapsed);
			ChatBox->ChatInput->SetRevertTextOnEscape(true);
			ChatBox->ChatInput->OnTextCommitted.AddUniqueDynamic(this, &UChatSystemComponent::OnChatCommittedFunc);
		}
	}
}

void UChatSystemComponent::OpenChatBox()
{
	//UE_LOG(LogTemp, Display, TEXT("ChatBox Open"));


	if (ChatBox && ChatBox->ChatInput)
	{
		//UE_LOG(LogTemp, Display, TEXT("ChatBox visibility : %s"), *UEnum::GetDisplayValueAsText(ChatBox->GetVisibility()).ToString());

		if (ChatBox->GetVisibility() == ESlateVisibility::Collapsed)
		{
			//UE_LOG(LogTemp, Display, TEXT("OpenChatBox : %s"), *UEnum::GetDisplayValueAsText(GetOwner()->GetLocalRole()).ToString());

			ChatBox->SetVisibility(ESlateVisibility::Visible);
			FInputModeGameAndUI InputModeGameAndUI;

			InputModeGameAndUI.SetWidgetToFocus(ChatBox->ChatInput->TakeWidget());
			InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			//InputModeGameAndUI.SetHideCursorDuringCapture(true);

			OwingPlayerController->SetShowMouseCursor(false);
			OwingPlayerController->SetInputMode(InputModeGameAndUI);
		}
		//else if (ChatBox->GetVisibility() == ESlateVisibility::Visible)
		//{
		//	UE_LOG(LogTemp, Display, TEXT("ChatBox Close"));
		//	ChatBox->SetVisibility(ESlateVisibility::Collapsed);
		//	FInputModeGameOnly InputModeGameOnly;
		//	OwingPlayerController->SetInputMode(InputModeGameOnly);
		//	//SetShowMouseCursor(false);
		//}
	}
}

void UChatSystemComponent::OnChatCommittedFunc(const FText& Text, ETextCommit::Type CommitMethod)
{
	//UE_LOG(LogTemp, Error, TEXT("OnChatCommittedFunc : %s"), *UEnum::GetDisplayValueAsText(CommitMethod).ToString());

	if (CommitMethod == ETextCommit::OnCleared || CommitMethod == ETextCommit::OnUserMovedFocus)
	{
		//OpenChatBox();
		return;
	}

	if (Text.ToString() == TEXT(""))
	{
		return;
	}

	FString PlayerName{ "" };

	APlayerState* TempPlayerState = OwingPlayerController->GetPlayerState<APlayerState>();
	if (TempPlayerState)
	{
		PlayerName = TempPlayerState->GetPlayerName();
	}

	ServerChatCommitted(Text, PlayerName); //TODO: Add a delay so that some absolute cucumber can't spam the chat.


}

void UChatSystemComponent::ChatButtonPressed()
{
	UE_LOG(LogTemp, Display, TEXT("ChatButtonPressed"));
	OpenChatBox();
}

void UChatSystemComponent::ServerChatCommitted_Implementation(const FText& Text, const FString& PlayerName)
{
	GetWorld()->GetAuthGameMode<ABlasterGameMode>()->SendChatMsg(Text, PlayerName);
}

void UChatSystemComponent::ClientChatCommitted_Implementation(const FText& Text, const FString& PlayerName)
{
	if (ChatBox)
	{
		ChatBox->OnTextCommitted(Text, PlayerName);
	}
}
