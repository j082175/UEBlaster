// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_ShotgunReload.h"
#include "Item/Pickable/Weapon/Shotgun.h"

void UAnimNotify_ShotgunReload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AShotgun* ShotGun = Cast<AShotgun>(MeshComp->GetOwner());
	if (ShotGun)
	{
		ShotGun->EjectCasing();
	}
}
