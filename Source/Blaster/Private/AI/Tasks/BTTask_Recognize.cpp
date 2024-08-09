// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_Recognize.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Characters/Enemy/Enemy.h"

EBTNodeResult::Type UBTTask_Recognize::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;
	AIEnemy = Cast<AEnemy>(ControllingPawn);
	if (AIEnemy == nullptr) return EBTNodeResult::Failed;
	BC = OwnerComp.GetBlackboardComponent();
	if (BC == nullptr) return EBTNodeResult::Failed;

	if (CheckHistory())
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				AdjustRecognizeTime();
			}), 0.5f, true);

		// TODO: Change Character's speed
	}
	else
	{
		SetupRecognizeTime();
	}


	return EBTNodeResult::Type();
}

void UBTTask_Recognize::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	// Target Recognized

	if (UGameplayStatics::GetTimeSeconds(this) >= RecognizeTime || BC->GetValueAsBool(TEXT("DoRecognize")))
	{
		BC->SetValueAsFloat(TEXT("RemainRecognizeTime"), 0.1f);
		BC->SetValueAsFloat(TEXT("LastRecognizedTime"), UGameplayStatics::GetTimeSeconds(this));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		// Update AI Widget & force to look to target
		if (AIEnemy)
		{
			float Percent = FMath::GetMappedRangeValueClamped(FVector2D(FirstSeeTime, RecognizeTime), FVector2D(1.1f, -0.1f), UGameplayStatics::GetTimeSeconds(this));
			// TODO: Update Recognize Widget

		}
	}

}

EBTNodeResult::Type UBTTask_Recognize::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Save Remaining time to recognize in BB and abort the task
	Super::AbortTask(OwnerComp, NodeMemory);

	BC->SetValueAsFloat(TEXT("RemainRecognizeTime"), RecognizeTime - UGameplayStatics::GetTimeSeconds(this));
	// TODO: Update Recognize Widget
	FinishLatentAbort(OwnerComp);

	return EBTNodeResult::Aborted;
}

bool UBTTask_Recognize::CheckHistory()
{
	// Check to see if we can remember character by comparing current time & last recognized time
	float LastRecognizedTime = BC->GetValueAsFloat(TEXT("LastRecognizedTime"));

	float A1 = AIEnemy->GetAIConfig().RememberTime + LastRecognizedTime;

	if (LastRecognizedTime > 0.f && A1 > UGameplayStatics::GetTimeSeconds(this))
	{
		// TODO: Update Recognition Bar
		UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
		const EBTNodeResult::Type NodeResult(true ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
		FinishLatentTask(*OwnerComp, NodeResult);
		return false;
	}
	else
	{
		// Add remaining recognize target to current time as new recognize time
		float RemainRecognizeTime = BC->GetValueAsFloat(TEXT("RemainRecognizeTime"));

		if (RemainRecognizeTime > 0.f)
		{
			RecognizeTime = UGameplayStatics::GetTimeSeconds(this) + RemainRecognizeTime;
			FirstSeeTime = UGameplayStatics::GetTimeSeconds(this);
			return true;
		}
	}

	return false;
}

void UBTTask_Recognize::SetupRecognizeTime()
{
	// Set first see time & when character can recognize the target
	FirstSeeTime = UGameplayStatics::GetTimeSeconds(this);
	RecognizeTime = FirstSeeTime + AIEnemy->GetAIConfig().MinRecognizeTime;
}

void UBTTask_Recognize::AdjustRecognizeTime()
{
	// Adjust recognize time a little bit if character is moving closer

	AActor* Target = Cast<AActor>(BC->GetValueAsObject(TEXT("Target")));
	if (Target == nullptr) return;

	TempCurrentDistance = (AIEnemy->GetActorLocation() - Target->GetActorLocation()).Length();

	if (TempCurrentDistance < LastDistance)
	{
		if (TempCurrentDistance < 250.f)
		{
			RecognizeTime -= UKismetMathLibrary::MapRangeClamped(TempCurrentDistance, 0.f, 250.f, 0.4f, 3.f);
		}
		else
		{
			RecognizeTime -= UKismetMathLibrary::MapRangeClamped(TempCurrentDistance, 250.f, 3000.f, 0.4f, 0.f);
		}

	}
	else
	{
		LastDistance = TempCurrentDistance;
	}
}
