// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_Chance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlasterTypes/BlackboardKeys.h"
#include "Kismet/KismetSystemLibrary.h"

bool UBTDecorator_Chance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	float Rate = FMath::RandRange(0.f, 1.f);

	if (Rate < SuccessRate)
	{
		return true;
	}
	else
	{
		return false;
	}
}
