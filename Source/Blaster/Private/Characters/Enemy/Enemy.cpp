// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/HpBarWidgetComponent.h"
#include "Components/AttributeComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Item/Pickable/Weapon/Weapon_Melee.h"
#include "Item/Pickups/Soul.h"

#include "Blaster/DrawDebug.h"

#include "MotionWarpingComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Actor/Projectile/Projectile.h"
#include "BlasterTypes/BlackboardKeys.h"


// AI
#include "AIController.h"
#include "AI/Navigation/NavigationTypes.h"
#include "NavigationData.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AISense_Damage.h"


// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	//ensure(PawnSensingComponent);

	InitializeDefaults();
}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();



}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyAIController = Cast<AEnemyAIController>(GetController());
	NullChecker(EnemyAIController, TEXT("EnemyAIController"), *GetName());
	NullChecker(SoulToSpawn, TEXT("SoulToSpawn"), *GetName());


	//PrimaryActorTick.TickInterval = 1.f;
	PrimaryActorTick.bCanEverTick = false;

	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
	if (WeaponClass)
	{
		Weapon = GetWorld()->SpawnActor<AWeapon_Melee>(WeaponClass);
		Weapon->ItemAttachToComponent(GetMesh(), Rules, TEXT("WeaponSocket"));

		Weapon->SetOwner(this);
		Weapon->SetInstigator(this);
	}

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("Tick"));

	//static bool isGoal = false;

	//AIController = Cast<AAIController>(GetController());
	//ensure(AIController);

	//bool isNav;
	//FNavLocation NavLocation;

	//if (isGoal)
	//{

	//	bool isNav = NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), 500.f, NavLocation);

	//	UE_LOG(LogTemp, Display, TEXT("X: %f, Y : %f, Z : %f"), NavLocation.Location.X, NavLocation.Location.Y, NavLocation.Location.Z);
	//	isGoal = false;
	//}

	//if (isNav)
	//{
	//	FAIMoveRequest MoveRequest;
	//	//MoveRequest.SetGoalActor(PatrolTarget);
	//	MoveRequest.SetGoalLocation(NavLocation.Location);
	//	MoveRequest.SetAcceptanceRadius(15.f);
	//	FNavPathSharedPtr NavPath;
	//	FPathFollowingRequestResult Result = AIController->MoveTo(MoveRequest, &NavPath);

	//	const TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
	//	for (const auto& Point : PathPoints) {
	//		const FVector& Location = Point.Location;
	//		DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 5.f);
	//	}
	//	isGoal = true;
	//}

}

//float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	UE_LOG(LogTemp, Display, TEXT("DamageCauser Owner : %s"), *DamageCauser->GetOwner()->GetName());
//	APlayerController* PlayerController = Cast<APlayerController>(EventInstigator);
//	if (PlayerController == nullptr)
//	{
//		//UE_LOG(LogTemp, Display, TEXT("We are Same"));
//		return 0.f;
//	}
//
//	float DamageResult = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
//
//	if (EventInstigator)
//	{
//		PawnDamageCauser = EventInstigator->GetPawn();
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("EventInstigator is NULL"));
//	}
//
//	if (EnemyAIController)
//	{
//		EnemyAIController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), PawnDamageCauser);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("EnemyAIController is NULL"));
//	}
//
//
//	return DamageResult;
//}

void AEnemy::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Display, TEXT("DamageCauser Owner : %s"), *DamageCauser->GetOwner()->GetName());
	APlayerController* PlayerController = Cast<APlayerController>(InstigatorController);
	if (PlayerController == nullptr)
	{
		//UE_LOG(LogTemp, Display, TEXT("We are Same"));
		return;
	}

	Super::ReceiveDamage(DamagedActor, Damage, DamageType, InstigatorController, DamageCauser);

	UAISense_Damage::ReportDamageEvent(this, this, InstigatorController->GetPawn(), Damage, DamageCauser->GetActorLocation(), GetActorLocation());

	if (InstigatorController)
	{
		PawnDamageCauser = InstigatorController->GetPawn();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EventInstigator is NULL"));
	}

	if (EnemyAIController)
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), PawnDamageCauser);
	}


}

void AEnemy::InitializeDelegates()
{
	Super::InitializeDelegates();

	//PawnSensingComponent->OnSeePawn.AddUniqueDynamic(this, &AEnemy::OnSeePawnFunc);
}

void AEnemy::InitializeDefaults()
{
	Super::InitializeDefaults();

	CharacterState = ECharacterState::ECS_EquippedOnehandedWeapon;

	//GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	if (GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = 300.f;

}

void AEnemy::IGetHit(const FVector& InHitPoint)
{
	//APlayerController* PlayerController = Cast<APlayerController>(GetInstigatorController());
	//if (PlayerController == nullptr)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("Enemy : GetInstigatorController is NULL"));
	//	return;
	//}

	//UE_LOG(LogTemp, Display, TEXT("IGetHit"));

	Super::IGetHit(InHitPoint);

	if (EnemyAIController)
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsHit"), true);
	}
}

