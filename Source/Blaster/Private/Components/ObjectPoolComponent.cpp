// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ObjectPoolComponent.h"
#include "Actor/PooledObject.h"
#include "Actor//PooledCharacter.h"
//#include "Interfaces/ObjectPoolInterface.h"

// Sets default values for this component's properties
UObjectPoolComponent::UObjectPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...



}

void UObjectPoolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Warning, TEXT("ObjectPoolComponent tick"));
}

void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	GenerateObject();
}

APooledObject* UObjectPoolComponent::GetSpawnedObject(const FTransform& SpawnTo, UClass* ClassInfo)
{
	if (!bTurnOffGenerate)
	{
		if (ObjectPool.Contains(ClassInfo))
		{
			APooledObject* PO = Get(SpawnTo, ClassInfo);
			if (PO) return PO;
		}
	}

	{
		APooledObject* PO = GetWorld()->SpawnActorDeferred<APooledObject>(ClassInfo, SpawnTo);
		PO->SetIsNotPoolable(true);
		PO->FinishSpawning(SpawnTo);
		return PO;
	}

}

APooledObject* UObjectPoolComponent::GetSpawnedObjectDeferred(const FTransform& SpawnTo, UClass* ClassInfo)
{
	if (!bTurnOffGenerate)
	{
		if (ObjectPool.Contains(ClassInfo))
		{
			int32 ObjectIndex = *ObjectIndexPool.Find(ClassInfo);
			int32 OutIndex = SpawnedPoolIndexes[ObjectIndex];
			APooledObject* PO = ObjectPool[ClassInfo][OutIndex];
			if (PO) return PO;
		}
	}

	{
		APooledObject* PO = GetWorld()->SpawnActorDeferred<APooledObject>(ClassInfo, SpawnTo);
		PO->SetIsNotPoolable(true);
		PO->FinishSpawning(SpawnTo);
		return PO;
	}

}

APooledCharacter* UObjectPoolComponent::GetSpawnedCharacter(const FTransform& SpawnTo, UClass* ClassInfo)
{
	if (!bTurnOffGenerate)
	{
		if (CharacterObjectPool.Contains(ClassInfo))
		{
			APooledCharacter* PC = GetCharacter(SpawnTo, ClassInfo);
			if (PC) return PC;
		}
	}

	{
		APooledCharacter* PC = GetWorld()->SpawnActorDeferred<APooledCharacter>(ClassInfo, SpawnTo);
		PC->SpawnDefaultController();
		PC->FinishSpawning(SpawnTo);
		return PC;
	}

}

APooledObject* UObjectPoolComponent::FinishSpawning(const FTransform& SpawnTo, UClass* InName)
{
	if (!bTurnOffGenerate)
	{
		if (!ObjectPool.Contains(InName))
		{
			UE_LOG(LogTemp, Error, TEXT("UObjectPoolComponent::Get : No Objects inbound"));
			return nullptr;
		}

		//UE_LOG(LogTemp, Display, TEXT("Get"));
		int32 ObjectIndex = *ObjectIndexPool.Find(InName);

		int32 OutIndex = SpawnedPoolIndexes[ObjectIndex];
		APooledObject* O = ObjectPool[InName][OutIndex];

		++OutIndex;
		if (OutIndex >= ObjectPool[InName].Num())
		{
			OutIndex = 0;
		}

		//UE_LOG(LogTemp, Display, TEXT("ObjectIndex : %d"), OutIndex);

		SpawnedPoolIndexes[ObjectIndex] = OutIndex;

		if (O->IsActive())
		{
			O->Deactivate();
		}

		if (!O->TeleportTo(SpawnTo.GetLocation(), SpawnTo.GetRotation().Rotator()))
			UE_LOG(LogTemp, Error, TEXT("TeleportTo Error"));

		O->SetLifeTime(PooledObjectLifeSpan[ObjectIndex]);
		O->SetIsActive(true);
		return O;
	}
	
	return nullptr;
}

void UObjectPoolComponent::OnPooledObjectDespawn(APooledObject* InPooledObject)
{


	if (ObjectPool.Contains(InPooledObject->GetClass()))
	{
		int a = 1;
		//ObjectPool[InPooledObject->GetClass()].Push(InPooledObject);
		//ObjectPool[InPooledObject->GetClass()][InPooledObject->GetPoolIndex()]->SetIsActive(false);
	}
}

