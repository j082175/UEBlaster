// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"

// Debug
#include "Blaster/DrawDebug.h"

// Components
#include "Components/WidgetComponent.h"

// Collisions
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

// Interfaces
#include "Interfaces/OverlapItemInterface.h"

// Niagara
#include "NiagaraComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Sound/SoundCue.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;


	OverlapCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	ensure(OverlapCapsule);

	//OverlapCapsule->AddLocalOffset(FVector(0.f, 0.f, 85.f));

	//PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	//PickupMesh->SetupAttachment(OverlapCapsule);


	//PickupMesh->SetRelativeScale3D(FVector(5.f));


	//RootComponent = PickupMesh;

	//PersistEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PersistEffectComponent"));

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	RootComponent = OverlapCapsule;
	//PickupMesh->SetupAttachment(RootComponent);
	//PersistEffectComponent->SetupAttachment(RootComponent);

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Megascans/3D_Assets/Sandstone_Boulder_vecqdebqx/S_Sandstone_Boulder_vecqdebqx_lod3_Var1.S_Sandstone_Boulder_vecqdebqx_lod3_Var1'"));
	//ensure(StaticMeshRef.Object);
	//if (ItemMesh->GetStaticMesh() == nullptr)
	//{
	//	ItemMesh->SetStaticMesh(StaticMeshRef.Object);
	//}

	InitializeCollisionStates();
	InitializeDefaults();

}

void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//UE_LOG(LogTemp, Display, TEXT("Item PostInitializeComponents"));

	InitializeCollisionStates();
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Display, TEXT("Item BeginPlay"));

	NullChecker(OverlapCapsule, TEXT("CapsuleComponent"), GetName());
	//NullChecker(PickupMesh, TEXT("ItemMesh"), GetName());
	NullChecker(PickupSound, TEXT("SoundBase"), GetName());

	//GEngine->AddOnScreenDebugMessage()
	if (HasAuthority()) InitializeDelegates();
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Moving(DeltaTime);
}

//// When this function called, it means that this class is going to be attached to another actor's mesh.
//void AItem::ItemAttachToComponent(USceneComponent* SceneComponent, FAttachmentTransformRules& AttachmentRules, FName InSocketName)
//{
//	ItemState = EItemState::EIS_Equipped;
//
//	if (OverlapCapsule) OverlapCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	if (PickupMesh)
//	{
//		if (!PickupMesh->AttachToComponent(SceneComponent, AttachmentRules, InSocketName))
//		{
//			UE_LOG(LogTemp, Error, TEXT("AttachToComponent Failed"));
//		}
//	}
//
//	if (SceneComponent && SceneComponent->GetOwner())
//	{
//		SetOwner(SceneComponent->GetOwner());
//		
//		APawn* InstigatorPawn = Cast<APawn>(SceneComponent->GetOwner());
//		SetInstigator(InstigatorPawn);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Item : No Owner"));
//	}
//
//}
//
//void AItem::ItemDetachToComponent(FDetachmentTransformRules& DetachmentRules)
//{
//	ItemState = EItemState::EIS_Hovering;
//
//	if (OverlapCapsule) OverlapCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
//	if (PickupMesh) PickupMesh->DetachFromComponent(DetachmentRules);
//
//	SetOwner(nullptr);
//	SetInstigator(nullptr);
//}

void AItem::ShowPickupWidget(bool bShowWidget)
{
	//UE_LOG(LogTemp, Display, TEXT("ShowPickupWidget : %d"), bShowWidget);
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}

void AItem::InitializeCollisionStates()
{
	//if (PickupMesh) PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (OverlapCapsule)
	{
		OverlapCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		OverlapCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
		OverlapCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

		//CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
		//CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	}
}

void AItem::InitializeDelegates()
{
	//if (OverlapCapsule)
	//{
	//	OverlapCapsule->OnComponentBeginOverlap.AddUniqueDynamic(this, &AItem::OnCapsuleBeginOverlapFunc);
	//	OverlapCapsule->OnComponentEndOverlap.AddUniqueDynamic(this, &AItem::OnCapsuleEndOverlapFunc);
	//}
}

void AItem::InitializeDefaults()
{
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	OverlapCapsule->SetCapsuleRadius(50.f);
	OverlapCapsule->SetCapsuleHalfHeight(50.f);
	OverlapCapsule->SetRelativeLocation(FVector(0.f, 20.f, 10.f));


}

//void AItem::OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor == this)
//	{
//		return;
//	}
//
//	//AddOnScreenDebugMessage(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
//
//
//	// interface
//	IOverlapItemInterface* IOverlappedActor = Cast<IOverlapItemInterface>(OtherActor);
//	if (IOverlappedActor && PickupWidget)
//	{
//		//UE_LOG(LogTemp, Display, TEXT("IOverlappedActor->IGetItem"));
//		IOverlappedActor->IGetItem(this);
//	}
//
//	if (PickupSound)
//	{
//		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
//	}
//}
//
//void AItem::OnCapsuleEndOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (OtherActor == this)
//	{
//		return;
//	}
//
//	// interface
//	IOverlapItemInterface* IOverlappedActor = Cast<IOverlapItemInterface>(OtherActor);
//	if (IOverlappedActor && PickupWidget)
//	{
//		IOverlappedActor->IGetItem(nullptr);
//	}
//}

void AItem::Moving(float DeltaTime)
{
	if (ItemState == EItemState::EIS_Hovering)
	{
		Speed += DeltaTime * Wavelength;

		float Width = Amplitude * FMath::Sin(Speed);
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Blue, FString::Printf(TEXT("Width : %f"), Width));
		//}

		AddActorLocalOffset(FVector(0.f, 0.f, Width));

		FRotator Rotation = FRotator(0.f, DeltaTime * 200.f, 0.f);
		AddActorWorldRotation(Rotation);
	}
}
