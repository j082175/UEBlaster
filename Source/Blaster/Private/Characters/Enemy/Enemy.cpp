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
#include "Components/ObjectPoolComponent.h"
#include "GameState/BlasterGameState.h"
#include "Components/InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// AI
#include "AIController.h"
#include "AI/Navigation/NavigationTypes.h"
#include "NavigationData.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Actor/SplineActor.h"

#include "Interfaces/OverlapItemInterface.h"

#include "Blaster.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	//ensure(PawnSensingComponent);

	InitializeDefaults();
}

void AEnemy::PostLoad()
{
	Super::PostLoad();


	GetMesh()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();



}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickInterval(0.1f);

	BaseAIController = Cast<ABaseAIController>(GetController());
	NullChecker(BaseAIController, TEXT("BaseAIController"), *GetName());
	NullChecker(SoulToSpawn, TEXT("SoulToSpawn"), *GetName());


	//PrimaryActorTick.TickInterval = 1.f;
	PrimaryActorTick.bCanEverTick = false;

	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
	if (WeaponClass)
	{
		Weapon = GetWorld()->SpawnActor<AWeapon_Melee>(WeaponClass);
		Weapon->SetReplicates(false);
		Weapon->ItemAttachToComponent(GetMesh(), Rules, SOCKET_WEAPON);
		Weapon->SetOwner(this);
		Weapon->SetInstigator(this);

		EquipWeapon(Weapon);
	}

	ACharacterBase* OwingActor = Cast<ACharacterBase>(GetOwner());
	if (OwingActor)
	{
		ISetTeam(OwingActor->IGetTeam());
		SetTeamColor(OwingActor->IGetTeam());
	}

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//CheckVisibilityAndSetupAnim();

	//UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation(), CheckAIOnOffComponent->GetScaledSphereRadius());

	//UE_LOG(LogTemp, Display, TEXT("Enemy Tick"));

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

void AEnemy::UnPossessed()
{
	Super::UnPossessed();

	//UE_LOG(LogTemp, Error, TEXT("Enemy UnPossessed"));
}

void AEnemy::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	if (AAIController* AIOwner = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BC = AIOwner->GetBlackboardComponent())
		{
			BC->SetValueAsObject(OWING_ACTOR, NewOwner);
		}
	}
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

	Super::ReceiveDamage(DamagedActor, Damage, DamageType, InstigatorController, DamageCauser);

	if (InstigatorController)
	{
		PawnDamageCauser = InstigatorController->GetPawn();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EventInstigator is NULL"));
	}

	//if (BaseAIController)
	//{
	//	BaseAIController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), PawnDamageCauser);
	//}


}

void AEnemy::Destroyed()
{
	if (Weapon)
	{
		Weapon->Destroy();
	}



	SetIsActive(false);
}

void AEnemy::SetIsActive(bool InIsActive)
{
	Super::SetIsActive(InIsActive);

	UE_LOG(LogTemp, Display, TEXT("SetIsActive : %d"), InIsActive);

	if (InventoryComponent->EquippedWeapon)
	{
		//UE_LOG(LogTemp, Display, TEXT("hidden EquippedWeapon"));
		InventoryComponent->EquippedWeapon->SetActorHiddenInGame(!InIsActive);
	}

	if (InIsActive)
	{
		if (BaseAIController)
		{
			UE_LOG(LogTemp, Display, TEXT("RunAI"));
			BaseAIController->RunAI();
			BaseAIController->PrimaryActorTick.bCanEverTick = true;
			BaseAIController->SetActorTickEnabled(true);
		}
	}
	else
	{
		if (BaseAIController)
		{
			UE_LOG(LogTemp, Display, TEXT("StopAI"));

			BaseAIController->StopAI();
			BaseAIController->PrimaryActorTick.bCanEverTick = true;
			BaseAIController->SetActorTickEnabled(false);
		}
		OnSpawnedEnemyDisabled.ExecuteIfBound(this);
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

	CharacterState = ECharacterState::EquippedOnehandedWeapon;

	//GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);
		GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_NavWalking;
		GetCharacterMovement()->bUpdateOnlyIfRendered = true;
	}

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	GetMesh()->bEnableUpdateRateOptimizations = true;
}


