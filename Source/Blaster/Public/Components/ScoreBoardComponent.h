// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Components/WidgetComponent.h"
#include "HUD/MyWidgetComponent.h"
#include "HUD/ScoreBoard/ScoreBoardText.h"
#include "Interfaces/TeamInterface.h"
#include "ScoreBoardComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UScoreBoardComponent : public UMyWidgetComponent
{
	GENERATED_BODY()

public:	
	UScoreBoardComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual void PostLoad() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE class UScoreBoard* GetScoreBoard() const { return WBP_ScoreBoard; }
	//TArray<UScoreBoardText*> GetScoreBoardTexts() const;

	FORCEINLINE TSubclassOf<class UScoreBoard> GetScoreBoardClassToInit() const { return WBP_ScoreBoardClass; }
	FORCEINLINE void SetScoreBoard(class UScoreBoard* InScoreBoard) { WBP_ScoreBoard = InScoreBoard; }
	

public:

	UFUNCTION(Client, Reliable)
	void ClientAddScoreBoard(ETeam InTeam, const FString& InPlayerName, const FString& OwnerName);

	void RemoveScoreBoard(const FString& InPlayerName);

	void ShowScoreBoard();
	void ReleaseScoreBoard();



	UFUNCTION(Client, Reliable)
	void ClientChangeScore(const FString& InPlayerName, int32 InValue, bool IsScore);

	UFUNCTION(Client, Reliable)
	void ClientRemoveScoreText(const FString& InPlayerName);

public:


	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<class ABlasterPlayerController> OwingController;

	UPROPERTY()
	TSubclassOf<class UScoreBoard> WBP_ScoreBoardClass;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UScoreBoard> WBP_ScoreBoard;


	void SetAllControllerScoreBoard();

	uint8 bWorldPlayersCountChanged : 1 = true;

	TSet<APlayerController*> CheckPC;

	FTimerHandle InitTimer;

};
