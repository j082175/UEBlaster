// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/PooledObject.h"
#include "Casing.generated.h"

UCLASS()
class BLASTER_API ACasing : public APooledObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACasing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:	

	virtual void SetIsActive(bool InIsActive) override;


private:
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	uint8 bIsHit : 1 = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> CasingMesh;

	UPROPERTY(EditAnywhere)
	float ShellEjectionImpulse;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundCue> ShellSound;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float DestroyTime = 1.5f;
};
