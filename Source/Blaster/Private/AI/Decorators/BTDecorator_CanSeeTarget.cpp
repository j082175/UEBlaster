// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_CanSeeTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlasterTypes/BlackboardKeys.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blaster/Blaster.h"
#include "GameFramework/Character.h"
//#include "Engine/SkeletalMeshSocket.h"

bool UBTDecorator_CanSeeTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("AIController is null"));
		return false;
	}
	APawn* AIPawn = AIController->GetPawn();
	if (AIPawn == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("AIPawn is null"));
		return false;
	}
	UBlackboardComponent* BC = AIController->GetBlackboardComponent();
	if (BC == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("BC is null"));
		return false;
	}
	AActor* TargetActor = Cast<AActor>(BC->GetValueAsObject(TARGET_ACTOR));
	if (TargetActor == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("TargetActor is null"));
		return false;
	}
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AIPawn);
	FHitResult OutHitResult;

	ETraceTypeQuery CanDamagedByWeapon = UEngineTypes::ConvertToTraceType(ECC_CanDamagedByWeapon);

	FVector SocketLocation = Cast<ACharacter>(AIPawn)->GetMesh()->GetSocketLocation(TEXT("neck_01"));
	

	UKismetSystemLibrary::LineTraceSingle(this, SocketLocation, SocketLocation + AIPawn->GetActorForwardVector() * 80000.f, CanDamagedByWeapon, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHitResult, true, FLinearColor::Green);


	//UE_LOG(LogTemp, Display, TEXT("OutHitResult : %x"), OutHitResult.GetActor());

	//if (OutHitResult.GetActor())
	//{
	//	UE_LOG(LogTemp, Display, TEXT("AIPawn : %s"), *AIPawn->GetName());
	//	UE_LOG(LogTemp, Display, TEXT("OutHitResult : %s"), *OutHitResult.GetActor()->GetName());
	//	UE_LOG(LogTemp, Display, TEXT("TargetActor : %s"), *TargetActor->GetName());
	//}

	if (OutHitResult.bBlockingHit && OutHitResult.GetActor() == TargetActor)
	{
		//UE_LOG(LogTemp, Display, TEXT("TimeToSee : %f"), TimeToSee);
		TimeToSee += GetWorld()->GetDeltaSeconds() * 3.f;


		if (TimeToSee >= CanSeeThreshold)
		{
			TimeToSee = 0.f;
			return true;
		}
	}
	return false;

}
