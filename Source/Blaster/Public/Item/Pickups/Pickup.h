// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "Item/Item.h"
#include "Pickup.generated.h"

DECLARE_DELEGATE_OneParam(FOnSpawnedPickupDisabledDelegate, AActor* /*DestroyedActor*/);


UCLASS()
class BLASTER_API APickup : public AItem
{
	GENERATED_BODY()

public:
	APickup();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;


	FOnSpawnedPickupDisabledDelegate OnSpawnedPickupDisabled;

	//UFUNCTION()
	//virtual void OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	float BaseTurnRate = 45.f;

	UFUNCTION()
	virtual void OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCapsuleEndOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	virtual void InitializeDelegates() override;

	// Particle


	// Niagara Effect
//private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> PersistEffectComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> PickupEffect;

	//	// Components
	//private:
	//	UPROPERTY(EditAnywhere)
	//	TObjectPtr<class USphereComponent> OverlapSphere;
	//
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> PickupMesh;

	// Sound
private:
	//UPROPERTY(EditAnywhere)
	//TObjectPtr<class USoundCue> PickupSound;


};
