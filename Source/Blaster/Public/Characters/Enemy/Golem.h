// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Enemy.h"

#include "BlasterTypes/AttackTypes.h"

#include "Golem.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AGolem : public AEnemy
{
	GENERATED_BODY()
	AGolem();
	virtual void Tick(float DeltaTime) override;
	
private:


	virtual void IAttack(FAttackEndedDelegate Delegate, const FString& InAttackType) override;



	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float RotationRate = 50.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	EAttackType AttackType;
};
