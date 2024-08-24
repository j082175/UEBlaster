// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStart/TeamPlayerStart.h"

void ATeamPlayerStart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("TeamPlayerStart tick"));
}
