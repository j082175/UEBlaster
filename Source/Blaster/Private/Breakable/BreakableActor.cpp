// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Item/Pickups/Treasure.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"

#include "Blaster/Blaster.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	ensure(GeometryCollectionComponent);
	RootComponent = GeometryCollectionComponent;
	GeometryCollectionComponent->SetNotifyBreaks(true);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	ensure(CapsuleComponent);

	CapsuleComponent->SetupAttachment(RootComponent);

	InitializeCollisionStates();
}

void ABreakableActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeDelegates();
}

void ABreakableActor::InitializeCollisionStates()
{
	GeometryCollectionComponent->SetGenerateOverlapEvents(true);
	GeometryCollectionComponent->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_CanDamagedByWeapon, ECollisionResponse::ECR_Block);

	GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_HitCapsule, ECollisionResponse::ECR_Overlap);
	//GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);

	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);

}

void ABreakableActor::InitializeDelegates()
{
	GeometryCollectionComponent->OnChaosBreakEvent.AddUniqueDynamic(this, &ABreakableActor::OnChaosBreakEventFunc);
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABreakableActor::IGetHit(const FVector& HitPoint)
{
	//UE_LOG(LogTemp, Display, TEXT("BreakableActor : isHit : %d"), bIsHit);
	if (Treasures.Num() > 0 && bIsHit == 0)
	{
		//UE_LOG(LogTemp, Display, TEXT("Spawned!"));
		UGameplayStatics::PlaySoundAtLocation(this, BreakSound, GetActorLocation());
		FTransform Transform = FTransform(GetActorLocation() + GetActorUpVector() * 70.f);
		int32 Random = FMath::RandRange(0, Treasures.Num() - 1);
		ATreasure* SpawnedTreasure = GetWorld()->SpawnActor<ATreasure>(Treasures[Random].Get(), Transform);

		if (SpawnedTreasure)
		{
			SpawnedTreasure->GetAreaCapsule()->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
			//SpawnedTreasure->FinishSpawning(Transform);
			//DrawDebugSphere(GetWorld(), SpawnedTreasure->GetActorLocation(), 20.f, 12, FColor::Red, false, 2.f);
		}
		//bIsHit = true;
	}
}

void ABreakableActor::OnChaosBreakEventFunc(const FChaosBreakEvent& BreakEvent)
{
	

	bIsHit = true;
	//GeometryCollectionComponent->SetGenerateOverlapEvents(false);
	//GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block); // 이거 작동이안됨 ㅅㅂ;
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetLifeSpan(LifeSpan);

	//UE_LOG(LogTemp, Display, TEXT("Chaos"));
	//UE_LOG(LogTemp, Display, TEXT("Chaos isHit : %d"), bIsHit);
}

