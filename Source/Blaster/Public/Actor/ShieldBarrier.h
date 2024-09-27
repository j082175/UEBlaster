// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenceActor.h"
#include "ShieldBarrier.generated.h"

UCLASS()
class BLASTER_API AShieldBarrier : public ADefenceActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShieldBarrier();
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	virtual void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;



private:
	void SetShieldFunc();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float ReplenishInterval = 0.5f;

	FTimerHandle ReplenishTimer;

	class UAttributeComponent* AttributeComponent;
};
