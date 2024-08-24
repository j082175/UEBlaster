// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController/EnemyAIController.h"
#include "Characters/Enemy/Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Perception/AIPerceptionComponent.h"
//#include "Perception/AISense.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "AISystem.h"

#include "Interfaces/AIInterface.h"

#include "BlasterTypes/BlackboardKeys.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));


	//Update UAISenseConfig_Sight properties based con possessed pawn

	// Setup AIController's Perception system values
	//AIPerceptionComponent = GetAIPerceptionComponent();

	
}

void AEnemyAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	//if (AIPerceptionComponent)
	//{
	//	FAISenseID SenseID = UAISense::GetSenseID<UAISense_Sight>();

	//	UAISenseConfig_Sight* SenseConfig = Cast<UAISenseConfig_Sight>(AIPerceptionComponent->GetSenseConfig(SenseID));
	//	if (SenseConfig)
	//	{
	//		// Noticie here, in AMyCharacter I have two properties AISenseSight_Radius, AISenseSight_LoseSightRadius that allow me to define
	//		// the perception setup for this specific character using the same AIController class.
	//		SenseConfig->SightRadius = 800.f;
	//		SenseConfig->LoseSightRadius = 1200.f;

	//		AIPerceptionComponent->ConfigureSense(*SenseConfig);
	//	}
	//}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	BindPerceptionFunctions(true);

	
	if (AISense_Sight) AIPerceptionComponent->SetDominantSense(AISense_Sight);

	AIPawn = Cast<IAIInterface>(GetPawn());

	//UWorld* World = GetWorld();
	//if (World)
	//{
	//	UAISystem* AISystem = UAISystem::GetCurrent(*World);
	//	if (AISystem) AISystem->bForgetStaleActors = true;

	//	bool b = AISystem->bForgetStaleActors;
	//	int a = 1;
	//}
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("EnemyAIController Tick"));
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	//UE_LOG(LogTemp, Display, TEXT("OnPossess"));

	Super::OnPossess(InPawn);

	RunAI();
}

void AEnemyAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	Super::UpdateControlRotation(DeltaTime, false);

	if (bUpdatePawn)
	{
		//Get pawn
		APawn* const MyPawn = GetPawn();

		if (!MyPawn) return;

		//Get Pawn current rotation
		const FRotator CurrentPawnRotation = MyPawn->GetActorRotation();

		//Calculate smoothed rotation
		SmoothTargetRotation = UKismetMathLibrary::RInterpTo_Constant(MyPawn->GetActorRotation(), ControlRotation, DeltaTime, SmoothFocusInterpSpeed);
		//Check if we need to change
		if (CurrentPawnRotation.Equals(SmoothTargetRotation, 1e-3f) == false)
		{
			//Change rotation using the Smooth Target Rotation
			MyPawn->FaceRotation(SmoothTargetRotation, DeltaTime);
		}
	}
}

void AEnemyAIController::RunAI()
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	ensure(UseBlackboard(BB_Asset, BlackboardComponent));
	GetBlackboardComponent()->SetValueAsVector(TEXT("HomePos"), GetPawn()->GetActorLocation());
	ensure(RunBehaviorTree(BT_Asset));

	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(GetBrainComponent());
	ensure(BehaviorTreeComponent);
	BehaviorTreeComponent->RestartTree();

	BindPerceptionFunctions(true);
}

void AEnemyAIController::StopAI()
{
	UE_LOG(LogTemp, Display, TEXT("StopAI"));
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(GetBrainComponent());
	ensure(BehaviorTreeComponent);
	BehaviorTreeComponent->RestartTree();
	BehaviorTreeComponent->StopLogic(TEXT("Elimed"));

	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	//BlackboardComponent->SetValueAsObject(TARGET_ACTOR, nullptr);

	BindPerceptionFunctions(false);
}

