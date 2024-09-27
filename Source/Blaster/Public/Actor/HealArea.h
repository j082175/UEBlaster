// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenceActor.h"
#include "HealArea.generated.h"

UCLASS()
class BLASTER_API AHealArea : public ADefenceActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealArea();
	virtual void BeginPlay() override;
private:
	virtual void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float HealingRate = 20.f;
};
