// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MyGameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::SpawnCharacter()
{
	if (LastPlayer)
	{
		LastPlayer->Destroy();

		TSubclassOf<AActor> P = (Player.LoadSynchronous());

		AActor* A = UGameplayStatics::GetActorOfClass(this, P);


		AActor* SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(P, A->GetActorTransform());
		if (SpawnedActor)
		{
			LastPlayer = SpawnedActor;
			SpawnedActor->FinishSpawning(A->GetActorTransform());
		}
	}
}