void AEnemyAIController::BindPerceptionFunctions(bool InSet)
{
	if (InSet)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnTargetPerceptionUpdated);
		AIPerceptionComponent->OnPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnPerceptionUpdated);
		AIPerceptionComponent->OnTargetPerceptionForgotten.AddUniqueDynamic(this, &ThisClass::OnTargetPerceptionForgotten);
		AIPerceptionComponent->OnTargetPerceptionInfoUpdated.AddUniqueDynamic(this, &ThisClass::OnTargetPerceptionInfoUpdated);
	}
	else
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &ThisClass::OnTargetPerceptionUpdated);
		AIPerceptionComponent->OnPerceptionUpdated.RemoveDynamic(this, &ThisClass::OnPerceptionUpdated);
		AIPerceptionComponent->OnTargetPerceptionForgotten.RemoveDynamic(this, &ThisClass::OnTargetPerceptionForgotten);
		AIPerceptionComponent->OnTargetPerceptionInfoUpdated.RemoveDynamic(this, &ThisClass::OnTargetPerceptionInfoUpdated);

		if (!AIPerceptionComponent->OnTargetPerceptionUpdated.IsAlreadyBound(this, &ThisClass::OnTargetPerceptionUpdated))
		{
			UE_LOG(LogTemp, Display, TEXT("OnTargetPerceptionUpdated Unbound"));
		}

		if (!AIPerceptionComponent->OnPerceptionUpdated.IsAlreadyBound(this, &ThisClass::OnPerceptionUpdated))
		{
			UE_LOG(LogTemp, Display, TEXT("OnTargetPerceptionUpdated Unbound"));
		}

		if (!AIPerceptionComponent->OnTargetPerceptionForgotten.IsAlreadyBound(this, &ThisClass::OnTargetPerceptionForgotten))
		{
			UE_LOG(LogTemp, Display, TEXT("OnTargetPerceptionUpdated Unbound"));
		}

		if (!AIPerceptionComponent->OnTargetPerceptionInfoUpdated.IsAlreadyBound(this, &ThisClass::OnTargetPerceptionInfoUpdated))
		{
			UE_LOG(LogTemp, Display, TEXT("OnTargetPerceptionUpdated Unbound"));
		}
	}
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	//UE_LOG(LogTemp, Display, TEXT("Percepted Actor : %s"), *Actor->GetName());
	//for (UAIPerceptionComponent::TAISenseConfigConstIterator iter = AIPerceptionComponent->GetSensesConfigIterator(); iter; iter++)
	//{

	//}

	// if target is same actor
	if (GetBlackboardComponent())
	{
		AActor* TargetActor = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TARGET_ACTOR));
		if (TargetActor)
		{
			GetBlackboardComponent()->SetValueAsBool(CAN_SEE, true);
			return;
		}
	}

	FActorPerceptionBlueprintInfo Info;
	AIPerceptionComponent->GetActorsPerception(Actor, Info);

	TSubclassOf<UAISense> Result = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);

	if (Result == AISense_Sight)
	{ 
		//UE_LOG(LogTemp, Display, TEXT("AISense_Sight Detected"));
		if (GetBlackboardComponent()) GetBlackboardComponent()->SetValueAsObject(TARGET_ACTOR, Actor);
	}
	else if (Result == AISense_Hearing)
	{
		//UE_LOG(LogTemp, Display, TEXT("AISense_Hearing Detected"));
		if (GetBlackboardComponent()) GetBlackboardComponent()->SetValueAsVector(DETECTED_LOCATION, Stimulus.StimulusLocation);
	}
	else if (Result == AISense_Damage)
	{
		//UE_LOG(LogTemp, Display, TEXT("AISense_Damage Detected"));

	}
	else if (Result == AISense_Prediction)
	{
		//UE_LOG(LogTemp, Display, TEXT("AISense_Prediction Detected"));
	}
	else if (Result == AISense_Team)
	{
		//UE_LOG(LogTemp, Display, TEXT("AISense_Team Detected"));
	}
	else if (Result == AISense_Touch)
	{
		//UE_LOG(LogTemp, Display, TEXT("AISense_Touch Detected"));
	}
	else if (Result != nullptr)
	{
		if (AIPawn) AIPawn->ISetAIState(EAIState::EAI_Attacking);

	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("No Sensing"));
	}
}

void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	//for (size_t i = 0; i < UpdatedActors.Num(); i++)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("UpdatedActor : %s"), *UpdatedActors[i]->GetName());
	//}
}

void AEnemyAIController::OnTargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnTargetPerceptionInfoUpdated : %s"), *UpdateInfo.Target->GetName());

}

void AEnemyAIController::OnTargetPerceptionForgotten(AActor* Actor)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnTargetPerceptionForgotten : %s"), *Actor->GetName());
	
	AActor* TargetActor = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TARGET_ACTOR));
	if (TargetActor == Actor && GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsObject(TARGET_ACTOR, nullptr);
		SetFocus(nullptr);
		if (AIPawn) AIPawn->ISetAIState(EAIState::EAI_Passive);
	}

}
