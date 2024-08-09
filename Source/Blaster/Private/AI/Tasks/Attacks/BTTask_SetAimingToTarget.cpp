// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/Attacks/BTTask_SetAimingToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/CharacterBase.h"
#include "Item/Pickable/Weapon/Weapon_Gun.h"
#include "BlasterTypes/BlackboardKeys.h"


EBTNodeResult::Type UBTTask_SetAimingToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACharacterBase* Character = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr) return EBTNodeResult::Failed;

	UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();
	if (BC == nullptr) return EBTNodeResult::Failed;

	if (Character->GetEquippedWeapon())
	{
		AActor* TargetActor = Cast<AActor>(BC->GetValueAsObject(TARGET_ACTOR));
		

		FTransform MuzzleTipTransform = Cast<AWeapon_Gun>(Character->GetEquippedWeapon())->GetWeaponMesh()->GetSocketTransform(TEXT("MuzzleFlash"));
		DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), TargetActor->GetActorLocation(), FColor::Red, false);

	}

	return EBTNodeResult::Succeeded;
}
