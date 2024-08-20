// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "Actor/PooledObject.h"
#include "ObjectPoolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectPoolComponent();

	UFUNCTION(BlueprintCallable)
	class APooledObject* GetSpawnedObject(const FTransform& SpawnTo, UClass* ClassInfo);

	UFUNCTION(BlueprintCallable)
	class APooledCharacter* GetSpawnedCharacter(const FTransform& SpawnTo, UClass* ClassInfo);

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPooledObjectDespawn(class APooledObject* PoolActor);

private:
	void GenerateObject();




	//UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	//int DefaultPoolSize = 50.f;

	//UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	//float PooledObjectLifeSpan = 0.f;


	// Objects
	class APooledObject* Get(const FTransform& SpawnTo, UClass* InName);

	TMap<UClass*, TArray<class APooledObject*>> ObjectPool;

	TMap<UClass*, int32> ObjectIndexPool;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class APooledObject>> ObjectsToSpawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TArray<int32> PoolSizeArr;

	TArray<int32> SpawnedPoolIndexes;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TArray<int32> PooledObjectLifeSpan;

	// Characters
	class APooledCharacter* GetCharacter(const FTransform& SpawnTo, UClass* InName);

	TMap<UClass*, TArray<class APooledCharacter*>> CharacterObjectPool;

	TMap<UClass*, int32> CharacterIndexPool;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class APooledCharacter>> CharactersToSpawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TArray<int32> CharacterPoolSizeArr;

	TArray<int32> CharacterSpawnedPoolIndexes;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TArray<int32> CharacterPooledObjectLifeSpan;
};
