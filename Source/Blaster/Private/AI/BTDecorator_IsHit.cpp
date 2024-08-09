// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsHit.h"
#include "AIController.h"
#include "Interfaces/AIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_IsHit::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	//UE_LOG(LogTemp, Display, TEXT("CalculateRawConditionValue"));

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return false;
	}

	IAIInterface* Enemy = Cast<IAIInterface>(ControllingPawn);
	if (Enemy == nullptr)
	{
		return false;
	}

	//if (Enemy->IIsHit()) {
	//	UE_LOG(LogTemp, Display, TEXT("isHit"));
	//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsHit"), false);
	//	return true;
	//}


	return false;
}

FString UBTDecorator_IsHit::GetStaticDescription() const
{
	Super::GetStaticDescription();

	//UE_LOG(LogTemp, Display, TEXT("GetStaticDescription"));

	return FString();
}

void UBTDecorator_IsHit::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);

	UE_LOG(LogTemp, Display, TEXT("OnNodeActivation"));
}

void UBTDecorator_IsHit::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
	Super::OnNodeDeactivation(SearchData, NodeResult);

	UE_LOG(LogTemp, Display, TEXT("OnNodeDeactivation"));
}

void UBTDecorator_IsHit::OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult)
{
	Super::OnNodeProcessed(SearchData, NodeResult);

	UE_LOG(LogTemp, Display, TEXT("OnNodeProcessed"));
}
