// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatBox.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UChatBox : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void SetVisibility(ESlateVisibility SlateVisibility);
	void OnTextCommitted(const FText& Text, const FString& PlayerName);


	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ChatInput;
	
	UPROPERTY()
	TObjectPtr<class UChatViewer> ChatViewer;

	// classes
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UChatTextBlock> ChatTextBlockClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UChatViewer> ChatViewerClass;
private:

	UPROPERTY()
	APlayerController* OwningController;

	UPROPERTY()
	class ABlasterPlayerState* BlasterPlayerState;

	FTimerHandle ChatViewerReleaseTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float ChatViewerReleaseTime = 3.f;

};
