 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Enemy.h"
//#include "Characters/BlasterCharacter.h"
#include "Interfaces/AIInterface.h"
#include "EnemyRange.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AEnemyRange : public AEnemy
{
	GENERATED_BODY()
public:
	AEnemyRange();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnMontageEndedFunc(UAnimMontage* Montage, bool bInterrupted) override;
	virtual void InitializeDefaults() override;

	virtual void Fire(bool bPressed) override;

protected:


	virtual void IAttack(FAttackEndedDelegate Delegate, const FString& AttackType) override;
	virtual void ISetAIState(EAIState InAIState) override;

	virtual void FinishReloading() override;


	virtual void FireProjectileWeapon(bool bPressed) override;
	virtual void FireHitScanWeapon(bool bPressed) override;
	virtual void FireShotgun(bool bPressed) override;

	void AttackFunc();

	virtual void SetIsActive(bool InIsActive) override;


	FVector TargetPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AWeapon_Gun* EquippedGun;


	float TargetPointCheckCount = 0.f;
	float TargetPointCheckThreshold = 2.f;
	FVector TargetPointCheck;


	virtual void StartFireTimer() override;
	virtual void FireTimerFinished() override;

private:
	void InitEnemyOverhead();
};
