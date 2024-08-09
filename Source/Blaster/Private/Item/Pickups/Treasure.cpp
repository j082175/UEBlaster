// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickups/Treasure.h"
#include "Kismet/GameplayStatics.h"

//#include 

void ATreasure::OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCapsuleBeginOverlapFunc(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	Destroy();
}
