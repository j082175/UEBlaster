// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickups/Pickup.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Item/Pickable/Weapon/WeaponTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Interfaces/OverlapItemInterface.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;

	//bReplicates = true;


	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	//OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphereComponent"));
	//OverlapSphere->SetupAttachment(RootComponent);
	//OverlapSphere->SetSphereRadius(150.f);
	//OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//OverlapSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	//OverlapSphere->AddLocalOffset(FVector(0.f, 0.f, 85.f));

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(RootComponent);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_PURPLE);
	PickupMesh->MarkRenderStateDirty(); // CustomDepth 설정 가능하게 허용
	PickupMesh->SetRenderCustomDepth(true);
	PickupMesh->SetRelativeScale3D(FVector(5.f));

	PersistEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PersistEffectComponent"));
	PersistEffectComponent->SetupAttachment(RootComponent);

}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PickupMesh)
	{
		PickupMesh->AddWorldRotation(FRotator(0.f, BaseTurnRate * DeltaTime, 0.f));
	}

	UE_LOG(LogTemp, Display, TEXT("Pickup Tick"));
}

void APickup::Destroyed()
{
	Super::Destroyed();

	//UE_LOG(LogTemp, Display, TEXT("Destroyed"));
	if (PickupSound) UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
	if (PickupEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, GetActorLocation(), GetActorRotation());

}

void APickup::OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}

	//AddOnScreenDebugMessage(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//UE_LOG(LogTemp, Display, TEXT("OnCapsuleBeginOverlapFunc"));

	// interface
	IOverlapItemInterface* IOverlappedActor = Cast<IOverlapItemInterface>(OtherActor);
	if (IOverlappedActor && PickupWidget)
	{
		//UE_LOG(LogTemp, Display, TEXT("IOverlappedActor->IGetItem"));
		IOverlappedActor->IGetItem(this);
	}

	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
	}
}

void APickup::OnCapsuleEndOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this)
	{
		return;
	}

	// interface
	IOverlapItemInterface* IOverlappedActor = Cast<IOverlapItemInterface>(OtherActor);
	if (IOverlappedActor && PickupWidget)
	{
		IOverlappedActor->IGetItem(nullptr);
	}
}

void APickup::InitializeDelegates()
{
	Super::InitializeDelegates();

	if (OverlapCapsule)
	{
		OverlapCapsule->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCapsuleBeginOverlapFunc);
		OverlapCapsule->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCapsuleEndOverlapFunc);
	}
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	//if (HasAuthority())
	//{
	//	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCapsuleBeginOverlapFunc);
	//}
}

//void APickup::OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//}

