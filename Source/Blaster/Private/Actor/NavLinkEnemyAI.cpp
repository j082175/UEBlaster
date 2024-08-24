// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/NavLinkEnemyAI.h"
#include "Characters/Enemy/Enemy.h"

void ANavLinkEnemyAI::BeginPlay()
{
	Super::BeginPlay();

	OnSmartLinkReached.AddDynamic(this, &ThisClass::SmartLinkReached);
}

void ANavLinkEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("NavLinkProxy Tick"));
}

void ANavLinkEnemyAI::SmartLinkReached(AActor* MovingActor, const FVector& DestinationPoint)
{
	//UE_LOG(LogTemp, Display, TEXT("SmartLinkReached"));

	AEnemy* Enemy = Cast<AEnemy>(MovingActor);
	if (Enemy == nullptr) return;

	Enemy->JumpToDestination(DestinationPoint);
}
