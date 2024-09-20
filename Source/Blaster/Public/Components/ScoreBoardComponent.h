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



public:

	void InitScoreBoard();

	UFUNCTION(Client, Reliable)
	void AddScoreBoard(ETeam InTeam, const FScoreBoardTextStruct& InStruct, APlayerController* InPC);
	void RemoveScoreBoard(const FString& InPlayerName);

	void ShowScoreBoard();
	void ReleaseScoreBoard();
private:


	UPROPERTY(Replicated, VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<class ABlasterPlayerController> OwingController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UScoreBoard> WBP_ScoreBoardClass;

	UFUNCTION()
	void OnRep_ScoreBoard();
	UPROPERTY(ReplicatedUsing = OnRep_ScoreBoard, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UScoreBoard> WBP_ScoreBoard;

	UFUNCTION(Server, Reliable)
	void SetAllControllerScoreBoard();


	TSet<APlayerController*> CheckPC;
};
