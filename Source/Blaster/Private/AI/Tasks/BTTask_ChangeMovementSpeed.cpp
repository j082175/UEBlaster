// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_ChangeMovementSpeed.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_ChangeMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACharacter* ControllingPawn = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	OriginMaxWalkSpeed = ControllingPawn->GetCharacterMovement()->MaxWalkSpeed;
	
	ControllingPawn->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeedToModify;

	return EBTNodeResult::Succeeded;
}