//bool AEnemy::IIsHit()
//{
//	if (!EnemyAIController)
//	{
//		return false;
//	}
//	return EnemyAIController->GetBlackboardComponent()->GetValueAsBool(TEXT("IsHit"));
//}

void AEnemy::IAttack(FAttackEndedDelegate Delegate, const FString& AttackType)
{
	if (!IsValid(AttackMontage)) return;
	if (bDisableGameplay) return;

	OnAttackEnded = Delegate;
	//Attack();

	int32 Rand = FMath::RandRange(0, AttackMontage->CompositeSections.Num() - 1);
	//Rand = 7;
	Rand = 0;
	MulticastRandomAttack(Rand, AttackType);
}

void AEnemy::ISetAIState(EAIState InAIState)
{
	//UE_LOG(LogTemp, Display, TEXT("ISetAIState : %s"), *UEnum::GetDisplayValueAsText(InAIState).ToString());

	EnemyAIController = EnemyAIController == nullptr ? Cast<AEnemyAIController>(GetController()) : EnemyAIController.Get();
	if (EnemyAIController)
	{
		EnemyAIController->SetAIState(InAIState);
	}

	switch (InAIState)
	{
	case EAIState::EAI_Passive:
		GetCharacterMovement()->MaxWalkSpeed = 150.f;
		UE_LOG(LogTemp, Display, TEXT("ISetAIState : EAI_Passive"));
		break;
	case EAIState::EAI_Attacking:
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		break;
	case EAIState::EAI_Frozen:
		break;
	case EAIState::EAI_Investigating:
		break;
	case EAIState::EAI_Dead:
		break;
	case EAIState::EAI_MAX:
		break;
	default:
		break;
	}

}

void AEnemy::OnMontageEndedFunc(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnMontageEndedFunc(Montage, bInterrupted);

	if (!bInterrupted)
	{
		if (HitReactMontage == Montage)
		{
			if (EnemyAIController)
			{
				EnemyAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsHit"), false);
			}

			//UE_LOG(LogTemp, Display, TEXT("OnAttackEnded ExecuteIfBound"));

			OnAttackEnded.ExecuteIfBound();
		}


	}

	if (AttackMontage == Montage)
	{
		//UE_LOG(LogTemp, Display, TEXT("OnAttackEnded ExecuteIfBound"));
		OnAttackEnded.ExecuteIfBound();

	}
}

void AEnemy::UpdateMotionWarpingTransform()
{
	FName WarpTargetName = TEXT("WarpTarget");
	FName RushToTarget = TEXT("RushToTarget");

	//UE_LOG(LogTemp, Display, TEXT("UpdateMotionWarpingTransform"));
	if (EnemyAIController && EnemyAIController->GetBlackboardComponent())
	{
		AActor* TargetActor = Cast<AActor>(EnemyAIController->GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
		if (TargetActor)
		{
			//UE_LOG(LogTemp, Display, TEXT("UpdateMotionWarpingTransform"));
			FVector TargetVector = TargetActor->GetActorLocation();
			FVector ThisVector = GetActorLocation();

			FVector Direction = (TargetVector - ThisVector).GetSafeNormal();
			Direction *= 75.f;

			FVector ToVector = ThisVector + Direction;
			
			
			FVector ToVector2 = TargetVector;

			FTransform TargetTransform(ToVector);
			FTransform TargetTransform2(ToVector2);
			//TargetTransform.DebugPrint();
			if (MotionWarpingComponent)
			{
				MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(WarpTargetName, TargetTransform);

				MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(RushToTarget, TargetTransform2);
			}
		}
	}
}

void AEnemy::JumpToDestination(FVector InDestination)
{
	if (bDisableGameplay) return;

	if (!GetCharacterMovement()->IsFalling())
	{

		if (MantleMontage != nullptr && VaultMontage != nullptr)
		{
			VaultOrMantle();
		}

		if (!bCanMorV)
		{
			UE_LOG(LogTemp, Display, TEXT("VaultOrMantleFunc Failed"));
			FVector OutLaunchVelocity;
			UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, OutLaunchVelocity, GetActorLocation(), InDestination + FVector(0.f, 0.f, 150.f));
			LaunchCharacter(OutLaunchVelocity, true, true);
		}
		return;
	}

	if (bHoldingTheFlag) return;

	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		//ACharacter::Jump();

	}

}

void AEnemy::OnSeePawnFunc(APawn* Pawn)
{
	//UE_LOG(LogTemp, Display, TEXT("%s Saw the %s"), *GetName(), *Pawn->GetName());

	if (EnemyAIController)
	{
		if (!Pawn->InputEnabled())
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), nullptr);
			return;
		}

		EnemyAIController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Pawn);
	}
}

