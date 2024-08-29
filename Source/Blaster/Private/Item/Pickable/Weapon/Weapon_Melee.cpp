#include "Item/Pickable/Weapon/Weapon_Melee.h"

// Components
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"

// Kismet
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Interfaces
#include "Interfaces/HitInterface.h"
#include "Interfaces/CanParryInterface.h"

// FieldSystem
#include "Field/FieldSystemComponent.h"

#include "Blaster/DrawDebug.h"
#include "Blaster/Blaster.h"

// Debug
#include "Engine/DamageEvents.h"

AWeapon_Melee::AWeapon_Melee()
{
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Megascans/3D_Assets/Sword_-_Scabbard_uh2tagpfa/S_Sword_-_Scabbard_uh2tagpfa_lod3_Var1.S_Sword_-_Scabbard_uh2tagpfa_lod3_Var1'"));
	//ensure(StaticMeshRef.Object);
	//ItemMesh->SetStaticMesh(StaticMeshRef.Object);



	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	ensure(BoxComponent);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	ensure(BoxTraceStart);

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	ensure(BoxTraceEnd);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));


	SetRootComponent(PickupMesh);
	OverlapCapsule->SetupAttachment(RootComponent);
	//PersistEffectComponent->SetupAttachment(RootComponent);
	FieldSystemComponent->SetupAttachment(RootComponent);
	BoxComponent->SetupAttachment(RootComponent);
	BoxTraceStart->SetupAttachment(RootComponent);
	BoxTraceEnd->SetupAttachment(RootComponent);

	InitializeCollisionStates();
}

void AWeapon_Melee::BeginPlay()
{
	Super::BeginPlay();
	
	NullChecker(BoxComponent, TEXT("BoxComponent"), GetName());
	NullChecker(BoxTraceStart, TEXT("BoxTraceStart"), GetName());
	NullChecker(BoxTraceEnd, TEXT("BoxTraceEnd"), GetName());

	InitializeDelegates();

	
}

void AWeapon_Melee::ItemAttachToComponent(USceneComponent* SceneComponent, FAttachmentTransformRules& AttachmentRules, FName InSocketName)
{
	Super::ItemAttachToComponent(SceneComponent, AttachmentRules, InSocketName);

	if (PickupMesh)
	{
		if (!PickupMesh->AttachToComponent(SceneComponent, AttachmentRules, InSocketName))
		{
			UE_LOG(LogTemp, Error, TEXT("AttachToComponent Failed"));
		}
	}

	UE_LOG(LogTemp, Display, TEXT("ItemAttachToComponent"));
}

void AWeapon_Melee::ItemDetachToComponent(FDetachmentTransformRules& DetachmentRules)
{
	Super::ItemDetachToComponent(DetachmentRules);

	if (PickupMesh) PickupMesh->DetachFromComponent(DetachmentRules);
}

void AWeapon_Melee::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner())
	{
		return;
	}

	//UE_LOG(LogTemp, Display, TEXT("OnBoxBeginOverlap"));
	HitTrace();
}

void AWeapon_Melee::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == GetOwner())
	{
		return;
	}
}

void AWeapon_Melee::InitializeCollisionStates()
{
	Super::InitializeCollisionStates();

	if (BoxComponent)
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	}
}

void AWeapon_Melee::InitializeDelegates()
{
	if (HasAuthority())
	{
		BoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBoxBeginOverlap);
		BoxComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnBoxEndOverlap);
	}
}

void AWeapon_Melee::HitTrace()
{
	//UE_LOG(LogTemp, Display, TEXT("ActorToIgnore : %d"), ActorsToIgnore.Num());

	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	FHitResult OutHitResult;

	ETraceTypeQuery CanDamagedFromWeapon = UEngineTypes::ConvertToTraceType(ECC_CanDamagedByWeapon);
	//ETraceTypeQuery Visibility = ETraceTypeQuery::TraceTypeQuery1;

	EDrawDebugTrace::Type DebugTrace = bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	UKismetSystemLibrary::BoxTraceSingle(this, Start, End, TraceHalfSize, BoxTraceStart->GetComponentRotation(), CanDamagedFromWeapon, false, ActorsToIgnore, DebugTrace, OutHitResult, true);

	//AActor* HitActor = nullptr;
	//if (OutHitResult.GetActor())
	//{
	//	HitActor = OutHitResult.GetActor();
	//}
	//UE_LOG(LogTemp, Display, TEXT("OutHitResult.GetActor : %x"), OutHitResult.GetActor());


	if (OutHitResult.GetActor())
	{
		//UE_LOG(LogTemp, Display, TEXT("HitActor : %s"), *OutHitResult.GetActor()->GetName());
		IHitInterface* IHitActor = Cast<IHitInterface>(OutHitResult.GetActor());
		if (IHitActor)
		{

			UGameplayStatics::ApplyDamage(OutHitResult.GetActor(), DamageAmount, GetInstigatorController(), this, UDamageType::StaticClass());
			IHitActor->IGetHit(OutHitResult.ImpactPoint, OutHitResult, GetInstigatorController());

			ActorsToIgnore.AddUnique(OutHitResult.GetActor());

			if (OutHitResult.GetActor()->Tags.Num() > 0 && OutHitResult.GetActor()->Tags[0] == TEXT("Hovering"))
			{
				return;
			}
			else {
				ApplyForce(FieldSystemComponent, OutHitResult);
			}

		}
	}
}

