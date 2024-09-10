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

	FString DistanceStr = Target == ETarget::OwingActor ? OWING_ACTOR_DISTANCE : TARGET_ACTOR_DISTANCE;
	FString Actor = Target == ETarget::OwingActor ? OWING_ACTOR : TARGET_ACTOR;

	AActor* TargetActor = Cast<AActor>(BC->GetValueAsObject(*Actor));
	if (TargetActor == nullptr) return;

	float Distance = (AIPawn->GetActorLocation() - TargetActor->GetActorLocation()).Size2D();

	BC->SetValueAsFloat(*DistanceStr, Distance);
}
