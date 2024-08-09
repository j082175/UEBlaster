// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_UpdateInfo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

void UBTService_UpdateInfo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();
	if (BC == nullptr) return;

	BC->SetValueAsFloat(TEXT("LastRecognize"), UGameplayStatics::GetTimeSeconds(this));
	AActor* Target = Cast<AActor>(BC->GetValueAsObject(TEXT("TargetKey")));
	if (Target == nullptr) return;

	BC->SetValueAsVector(TEXT("LastLocation"), Target->GetActorLocation());
}
