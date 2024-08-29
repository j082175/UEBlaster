// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_CalculateDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlasterTypes/BlackboardKeys.h"

void UBTService_CalculateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIPawn == nullptr) return;

	UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();
	if (BC == nullptr) return;

	AActor* TargetActor = Cast<AActor>(BC->GetValueAsObject(TARGET_ACTOR));
	if (TargetActor == nullptr) return;

	float Distance = (AIPawn->GetActorLocation() - TargetActor->GetActorLocation()).Size2D();

	BC->SetValueAsFloat(TARGET_ACTOR_DISTANCE, Distance);
}
