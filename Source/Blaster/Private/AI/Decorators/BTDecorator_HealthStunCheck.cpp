// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_HealthStunCheck.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIController.h"

#include "Characters/Enemy/Enemy.h"

bool UBTDecorator_HealthStunCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) return false;

	// Check Character Status
	
	// is alive and not stunning => return true;

	return true;
}
