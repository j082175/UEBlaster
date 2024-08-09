// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BlasterTypes/AIState.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyAIController();

    FORCEINLINE void SetAIState(EAIState InAIState) { AIState = InAIState; }
protected:
    virtual void PostInitializeComponents() override;
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

public:
    virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;

public:
    void RunAI();
    void StopAI();

private:
    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

    UFUNCTION()
    void OnTargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);

    UFUNCTION()
    void OnTargetPerceptionForgotten(AActor* Actor);

protected:
    class IAIInterface* AIPawn;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    TObjectPtr<class UAIPerceptionComponent> AIPerceptionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
    TObjectPtr<class UBlackboardData> BB_Asset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
    TObjectPtr<class UBehaviorTree> BT_Asset;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    EAIState AIState;


    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
    TSubclassOf<UAISense> AISense_Sight;

    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
    TSubclassOf<UAISense> AISense_Hearing;

    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
    TSubclassOf<UAISense> AISense_Damage;

    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
    TSubclassOf<UAISense> AISense_Prediction;

    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
    TSubclassOf<UAISense> AISense_Team;

    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
    TSubclassOf<UAISense> AISense_Touch;



    // For SetFocus
    FRotator SmoothTargetRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SmoothFocusInterpSpeed = 150;
};
