// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ChattingUI/ChatBox.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "HUD/ChattingUI/ChatTextBlock.h"
#include "Kismet/KismetInputLibrary.h"
#include "HUD/ChattingUI/ChatViewer.h"
#include "PlayerState/BlasterPlayerState.h"
#include "Components/ChatSystemComponent.h"


void UChatBox::NativeConstruct()
{
	Super::NativeConstruct();

	//UE_LOG(LogTemp, Display, TEXT("%s : NativeConstruct, owner : %s, %s"), *GetName(), *GetOwningPlayer()->GetName(), *UEnum::GetDisplayValueAsText(GetOwningPlayer()->GetLocalRole()).ToString());

	//bIsFocusable = true;
	//SetKeyboardFocus();
	SetCursor(EMouseCursor::None);
	 
	if (ChatViewerClass) ChatViewer = CreateWidget<UChatViewer>(this, ChatViewerClass);
	if (!ChatViewer)
	{
		UE_LOG(LogTemp, Error, TEXT("ChatViewer is not initialized!"));
	}

}

void UChatBox::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	 
	//UE_LOG(LogTemp, Warning, TEXT("Chatbox tick"));
}

FReply UChatBox::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	 
	// Not Work

	//UE_LOG(LogTemp, Display, TEXT("NativeOnKeyDown"));

	//if (UKismetInputLibrary::EqualEqual_KeyKey(InKeyEvent.GetKey(), EKeys::Tab))
	//{
	//    ChatTextBox->SetVisibility(ESlateVisibility::Collapsed);
	//}

	//return FReply::Handled();
}

FReply UChatBox::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Result = Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);

	//GetOwningPlayer()->GetComponentByClass<

	if (UKismetInputLibrary::EqualEqual_KeyKey(InKeyEvent.GetKey(), EKeys::Enter))
	{
		//UE_LOG(LogTemp, Display, TEXT("Tap Pressed"));

		Super::SetVisibility(ESlateVisibility::Collapsed);
		FInputModeGameOnly InputModeGameOnly;
		if (GetOwningPlayer()) GetOwningPlayer()->SetInputMode(InputModeGameOnly);

		SwitchOnChatViewer();


		//Result = FReply::Handled().ClearUserFocus();
	}

	return Result;
}

void UChatBox::SetVisibility(ESlateVisibility SlateVisibility)
{
	if (SlateVisibility == ESlateVisibility::Visible)
	{
		ChatViewer->SetVisibility(SlateVisibility);
		GetWorld()->GetTimerManager().ClearTimer(ChatViewerReleaseTimerHandle);
		ChatViewerReleaseTimerHandle.Invalidate();
	}

	Super::SetVisibility(SlateVisibility);
}

void UChatBox::OnTextCommitted(const FText& Text, const FString& PlayerName)
{
	OwningController = OwningController == nullptr ? GetOwningPlayer() : OwningController;

	//if (ChatTextBlockClass && OwningController)
	//{
	//    BlasterPlayerState = BlasterPlayerState == nullptr ? OwningController->GetPlayerState<ABlasterPlayerState>() : BlasterPlayerState;
	//    if (BlasterPlayerState)
	//    {
	//        int32 LastIndex = ChatTextBlocks.Num();

	//        UChatTextBlock* ChatTextBlock = CreateWidget<UChatTextBlock>(GetOwningPlayer(), ChatTextBlockClass);
	//        if (ChatTextBlock && ChatTextBlock->ChatTextSlot)
	//        {
	//            UE_LOG(LogTemp, Display, TEXT("Text : %s"), *Text.ToString());

	//            const FString TextToDisplay = FString::Printf(TEXT("%s: %s"), *PlayerName, *Text.ToString());
	//            ChatTextBlock->ChatTextSlot->SetText(FText::FromString(TextToDisplay));
	//            ChatTextBlocks.Add(ChatTextBlock);
	//            ChatTextBox->AddChild(ChatTextBlock);
	//        }
	//    }
	//}
	//ChatInput->SetText(FText::FromString(""));

	if (ChatTextBlockClass && OwningController)
	{
		BlasterPlayerState = BlasterPlayerState == nullptr ? OwningController->GetPlayerState<ABlasterPlayerState>() : BlasterPlayerState;
		if (BlasterPlayerState && ChatViewer)
		{
			if (!ChatViewer->IsInViewport()) ChatViewer->AddToViewport();


			SwitchOnChatViewer();


			int32 LastIndex = ChatViewer->ChatTextBlocks.Num();

			UChatTextBlock* ChatTextBlock = CreateWidget<UChatTextBlock>(GetOwningPlayer(), ChatTextBlockClass);
			if (ChatTextBlock && ChatTextBlock->ChatTextSlot)
			{
				UE_LOG(LogTemp, Display, TEXT("Text : %s"), *Text.ToString());

				const FString TextToDisplay = FString::Printf(TEXT("%s: %s"), *PlayerName, *Text.ToString());
				ChatTextBlock->ChatTextSlot->SetText(FText::FromString(TextToDisplay));
				ChatViewer->ChatTextBlocks.Add(ChatTextBlock);
				ChatViewer->ChatTextBox->AddChild(ChatTextBlock);
			}
		}
	}
	ChatInput->SetText(FText::FromString(""));
}

void UChatBox::SwitchOnChatViewer()
{
	ChatViewer->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().ClearTimer(ChatViewerReleaseTimerHandle);
	ChatViewerReleaseTimerHandle.Invalidate();

	if (!ChatViewerReleaseTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(ChatViewerReleaseTimerHandle, this, &ThisClass::ChatViewerReleaseFunc, ChatViewerReleaseTime, false);
	}
}

void UChatBox::ChatViewerReleaseFunc()
{
	//UE_LOG(LogTemp, Display, TEXT("Chatbox Disabled"));
	ChatViewer->SetVisibility(ESlateVisibility::Collapsed);
	FInputModeGameOnly InputModeGameOnly;
	if (GetOwningPlayer()) GetOwningPlayer()->SetInputMode(InputModeGameOnly);
	ChatViewerReleaseTimerHandle.Invalidate();
}
