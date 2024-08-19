// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MantleVaultComponent.h"
#include "MotionWarpingComponent.h"
#include "Characters/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "MotionWarpingComponent.h"

#include "Characters/CharacterBase.h"

// Sets default values for this component's properties
UMantleVaultComponent::UMantleVaultComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMantleVaultComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Owner = Cast<ACharacterBase>(GetOwner());
}


// Called every frame
void UMantleVaultComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UMantleVaultComponent::GetInitialObjectLocation_C()
{
	//UE_LOG(LogTemp, Display, TEXT("GetInitialObjectLocation_C"));

	//UE_LOG(LogTemp, Warning, TEXT("Height %f, %f, %f"), ObjectHeightVector.X, ObjectHeightVector.Y, ObjectHeightVector.Z);
	//UE_LOG(LogTemp, Warning, TEXT("Depth %f, %f, %f"), ObjectDepthVector.X, ObjectDepthVector.Y, ObjectDepthVector.Z);
	//UE_LOG(LogTemp, Warning, TEXT("Landing %f, %f, %f"), LandingVector.X, LandingVector.Y, LandingVector.Z);


	if (Owner == nullptr) return false;

	ResetVector();

	FVector Start = Owner->GetActorLocation();
	Start.Z -= 10.f;

	FVector End = Start + Owner->GetActorForwardVector() * DetectRange;

	ETraceTypeQuery Visibility = ETraceTypeQuery::TraceTypeQuery1;
	ETraceTypeQuery CanMorV = ETraceTypeQuery::TraceTypeQuery4;
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;

	EDrawDebugTrace::Type DrawDebug = bDrawDetectPoint != true ? EDrawDebugTrace::None : EDrawDebugTrace::ForDuration;
	bool IsTrace = UKismetSystemLibrary::SphereTraceSingle(Owner, Start, End, 10.f, CanMorV, false, ActorsToIgnore, DrawDebug, HitResult, true);
	if (IsTrace)
	{
		return GetObjectDimensions_C(HitResult.ImpactPoint);
	}
	else
	{
		return false;
	}



}

bool UMantleVaultComponent::GetObjectDimensions_C(const FVector& InInitialImpactLocation)
{
	size_t Loop = 50;
	int32 IndexDistance = 10.f;

	for (size_t HeightIndex = 1; HeightIndex < Loop; HeightIndex++)
	{
		ETraceTypeQuery Visibility = ETraceTypeQuery::TraceTypeQuery1;
		ETraceTypeQuery CanMorV = ETraceTypeQuery::TraceTypeQuery4;

		TArray<AActor*> ActorsToIgnore;
		FHitResult HitResult;

		FVector Start = InInitialImpactLocation;
		Start.Z -= 30.f;
		Start.Z += HeightIndex * IndexDistance;

		FVector End = Start + Owner->GetActorForwardVector() * 75.f;

		EDrawDebugTrace::Type DrawDebug = bDrawHeight != true ? EDrawDebugTrace::None : EDrawDebugTrace::ForDuration;
		bool IsTrace = UKismetSystemLibrary::LineTraceSingle(Owner, Start, End, CanMorV, false, ActorsToIgnore, DrawDebug, HitResult, true);
		if (IsTrace)
		{
			ObjectHeightVector = HitResult.ImpactPoint;
		}
		else
		{
			break;
		}
	}

	for (size_t DepthIndex = 1; DepthIndex < Loop; DepthIndex++)
	{
		ETraceTypeQuery Visibility = ETraceTypeQuery::TraceTypeQuery1;
		ETraceTypeQuery CanMorV = ETraceTypeQuery::TraceTypeQuery4;
		TArray<AActor*> ActorsToIgnore;
		FHitResult HitResult;

		FVector DepthLocation = Owner->GetActorForwardVector() * DepthIndex * IndexDistance;

		FVector Start = ObjectHeightVector + DepthLocation;

		FVector End = Start;
		End.Z += 30.f;

		EDrawDebugTrace::Type DrawDebug = bDrawDepth != true ? EDrawDebugTrace::None : EDrawDebugTrace::ForDuration;
		bool IsTrace = UKismetSystemLibrary::LineTraceSingle(Owner, Start, End, CanMorV, false, ActorsToIgnore, DrawDebug, HitResult, true);
		if (IsTrace)
		{
			ObjectDepthVector = HitResult.ImpactPoint;
		}
		else
		{
			break;
		}
	}

	{
		ETraceTypeQuery Visibility = ETraceTypeQuery::TraceTypeQuery1;
		ETraceTypeQuery CanMorV = ETraceTypeQuery::TraceTypeQuery4;
		TArray<AActor*> ActorsToIgnore;
		FHitResult HitResult;

		FVector Start = Owner->GetActorForwardVector() * 150.f + ObjectDepthVector;

		FVector End = Start;
		End.Z -= 150.f;

		EDrawDebugTrace::Type DrawDebug = bDrawLandingPoint != true ? EDrawDebugTrace::None : EDrawDebugTrace::ForDuration;
		bool IsTrace = UKismetSystemLibrary::LineTraceSingle(Owner, Start, End, CanMorV, false, ActorsToIgnore, DrawDebug, HitResult, true);

		ObjectHeightVector.Z += IndexDistance;
		ObjectDepthVector.Z += IndexDistance;


		//UE_LOG(LogTemp, Warning, TEXT("Depth %f"), (Owner->GetActorLocation() - ObjectDepthVector).Size2D());
		//UE_LOG(LogTemp, Warning, TEXT("Height %f"), ObjectHeightVector.Z - Owner->GetActorLocation().Z);


		if (IsTrace)
		{
			LandingVector = HitResult.ImpactPoint;

			UKismetSystemLibrary::DrawDebugSphere(Owner, ObjectHeightVector, 20.f, 12, FLinearColor::White, 5.f, 3.f);
			UKismetSystemLibrary::DrawDebugSphere(Owner, ObjectDepthVector, 20.f, 12, FLinearColor::Red, 5.f, 3.f);
			UKismetSystemLibrary::DrawDebugSphere(Owner, LandingVector, 20.f, 12, FLinearColor::Blue, 5.f, 3.f);




			if (ValutHeightThreshold < ObjectHeightVector.Z - Owner->GetActorLocation().Z)
			{
				goto Label1;
			}
			else if (DepthThreshold < (Owner->GetActorLocation() - ObjectDepthVector).Size2D())
			{
				return false;
			}
			else
			{
				SetMotionWarpingPositions_Vault();

			}

		}
		else
		{
			LandingVector = HitResult.TraceEnd;
			LandingVector.Z += 50.f;

			UKismetSystemLibrary::DrawDebugSphere(Owner, ObjectHeightVector, 20.f, 12, FLinearColor::White, 5.f, 3.f);
			UKismetSystemLibrary::DrawDebugSphere(Owner, ObjectDepthVector, 20.f, 12, FLinearColor::Red, 5.f, 3.f);
			UKismetSystemLibrary::DrawDebugSphere(Owner, LandingVector, 20.f, 12, FLinearColor::Yellow, 5.f, 3.f);

		Label1:
			if (HeightThreshold < (ObjectHeightVector.Z - Owner->GetActorLocation().Z)) return false;
			SetMotionWarpingPositions_Mantle();
		}
	}

	return true;
}

