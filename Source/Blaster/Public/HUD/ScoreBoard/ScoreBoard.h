// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/MyUserWidget.h"
//#include "Blueprint/UserWidget.h"
#include "HUD/ScoreBoard/ScoreBoardText.h"
#include "ScoreBoard.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UScoreBoard : public UMyUserWidget
{
	GENERATED_BODY()
	
public:
	UScoreBoard(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void NativeConstruct() override;

	void SetBlueTeamScore(int32 InCurrentScore);
	void SetRedTeamScore(int32 InCurrentScore);

	void AddBlueTeam(const FScoreBoardTextStruct& InScoreBoardTextStruct, const FString& OwnerName);
	void AddRedTeam(const FScoreBoardTextStruct& InScoreBoardTextStruct, const FString& OwnerName);

	void RemoveTeam(const FString& InPlayerName);


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTeamScore> WBP_TeamScore;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> BlueTeamScoreBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> RedTeamScoreBox;

	UPROPERTY(VisibleAnywhere)
	TMap<FString, class UScoreBoardText*> ScoreBoardTextMap;

	//UFUNCTION()
	//void OnRep_ScoreBoardTextArr();
	//UPROPERTY(ReplicatedUsing = OnRep_ScoreBoardTextArr, VisibleAnywhere)
	//TArray<class UScoreBoardText*> ScoreBoardTextArr;

//private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UScoreBoardText> ScoreBoardTextClass;

	UPROPERTY()
	TObjectPtr<class UScoreBoardText> ScoreBoardText;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UScoreBoardComponent> OwingComponent;
};