void AEnemy::SetDead()
{
	Super::SetDead();

	if (EnemyAIController)
	{
		UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(EnemyAIController->GetBrainComponent());
		BehaviorTreeComponent->StopTree();
	}

	if (SoulToSpawn)
	{
		ASoul* SpawnedSoul = GetWorld()->SpawnActorDeferred<ASoul>(SoulToSpawn, FTransform(GetActorLocation() + GetActorUpVector() * 70.f));
		SpawnedSoul->SetOwner(this);
		SpawnedSoul->SetInstigator(this);
		SpawnedSoul->SetSoul(100.f);
		SpawnedSoul->FinishSpawning(GetActorTransform());
	}
}

void AEnemy::SpawnMagic(FName SocketToSpawn)
{
	if (EnemyAIController == nullptr) return;

	APawn* InstigatorPawn = EnemyAIController->GetPawn();
	const USkeletalMeshSocket* MuzzleFlashSocket = GetMesh()->GetSocketByName(SocketToSpawn);
	UWorld* World = GetWorld();

	AActor* TargetActor = Cast<AActor>(EnemyAIController->GetBlackboardComponent()->GetValueAsObject(TARGET_ACTOR));
	if (TargetActor == nullptr) return;

	if (MuzzleFlashSocket && World)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetMesh());
		FVector ToTarget = TargetActor->GetActorLocation() - SocketTransform.GetLocation();
		FVector ToTargetNormal = ToTarget.GetSafeNormal();
		FRotator TargetRotation = ToTarget.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = InstigatorPawn;
		SpawnParams.Instigator = InstigatorPawn;

		AProjectile* SpawnedProjectile = nullptr;

		if (HasAuthority() && ProjectileMagicClass)
		{
			SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileMagicClass, SocketTransform.GetLocation() + ToTargetNormal * 50.f, TargetRotation, SpawnParams);
			//SpawnedProjectile = World->SpawnActorDeferred<AProjectile>(ProjectileMagic, SocketTransform);

			SpawnedProjectile->bUseServerSideRewind = false;
			SpawnedProjectile->Damage = ProjectileMagicDamage;

		}

	}
}

bool AEnemy::CheckParryFunc(AActor* OtherActor)
{
	//UE_LOG(LogTemp, Display, TEXT("CheckParryFunc"));
	//UE_LOG(LogTemp, Warning, TEXT("CheckParryFunc , this : %s, Other : %s"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString(), *UEnum::GetDisplayValueAsText(OtherActor->GetLocalRole()).ToString());

	if (GetActorForwardVector().Dot(OtherActor->GetActorForwardVector()) < -0.5f) // 전방 기준 45도 이내일때만 발동
	{
		//UE_LOG(LogTemp, Warning, TEXT("Blocking"));
		DisableHitCapsulesCollision();
		CombatState = ECombatState::ECS_Parried;
		//if (EnemyAIController) EnemyAIController->StopAI();

		if (ParryEffect)
		{
			FTransform ParryEffectTransform(GetActorRotation().Quaternion(), GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParryEffect, ParryEffectTransform);
		}

		float StunTime = ShortStunTime;

		if (GetAttributeComponent()->GetCurrentParryGauge() + 1.f >= GetAttributeComponent()->GetMaxParryGauge())
		{
			//UE_LOG(LogTemp, Display, TEXT("HardParried"));

			if (StunMontage) PlayMontage(GetMesh()->GetAnimInstance(), StunMontage, TEXT("Stun"), -1);

			StunTime = LongStunTime;
			GetAttributeComponent()->SetCurrentParryGauge(0.f);
			GetAttributeComponent()->OnParryGaugeChanged.Broadcast(0.f, GetAttributeComponent()->GetMaxParryGauge());
		}
		else
		{

			if (ParriedMontage) PlayMontage(GetMesh()->GetAnimInstance(), ParriedMontage, TEXT("Parried"), -1);
		}

		if (EnemyAIController) EnemyAIController->GetBlackboardComponent()->SetValueAsEnum(E_COMBAT_STATE, (int)CombatState);

		//UE_LOG(LogTemp, Display, TEXT("Time : %f"), StunTime);

		FTimerHandle AITimer;
		GetWorldTimerManager().SetTimer(AITimer, FTimerDelegate::CreateLambda([&]()
			{
				//UE_LOG(LogTemp, Display, TEXT("End, Stun Time : %f"), StunTime);
				if (StunMontage) StopAnimMontage(StunMontage);
				CombatState = ECombatState::ECS_Unoccupied;
				if (EnemyAIController) EnemyAIController->GetBlackboardComponent()->SetValueAsEnum(E_COMBAT_STATE, (int)CombatState);
				//if (EnemyAIController) EnemyAIController->RunAI();
			}), StunTime, false);


		return true;
	}



	return false;
}