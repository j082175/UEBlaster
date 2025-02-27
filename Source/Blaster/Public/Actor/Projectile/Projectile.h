// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Item/Pickable/Weapon/Weapon.h"
#include "Actor/PooledObject.h"
#include "Projectile.generated.h"

UCLASS()
class BLASTER_API AProjectile : public APooledObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	virtual void Destroyed() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetIsActive(bool InIsActive) override;

	virtual void SetProjectileMovementVelocity(const FVector& InVelocity);


	// Used with server-side rewind
	UPROPERTY(EditAnywhere)
	bool bUseServerSideRewind = false;
	FVector_NetQuantize TraceStart;
	FVector_NetQuantize100 InitialVelocity;

	UPROPERTY(EditAnywhere)
	float InitialSpeed = 30000.f;

	// Only set this for Grenades and Rockets
	UPROPERTY(EditAnywhere)
	float Damage = 21.f;

	// Doesn't matter for Grenades and Rockets
	UPROPERTY(EditAnywhere)
	float HeadShotDamage = 40.f;

	UPROPERTY(EditAnywhere)
	float DamageDeviation;

	UPROPERTY()
	float RandomDamage;

	UPROPERTY()
	float RandomHeadShotDamage;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);



	void ExplodeDamage();
protected:

	UPROPERTY(EditAnywhere)
	float DamageInnerRadius = 200.f;
	UPROPERTY(EditAnywhere)
	float DamageOuterRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBoxComponent> CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UParticleSystem> ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class USoundCue> ImpactSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> ProjectileMesh;


private:



	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystem> Tracer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystemComponent> TracerComponent;

protected:
	void StartDestroyTimer();
	void DestroyTimerFinished();


private:
	FTimerHandle DestroyTimer;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 3.f;



protected:
	void ApplyForce(class UFieldSystemComponent* FieldSystemComponent, const FHitResult& HitPoint);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FieldSystem", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UFieldSystemComponent> FieldSystemComponent;

protected:
	void SpawnTrailSystem();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UNiagaraSystem> TrailSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UNiagaraComponent> TrailSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMaterial> BulletHole;
};