void UMantleVaultComponent::SetMotionWarpingPositions_Vault()
{
	FMotionWarpingTarget Target;
	Target.Name = TEXT("Warp1");
	Target.Location = FMath::Lerp(ObjectHeightVector, ObjectDepthVector, 0.5f);
	//Target.Rotation = Owner->GetActorRotation();
	Target.Rotation = (ObjectDepthVector - ObjectHeightVector).Rotation();

	Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTarget(Target);


	Target.Name = TEXT("Warp2");
	Target.Location = LandingVector;
	Target.Rotation = Owner->GetActorRotation();

	Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTarget(Target);

	VaultOverObject();
}

void UMantleVaultComponent::SetMotionWarpingPositions_Mantle()
{
	//UE_LOG(LogTemp, Display, TEXT("SetMotionWarpingPositions_Mantle"));
	FMotionWarpingTarget Target;
	Target.Name = TEXT("Warp1");
	Target.Location = ObjectHeightVector;
	Target.Location.Z -= 130.f;

	Target.Location.X = ObjectHeightVector.X - Owner->GetActorForwardVector().X * 20.f;
	Target.Location.Y = ObjectHeightVector.Y - Owner->GetActorForwardVector().Y * 20.f;

	Target.Rotation = Owner->GetActorRotation();

	Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTarget(Target);


	Target.Name = TEXT("Warp2");
	Target.Location = ObjectHeightVector;
	Target.Location.X = ObjectHeightVector.X + Owner->GetActorForwardVector().X * 30.f;
	Target.Location.Y = ObjectHeightVector.Y + Owner->GetActorForwardVector().Y * 30.f;

	Target.Rotation = Owner->GetActorRotation();

	Owner->GetMotionWarpingComponent()->AddOrUpdateWarpTarget(Target);

	MantleObject();
}

void UMantleVaultComponent::VaultOverObject()
{

	Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	Owner->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	int32 RandomSection = FMath::RandRange(0, 1);
	FName Str = FName(FString::Printf(TEXT("IdleVault%d"), RandomSection));
	Owner->PlayVaultMontage(Str);

}

void UMantleVaultComponent::MantleObject()
{
	//UE_LOG(LogTemp, Display, TEXT("%f"), (ObjectHeightVector.Z - Owner->GetActorLocation().Z));


	Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	Owner->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	int32 RandomSection = 0;
	FName Str = FName(FString::Printf(TEXT("Mantle%d"), RandomSection));
	Owner->PlayMantleMontage(Str);

}

void UMantleVaultComponent::ResetVector()
{
	ObjectHeightVector = FVector(0.f);
	ObjectDepthVector = FVector(0.f);
	LandingVector = FVector(0.f);
}

