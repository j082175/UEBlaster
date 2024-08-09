// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Perception/PawnSensingComponent.h"

#include "Interfaces/AIInterface.h"

#include "BlasterTypes/BlackboardKeys.h"

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		UE_LOG(LogTemp, Display, TEXT("Controlling Pawn null"));
		return;
	}

	IAIInterface* Enemy = Cast<IAIInterface>(ControllingPawn);
	if (!Enemy)
	{
		UE_LOG(LogTemp, Display, TEXT("Controlling Pawn cast failed"));

		return;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	//UPawnSensingComponent* PawnSensingComponent = Enemy->IGetSensingComponent();

	//APawn* Target = Cast<APawn>(BlackboardComponent->GetValueAsObject(TARGET_ACTOR));
	//bool bCanSee = PawnSensingComponent->CouldSeePawn(Target);

	//if (!bCanSee)
	//{
	//	//UE_LOG(LogTemp, Display, TEXT("%s Cant See"), *ControllingPawn->GetName());
	//	BlackboardComponent->SetValueAsObject(TARGET_ACTOR, nullptr);
	//	IntervalDerived = 0.3f;
	//	Interval = IntervalDerived;
	//}
	//else {
	//	//UE_LOG(LogTemp, Display, TEXT("%s Can See"), *ControllingPawn->GetName());
	//	IntervalDerived = 10.f;
	//	Interval = IntervalDerived;
	//}



	//float Distance = (ControllingPawn->GetActorLocation() - Target->GetActorLocation()).Length();

	//if (Distance > Enemy->IGetSightRadius())
	//{
	//	BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), nullptr);
	//}



}
