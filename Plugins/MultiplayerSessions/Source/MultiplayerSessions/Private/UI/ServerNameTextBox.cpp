// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerNameTextBox.h"
#include "Components/EditableTextBox.h"

#include "UI//MainMenu_Home.h"

void UServerNameTextBox::NativeConstruct()
{
	Super::NativeConstruct();

	ServerNameTextBox->OnTextCommitted.AddUniqueDynamic(this, &ThisClass::OnChatCommittedFunc);

	FInputModeGameAndUI InputModeUIOnly;
	InputModeUIOnly.SetHideCursorDuringCapture(true);
	InputModeUIOnly.SetWidgetToFocus(ServerNameTextBox->TakeWidget());
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	ServerNameTextBox->SetFocus();
	//GetOwningPlayer()->SetShowMouseCursor(false);
	GetOwningPlayer()->SetInputMode(InputModeUIOnly);
}

FString UServerNameTextBox::GetServerNameTextBox() const
{
	return ServerNameTextBox->GetText().ToString();
}

void UServerNameTextBox::SetServerNameTextBox(const FString& InServerName)
{
	ServerNameTextBox->SetText(FText::FromString(InServerName));

}

void UServerNameTextBox::OnChatCommittedFunc(const FText& Text, ETextCommit::Type CommitMethod)
{
	//UE_LOG(LogTemp, Error, TEXT("OnChatCommittedFunc : %s"), *UEnum::GetDisplayValueAsText(CommitMethod).ToString());

	if (CommitMethod == ETextCommit::OnCleared || CommitMethod == ETextCommit::OnUserMovedFocus || Text.ToString() == TEXT(""))
	{
		UE_LOG(LogTemp, Display, TEXT("UServerNameTextBox Remove"));
		RemoveFromParent();
		return;
	}

	MainMenu_Home->CreateButtonFinished(Text.ToString());
}