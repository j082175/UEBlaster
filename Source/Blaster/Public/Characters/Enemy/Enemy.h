// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//#include "GameFramework/Character.h"
#include "Characters/CharacterBase.h"

// Interfaces
//#include "Interfaces/HitInterface.h"
#include "Interfaces/AIInterface.h"
#include "Interfaces/InteractWithCrosshairsInterface.h"
#include "Interfaces/GetPatrolRouteInterface.h"

#include "Enemy.generated.h"

DECLARE_DELEGATE_OneParam(FOnSpawnedEnemyDisabledDelegate, AActor* /*DestroyedActor*/);


USTRUCT(BlueprintType)
struct FAIConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AIConfig|Recognize")
	float MinRecognizeTime = 10.f;
	UPROPERTY(EditAnywhere, Category = "AIConfig|Recognize")
	float RememberTime = 15.f;
};

UCLASS()
class BLASTER_API AEnemy : public ACharacterBase , public IAIInterface, public IInteractWithCrosshairsInterface, public IGetPatrolRouteInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	FORCEINLINE FAIConfig GetAIConfig() const { return AIConfig; }
	FORCEINLINE class ABaseAIController* GetBaseAIController() const { return BaseAIController; }

	virtual void SetOwner(AActor* NewOwner) override;

	FOnSpawnedEnemyDisabledDelegate OnSpawnedEnemyDisabled;
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;



	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser) override;

	virtual void Destroyed() override;
	virtual void SetIsActive(bool InIsActive) override;


	// Initialize
protected:
	virtual void InitializeDelegates() override;
	virtual void InitializeDefaults() override;

	// Components
protected:
	//UPROPERTY(VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
	//TObjectPtr<class UPawnSensingComponent> PawnSensingComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	//TObjectPtr<class UMotionWarpingComponent> MotionWarpingComponent;

	// Interface
protected:
	virtual void IGetHit(const FVector& InHitPoint, const FHitResult& InHitResult, class AController* InPlayerController) override;
	//FORCEINLINE virtual class UPawnSensingComponent* IGetSensingComponent() override { return PawnSensingComponent; }
	//virtual bool IIsHit() override;
	virtual void IAttack(FAttackEndedDelegate Delegate, const FString& AttackType) override;
	virtual void ISetAIState(EAIState InAIState) override;
	virtual class ASplineActor* IGetPatrolRoute() override;
	virtual void ISetPatrolRoute(class ASplineActor* InSplineActor) override;

	//FAttackEndedDelegate OnAttackEnded;

	// Animation
protected:
	virtual void OnMontageEndedFunc(UAnimMontage* Montage, bool bInterrupted) override;
	virtual void UpdateMotionWarpingTransform() override;

	// AI
public:
	void JumpToDestination(FVector InDestination);
protected:



	UFUNCTION()
	void OnSeePawnFunc(class APawn* Pawn);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<class ASplineActor> PatrolTarget;

	UPROPERTY(VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<class ABaseAIController> BaseAIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<AActor> PawnDamageCauser;

	// Set State
protected:
	virtual void SetDead() override;


	// Actor Class
protected:
	UPROPERTY(EditAnywhere, Category = "ActorClassWeapon", meta = (AllowPrivateAccess = true))
	TSubclassOf<class AWeapon_Melee> WeaponClass;

	// Item to spawn
	UPROPERTY(EditAnywhere, Category = "ActorClassItem", meta = (AllowPrivateAccess = true))
	TSubclassOf<class ASoul> SoulToSpawn;

protected:
	UPROPERTY(EditAnywhere)
	FAIConfig AIConfig;

protected:
	virtual void SpawnMagic(FName SocketToSpawn) override;


	// Parry
public:
	bool CheckParryFunc(AActor* OtherActor);
protected:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float ShortStunTime = 3.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float LongStunTime = 10.f;


	// Attack
	
};
