// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SplineActor.h"
#include "Components/SplineComponent.h"
#include "Dom/JsonObject.h"
#include "Components/SphereComponent.h"
#include "Interfaces/GetPatrolRouteInterface.h"

// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SetRootComponent(SplineComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);

	SphereComponent->SetSphereRadius(100.f);
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();
	
	FInterpCurveVector ICVector = SplineComponent->GetSplinePointsPosition();

	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereBeginOverlapFunc);
}

// Called every frame
void ASplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UE_LOG(LogTemp, Warning, TEXT("SplineActor Tick"));
}

void ASplineActor::IncrementPatrolRoute()
{
	//UE_LOG(LogTemp, Display, TEXT("PatrolIndex : %d"), PatrolIndex);
	//UE_LOG(LogTemp, Display, TEXT("GetNumberOfSplinePoints : %d"), SplineComponent->GetNumberOfSplinePoints());

	if (PatrolIndex == SplineComponent->GetNumberOfSplinePoints() - 1)
	{
		Direction = -1;
	}
	else if (PatrolIndex == 0)
	{
		Direction = 1;
	}

	PatrolIndex += Direction;
}

void ASplineActor::OnSphereBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}

	IGetPatrolRouteInterface* PatrolRoutePawn = Cast<IGetPatrolRouteInterface>(OtherActor);
	if (PatrolRoutePawn)
	{
		PatrolRoutePawn->ISetPatrolRoute(this);
	}
}

FVector ASplineActor::GetSplinePointAsWorldPosition()
{
	return SplineComponent->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}