void AEnemy::IGetHit(const FVector& InHitPoint, const FHitResult& InHitResult)
{
	Super::IGetHit(InHitPoint, InHitResult);

	float Rand = FMath::RandRange(0.f, 1.f);
	if (Rand < 0.1f)
	{
		PlayHitReactMontage(InHitPoint);
	}

	if (BaseAIController)
	{
		if (UBlackboardComponent* BC = BaseAIController->GetBlackboardComponent())
		{
			BC->SetValueAsBool(TEXT("IsHit"), true);
		}
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

	BaseAIController = BaseAIController == nullptr ? Cast<ABaseAIController>(GetController()) : BaseAIController.Get();
	if (BaseAIController)
	{
		BaseAIController->SetAIState(InAIState);
	}

	switch (InAIState)
	{
	case EAIState::Passive:
		SetAiming(true);
		//UE_LOG(LogTemp, Display, TEXT("ISetAIState : Passive"));
		break;
	case EAIState::Attacking:
		SetAiming(true);
		break;
	case EAIState::Frozen:
		break;
	case EAIState::Investigating:
		SetAiming(false);

		break;
	case EAIState::Dead:
		break;
	case EAIState::MAX:
		break;
	default:
		break;
	}

}

ASplineActor* AEnemy::IGetPatrolRoute()
{
	return PatrolTarget;
}

void AEnemy::ISetPatrolRoute(ASplineActor* InSplineActor)
{
	PatrolTarget = InSplineActor;
}

void AEnemy::OnMontageEndedFunc(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnMontageEndedFunc(Montage, bInterrupted);



	if (!bInterrupted)
	{
		if (HitReactMontage == Montage)
		{
			if (BaseAIController)
			{
				BaseAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsHit"), false);
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
	if (BaseAIController && BaseAIController->GetBlackboardComponent())
	{
		AActor* TargetActor = Cast<AActor>(BaseAIController->GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
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

	if (BaseAIController)
	{
		if (!Pawn->InputEnabled())
		{
			BaseAIController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), nullptr);
			return;
		}

		BaseAIController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Pawn);
	}
}

void AEnemy::SetDead()
{
	Super::SetDead();

	if (BaseAIController)
	{
		UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BaseAIController->GetBrainComponent());
		BehaviorTreeComponent->StopTree();
	}

	if (SoulToSpawn)
	{
		ASoul* SpawnedSoul = GetWorld()->SpawnActorDeferred<ASoul>(SoulToSpawn, FTransform(GetActorLocation() + GetActorUpVector() * 70.f));
		SpawnedSoul->SetOwner(this);
		SpawnedSoul->SetInstigator(this);
		SpawnedSoul->SetSoul(10.f);
		SpawnedSoul->FinishSpawning(GetActorTransform());
	}
}

void AEnemy::SpawnMagic(FName SocketToSpawn)
{
	if (BaseAIController == nullptr) return;

	APawn* InstigatorPawn = BaseAIController->GetPawn();
	const USkeletalMeshSocket* MuzzleFlashSocket = GetMesh()->GetSocketByName(SocketToSpawn);
	UWorld* World = GetWorld();

	AActor* TargetActor = Cast<AActor>(BaseAIController->GetBlackboardComponent()->GetValueAsObject(TARGET_ACTOR));
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
			FTransform T(TargetRotation, SocketTransform.GetLocation() + ToTargetNormal * 50.f);
			//SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileMagicClass, SocketTransform.GetLocation() + ToTargetNormal * 50.f, TargetRotation, SpawnParams);

			SpawnedProjectile = Cast<AProjectile>(GetWorld()->GetGameState<ABlasterGameState>()->GetComponentByClass<UObjectPoolComponent>()->GetSpawnedObject(T, ProjectileMagicClass));
			SpawnedProjectile->SetProjectileMovementVelocity(ToTarget);
			SpawnedProjectile->SetOwner(InstigatorPawn);
			SpawnedProjectile->SetInstigator(InstigatorPawn);
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
		CombatState = ECombatState::Parried;
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

		if (BaseAIController) BaseAIController->GetBlackboardComponent()->SetValueAsEnum(E_COMBAT_STATE, (int)CombatState);

		//UE_LOG(LogTemp, Display, TEXT("Time : %f"), StunTime);

		FTimerHandle AITimer;
		GetWorldTimerManager().SetTimer(AITimer, FTimerDelegate::CreateLambda([&]()
			{
				//UE_LOG(LogTemp, Display, TEXT("End, Stun Time : %f"), StunTime);
				if (StunMontage) StopAnimMontage(StunMontage);
				CombatState = ECombatState::Unoccupied;
				if (BaseAIController) BaseAIController->GetBlackboardComponent()->SetValueAsEnum(E_COMBAT_STATE, (int)CombatState);
				//if (EnemyAIController) EnemyAIController->RunAI();
			}), StunTime, false);


		return true;
	}



	return false;
}

void AEnemy::CheckVisibilityAndSetupAnim()
{
	//WasRecentlyRendered(0.5f)



	if (GetWorld()->GetFirstPlayerController()->LineOfSightTo(this))
	{
		GetMesh()->ToggleVisibility(true);
		GetMesh()->SetComponentTickEnabled(true);
	}
	else
	{
		GetMesh()->ToggleVisibility(false);
		GetMesh()->SetComponentTickEnabled(false);
	}
}
