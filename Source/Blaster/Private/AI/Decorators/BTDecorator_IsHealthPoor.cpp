// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_IsHealthPoor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlasterTypes/BlackboardKeys.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Characters/CharacterBase.h"
#include "Components/AttributeComponent.h"

bool UBTDecorator_IsHealthPoor::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr) return false;

	APawn* AIPawn = AIController->GetPawn();
	if (AIPawn == nullptr) return false;

	UBlackboardComponent* BC = AIController->GetBlackboardComponent();
	if (BC == nullptr) return false;

	ACharacterBase* Character = Cast<ACharacterBase>(AIPawn);
	if (Character == nullptr) return false;

	float MaxHP = Character->GetAttributeComponent()->GetMaxHp();
	float CurrentHP = Character->GetAttributeComponent()->GetCurrentHp();

	float CurrentPercent = CurrentHP / MaxHP;
	if (CurrentPercent < HpThreshold)
	{
		return true;
	}
	else
	{
		return false;
	}
}
