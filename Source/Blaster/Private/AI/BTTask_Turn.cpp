// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Turn.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlasterTypes/BlackboardKeys.h"

EBTNodeResult::Type UBTTask_Turn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TARGET_ACTOR));

	if (nullptr == TargetActor)
	{
		return EBTNodeResult::Failed;
	}
	//UE_LOG(LogTemp, Display, TEXT("OwnerController : %s"), *OwnerController->GetName());
	
	OwnerController->SetFocus(TargetActor);

	FVector XAxis = TargetActor->GetActorLocation() - ControllingPawn->GetActorLocation();
	XAxis.Z = 0.f;
	FRotator FocusRot =  FRotationMatrix::MakeFromX(XAxis).Rotator();


	//ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), FocusRot, GetWorld()->GetDeltaSeconds(), 5.f));


	//FTimerHandle TimerHandle;
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
	//	{
	//		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//		UE_LOG(LogTemp, Display, TEXT("FinishLatentTask"));
	//	}), 2.f, true);

	return EBTNodeResult::Succeeded;

}
