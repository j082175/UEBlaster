// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerNameTextBox.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UServerNameTextBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	FString GetServerNameTextBox() const;
	void SetServerNameTextBox(const FString& InServerName);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableTextBox> ServerNameTextBox;

	TWeakObjectPtr<class UMainMenu_Home> MainMenu_Home;

private:
	UFUNCTION()
	void OnChatCommittedFunc(const FText& Text, ETextCommit::Type CommitMethod);

	TWeakObjectPtr<class UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;
};
