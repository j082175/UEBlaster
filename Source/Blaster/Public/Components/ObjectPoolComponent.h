// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actor/PooledObject.h"
#include "ObjectPoolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectPoolComponent();

	UFUNCTION(BlueprintCallable)
	APooledObject* GetSpawnedObject(const FTransform& SpawnTo, UClass* ClassInfo);

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPooledObjectDespawn(class APooledObject* PoolActor);

private:
	void GenerateObject();

	APooledObject* Get(const FTransform& SpawnTo, UClass* InName);

	TMap<UClass*, int32> ObjectIndexPool;
	TMap<UClass*, TArray<APooledObject*>> ObjectPool;


	//UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	//int DefaultPoolSize = 50.f;

	//UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	//float PooledObjectLifeSpan = 0.f;


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class APooledObject>> ObjectsToSpawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TArray<int32> PoolSizeArr;

	//UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	//TArray<int32> PooledObjectLifeSpan;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TArray<int32> SpawnedPoolIndexes;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TArray<int32> PooledObjectLifeSpan;
};
