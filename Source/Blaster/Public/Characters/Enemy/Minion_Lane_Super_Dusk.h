// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/Enemy.h"
#include "PhysicsEngine/SphylElem.h"
#include "Minion_Lane_Super_Dusk.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AMinion_Lane_Super_Dusk : public AEnemy
{
	GENERATED_BODY()
public:
	AMinion_Lane_Super_Dusk();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


protected:
	virtual void BeginPlay() override;
	virtual void OnMontageEndedFunc(UAnimMontage* Montage, bool bInterrupted) override;
	virtual void InitializeCollisionStates() override;
	virtual void InitializeDefaults() override;
	virtual void InitializeDelegates() override;

public:
	virtual void Tick(float DeltaTime) override;
	
private:
	UFUNCTION()
	void OnRightWeaponCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLeftWeaponCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void OnPlayMontageNotifyBeginFunc(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload) override;

	void InitLaserSpawner();
	void LaunchLaser();

	virtual void RandomAttack(int32 Index, const FString& AttackType) override;


	void TeleportAttack();




	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float Damage = 20.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UFieldSystemComponent> FieldSystemComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystem> ImpactParticle;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystem> TeleportStartParticle;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystem> TeleportEndParticle;

	//UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	//TObjectPtr<class UParticleSystem> SkyLaserParticle;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> TeleportStartSound;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> TeleportEndSound;


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AParticleSpawner> ParticleSpawner;

	//UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	//TSubclassOf<class AProjectile> ProjectileToSpawn;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class AParticleSpawner>> ParticleSpawnerArr;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float TeleportDelay = 1.f;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	APawn* TeleportTo;

	uint8 bCheckLeftRightAttack : 1;

};
