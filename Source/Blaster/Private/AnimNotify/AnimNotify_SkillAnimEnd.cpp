// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_SkillAnimEnd.h"
#include "Characters/BlasterCharacter.h"

void UAnimNotify_SkillAnimEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (ABlasterCharacter* CB = Cast<ABlasterCharacter>(Owner))
		{
			if (CB->GetCombatState() != ECombatState::UltimateMode)
			{
				CB->SetCombatState(ECombatState::Unoccupied);
				CB->HideWeaponBone(true, false);
			}

			CB->SetRotateInPlace(true);
		}
	}
}