// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatViewer.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UChatViewer : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UScrollBox* ChatTextBox;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<class UChatTextBlock> ChatTextBlockClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UChatTextBlock*> ChatTextBlocks;
};