void UObjectPoolComponent::GenerateObject()
{
	if (bTurnOffGenerate)
	{
		UE_LOG(LogTemp, Warning, TEXT("TurnOnGenerate false, GenerateObject Denied"));
		return;
	}

	if (PoolSizeArr.Num() != ObjectsToSpawn.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectPoolComponent::GenerateObject : PoolSizeArr and ObjectsToSpawn Num Not Equals, Generate Denied"));
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{

		// Objects
		int Count = 0;
		for (TSubclassOf<APooledObject> PO : ObjectsToSpawn)
		{
			ObjectPool.Add(PO);
			ObjectPool[PO].Reserve(PoolSizeArr[Count]);

			ObjectIndexPool.Add(PO, Count);

			SpawnedPoolIndexes.Reserve(ObjectsToSpawn.Num());

			SpawnedPoolIndexes.Add(0);
			
			for (size_t i = 0; i < PoolSizeArr[Count]; i++)
			{
				FTransform T(FRotator::ZeroRotator, FVector(0.f, 0.f, 100.f));
				APooledObject* Actor = World->SpawnActorDeferred<APooledObject>(PO, T);
				if (Actor)
				{
					Actor->SetIsActive(false);
					Actor->SetPoolIndex(i);
					Actor->FinishSpawning(T);
					Actor->OnPooledObjectDespawn.AddUniqueDynamic(this, &ThisClass::OnPooledObjectDespawn);

					ObjectPool[PO].Add(Actor);

				}
			}

			++Count;
		}

		// Characters
		Count = 0;
		for (TSubclassOf<APooledCharacter> PO : CharactersToSpawn)
		{
			CharacterObjectPool.Add(PO);
			CharacterObjectPool[PO].Reserve(CharacterPoolSizeArr[Count]);

			CharacterIndexPool.Add(PO, Count);

			CharacterSpawnedPoolIndexes.Reserve(CharactersToSpawn.Num());

			CharacterSpawnedPoolIndexes.Add(0);

			for (size_t i = 0; i < CharacterPoolSizeArr[Count]; i++)
			{
				FTransform T(FRotator::ZeroRotator, FVector(0.f, 0.f, 100.f));
				APooledCharacter* PooledCharacter = World->SpawnActorDeferred<APooledCharacter>(PO, T);
				if (PooledCharacter)
				{
					PooledCharacter->SpawnDefaultController();
					PooledCharacter->FinishSpawning(T);

					PooledCharacter->SetIsActive(false);
					PooledCharacter->SetPoolIndex(i);
					//Actor->OnPooledObjectDespawn.AddDynamic(this, &ThisClass::OnPooledObjectDespawn);

					CharacterObjectPool[PO].Add(PooledCharacter);
				}
			}

			++Count;
		}
	}
}

APooledObject* UObjectPoolComponent::Get(const FTransform& SpawnTo, UClass* InName)
{
	if (!ObjectPool.Contains(InName))
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectPoolComponent::Get : No Objects inbound"));
		return nullptr;
	}

	//UE_LOG(LogTemp, Display, TEXT("Get"));
	int32 ObjectIndex = *ObjectIndexPool.Find(InName);
	
	int32 OutIndex = SpawnedPoolIndexes[ObjectIndex];

	if (ObjectPool[InName].Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectPoolComponent::Get :Objects num is zero!"));
		return nullptr;
	}

	APooledObject* O = ObjectPool[InName][OutIndex];

	++OutIndex;
	if (OutIndex >= ObjectPool[InName].Num())
	{
		OutIndex = 0;
	}

	//UE_LOG(LogTemp, Display, TEXT("ObjectIndex : %d"), OutIndex);

	SpawnedPoolIndexes[ObjectIndex] = OutIndex;

	if (O->IsActive())
	{
		O->Deactivate();
	}

	if (!O->TeleportTo(SpawnTo.GetLocation(), SpawnTo.GetRotation().Rotator()))
		UE_LOG(LogTemp, Error, TEXT("TeleportTo Error"));

	O->SetLifeTime(PooledObjectLifeSpan[ObjectIndex]);
	O->SetIsActive(true);
	return O;
}

APooledCharacter* UObjectPoolComponent::GetCharacter(const FTransform& SpawnTo, UClass* InName)
{

	if (!CharacterObjectPool.Contains(InName))
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectPoolComponent::GetCharacter : No Characters inbound"));
		return nullptr;
	}

	int32 ObjectIndex = *CharacterIndexPool.Find(InName);

	int32 OutIndex = CharacterSpawnedPoolIndexes[ObjectIndex];

	if (CharacterObjectPool[InName].Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectPoolComponent::GetCharacter :Characters num is zero!"));
		return nullptr;
	}

	APooledCharacter* PooledCharacter = CharacterObjectPool[InName][OutIndex];

	++OutIndex;
	if (OutIndex >= CharacterObjectPool[InName].Num())
	{
		OutIndex = 0;
	}

	//UE_LOG(LogTemp, Display, TEXT("ObjectIndex : %d"), OutIndex);

	CharacterSpawnedPoolIndexes[ObjectIndex] = OutIndex;

	if (PooledCharacter->IsActive())
	{
		//PooledCharacter->Deactivate();
		return nullptr;
	}

	if (!PooledCharacter->TeleportTo(SpawnTo.GetLocation(), SpawnTo.GetRotation().Rotator()))
		UE_LOG(LogTemp, Error, TEXT("TeleportTo Error"));
	PooledCharacter->SetLifeTime(CharacterPooledObjectLifeSpan[ObjectIndex]);
	PooledCharacter->SetIsActive(true);

	return PooledCharacter;
}

