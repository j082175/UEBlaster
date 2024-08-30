// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Interfaces
#include "Interfaces/HitInterface.h"
#include "Interfaces/InteractWithCrosshairsInterface.h"

#include "BreakableActor.generated.h"

UCLASS()
class BLASTER_API ABreakableActor : public AActor, public IHitInterface, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

private:
	virtual void InitializeCollisionStates();
	virtual void InitializeDelegates();

private:
	virtual void IGetHit(const FVector& HitPoint, const FHitResult& InHitResult) override;

private:
	UFUNCTION()
	void OnChaosBreakEventFunc(const FChaosBreakEvent& BreakEvent);

	// Components
private:
	UPROPERTY(VisibleAnywhere, Category = "Geometry", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UGeometryCollectionComponent> GeometryCollectionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Capsule", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

private:

	UPROPERTY(EditAnywhere, Category = "Item", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class ATreasure>> Treasures;

	UPROPERTY(EditAnywhere, Category = "Actor", meta = (AllowPrivateAccess = true))
	float LifeSpan = 10.f;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> BreakSound;

	uint8 bIsHit : 1 = false;
};
