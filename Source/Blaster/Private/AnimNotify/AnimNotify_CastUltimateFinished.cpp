// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_CastUltimateFinished.h"
#include "Components/SkillComponent.h"
#include "Blaster/Blaster.h"

void UAnimNotify_CastUltimateFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (USkillComponent* SC = Owner->GetComponentByClass<USkillComponent>())
		{
			SC->UltimateCast();
		}
	}
}
