// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_CanSeeTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlasterTypes/BlackboardKeys.h"
#include "Kismet/KismetSystemLibrary.h"

bool UBTDecorator_CanSeeTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr) return false;

	APawn* AIPawn = AIController->GetPawn();
	if (AIPawn == nullptr) return false;

	UBlackboardComponent* BC = AIController->GetBlackboardComponent();
	if (BC == nullptr) return false;

	AActor* TargetActor = Cast<AActor>(BC->GetValueAsObject(TARGET_ACTOR));
	if (TargetActor == nullptr) return false;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AIPawn);
	FHitResult OutHitResult;

	UKismetSystemLibrary::LineTraceSingle(this, AIPawn->GetActorLocation(), TargetActor->GetActorLocation(), ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHitResult, true, FLinearColor::Green);

	

	if (OutHitResult.bBlockingHit && OutHitResult.GetActor() == TargetActor)
	{
		return true;
	}

	return false;
}
