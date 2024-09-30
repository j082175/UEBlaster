// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_AttachActor.h"
#include "Components/InventoryComponent.h"
#include "Item/Pickable/Weapon/Weapon.h"
#include "GameFramework/Character.h"

void UAnimNotify_AttachActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacter* Owner = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (!Owner->HasAuthority()) return;
		if (UInventoryComponent* IC = Owner->GetComponentByClass<UInventoryComponent>())
		{
			if (AWeapon* Weapon = IC->GetEquippedWeapon())
			{
				Weapon->AttachToComponentMulticast(Owner->GetMesh(), SocketName);
			}
		}
	}


}
