// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickable/Pickable.h"
void APickable::AttachToComponentMulticast_Implementation(USceneComponent* Parent, FName SocketName)
{
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Parent, Rules, SocketName);
}

