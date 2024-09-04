// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickable/Weapon/Weapon_Gun.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Interfaces/OverlapItemInterface.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimationAsset.h"
#include "Actor/Casing.h"
#include "Engine/SkeletalMeshSocket.h"
#include "PlayerController/BlasterPlayerController.h"
#include "Characters/BlasterCharacter.h"
#include "Components/CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryComponent.h"
#include "Blaster/Blaster.h"

// Niagara
#include "NiagaraComponent.h"

// FieldSystem
#include "Field/FieldSystemComponent.h"


#include "Kismet/GameplayStatics.h"
//#include "GameMode/BlasterGameMode.h"
#include "GameState/BlasterGameState.h"
#include "Components/ObjectPoolComponent.h"

#include "GameData/DataSingleton.h"



// Sets default values
AWeapon_Gun::AWeapon_Gun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true; // Now this actor can be replicated
	SetReplicateMovement(true);

	WeaponSKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	//WeaponSKMesh->SetupAttachment(RootComponent);



	WeaponSKMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	WeaponSKMesh->MarkRenderStateDirty(); // CustomDepth 설정 가능하게 허용
	EnableCustomDepth(true);



	OverlapCapsule->SetGenerateOverlapEvents(true);


	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidgetComponent"));


	SetRootComponent(WeaponSKMesh);
	OverlapCapsule->SetupAttachment(RootComponent);
	//PersistEffectComponent->SetupAttachment(RootComponent);
	FieldSystemComponent->SetupAttachment(RootComponent);
	PickupWidget->SetupAttachment(RootComponent);


	InitializeCollisionStates();
	OverlapCapsule->SetCapsuleHalfHeight(35.f);
	OverlapCapsule->SetCapsuleRadius(35.f);

	AdditionalSphereRadius = SphereRadius;

	DefaultRecoilYaw = FMath::Abs(DefaultRecoilPitch);
}

void AWeapon_Gun::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//WeaponData = 

	WeaponData = UDataSingleton::Get().GetWeaponName(WeaponName);

	//Damage = WeaponData.Damage;
	//FireDelay = WeaponData.FireDelay;
	//MagCapacity = WeaponData.MagCapacity;
	//Ammo = WeaponData.MagCapacity;
}

//void AWeapon_Gun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(AWeapon_Gun, WeaponState);
//	//DOREPLIFETIME(AWeapon_Gun, Ammo);
//	DOREPLIFETIME_CONDITION(ThisClass, bUseServerSideRewind, COND_OwnerOnly);
//}

// Called every frame
void AWeapon_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called when the game starts or when spawned
void AWeapon_Gun::BeginPlay()
{
	Super::BeginPlay();

	if (true) // Showing on our PickupWidget doesn't affect our gameplay. but pickup Weapon should only played on server.
	{
		//if (!HasAuthority())
		//{
		//	FString Str = UEnum::GetDisplayValueAsText(GetLocalRole()).ToString();
		//	UE_LOG(LogTemp, Error, TEXT("%s"), *Str);
		//}


		//UE_LOG(LogTemp, Display, TEXT("HasAuthority"));
		//OverlapCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//OverlapCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		//OverlapCapsule->OnComponentBeginOverlap.AddUniqueDynamic(this, &AWeapon_Gun::OnCapsuleBeginOverlapFunc);
		//OverlapCapsule->OnComponentEndOverlap.AddUniqueDynamic(this, &AWeapon_Gun::OnCapsuleEndOverlapFunc);
	}

	//if (PickupWidget)
	//{
	//	PickupWidget->SetVisibility(false);
	//}

	//if (!HasAuthority())
	//{
	//	FireDelay = 0.001f;
	//}
}

void AWeapon_Gun::OnRep_Owner()
{
	Super::OnRep_Owner();
	if (Owner == nullptr)
	{
		BlasterOwnerCharacter = nullptr;
		BlasterOwnerController = nullptr;
	}
	else
	{

		BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;

		TWeakObjectPtr<UInventoryComponent> IC = BlasterOwnerCharacter->GetComponentByClass<UInventoryComponent>();
		if (BlasterOwnerCharacter && IC->GetEquippedWeapon() && IC->GetEquippedWeapon() == this)
		{
			SetHUDAmmo();
		}
	}
}

#if WITH_EDITOR
void AWeapon_Gun::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 변경된 프로퍼티의 이름을 가져온다.
	FName PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// 변경된 프로퍼티에 따라 작업을 수행
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, DefaultRecoilPitch))
	{
		DefaultRecoilYaw = FMath::Abs(DefaultRecoilPitch);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, DefaultRecoilYaw))
	{
		DefaultRecoilPitch = -DefaultRecoilYaw;
	}
}
#endif // WITH_EDITOR

void AWeapon_Gun::ItemAttachToComponent(USceneComponent* SceneComponent, FAttachmentTransformRules& AttachmentRules, FName InSocketName)
{
	Super::ItemAttachToComponent(SceneComponent, AttachmentRules, InSocketName);

	if (WeaponSKMesh)
	{
		if (!WeaponSKMesh->AttachToComponent(SceneComponent, AttachmentRules, InSocketName))
		{
			UE_LOG(LogTemp, Error, TEXT("AttachToComponent Failed"));
		}
	}
}

void AWeapon_Gun::ItemDetachToComponent(FDetachmentTransformRules& DetachmentRules)
{
	Super::ItemDetachToComponent(DetachmentRules);

	if (WeaponSKMesh) WeaponSKMesh->DetachFromComponent(DetachmentRules);
}

float AWeapon_Gun::GetRandomRecoilPitch() const
{
	return FMath::RandRange(FMath::Clamp(DefaultRecoilPitch - PitchRange, 0.f, DefaultRecoilPitch), DefaultRecoilPitch + PitchRange);
}

float AWeapon_Gun::GetRandomRecoilYaw() const
{
	return FMath::RandRange(DefaultRecoilYaw - YawRange, DefaultRecoilPitch + YawRange);
}

void AWeapon_Gun::SetHUDAmmo()
{

	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
	if (BlasterOwnerCharacter) BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->GetController()) : BlasterOwnerController;

	UInventoryComponent* IC = BlasterOwnerCharacter->GetComponentByClass<UInventoryComponent>();

	//if (BlasterOwnerController)
	//{
	//	BlasterOwnerController->SetHUDWeaponAmmo(Ammo);
	//	//UE_LOG(LogTemp, Warning, TEXT("BlasterOwnerController"));

	//}
	// 

	if (IC)
	{
		IC->OnCurrentAmmoChanged.ExecuteIfBound(Ammo);
	}
	//if (IC && IC->OnCurrentAmmoChanged.ExecuteIfBound(Ammo))
	//{
	//}
	//else
	//{
	//	if (HasAuthority())
	//	{
	//		//UE_LOG(LogTemp, Error, TEXT("BlasterOwnerController"));
	//		FTimerHandle Timerhandle;
	//		GetWorldTimerManager().SetTimer(Timerhandle, this, &ThisClass::SetHUDAmmo, 0.1f);
	//	}
	//}
}

void AWeapon_Gun::Dropped()
{
	Super::Dropped();

	SetWeaponState(EWeaponState::EWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;

	WeaponSKMesh->DetachFromComponent(DetachRules);
}

void AWeapon_Gun::EjectCasing()
{
	//UE_LOG(LogTemp, Display, TEXT("Eject"));
	if (CasingClass)
	{
		const USkeletalMeshSocket* AmmoEjectSocket = GetWeaponMesh()->GetSocketByName(TEXT("AmmoEject"));
		if (AmmoEjectSocket)
		{
			FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(GetWeaponMesh());
			if (CasingClass)
			{
				UWorld* World = GetWorld();
				if (World)
				{
					FTransform T(SocketTransform.GetRotation().Rotator(), SocketTransform.GetLocation());
					//World->SpawnActor<ACasing>(CasingClass, T);
					//APooledObject* PO = Cast<UObjectPoolSubsystem>(GetGameInstance())->GetSpawnedObject(T, CasingClass);
					//CasingClass.GetDefaultObject()->Deactivate();

					GetWorld()->GetGameStateChecked<ABlasterGameState>()->GetComponentByClass<UObjectPoolComponent>()->GetSpawnedObject(T, CasingClass);
					//GetWorld()->GetAuthGameMode<ABlasterGameMode>()->GetComponentByClass<UObjectPoolComponent>()->GetSpawnedObject(T, CasingClass);
				}
			}
		}
	}
}

void AWeapon_Gun::InitializeCollisionStates()
{
	Super::InitializeCollisionStates();

	if (OverlapCapsule)
	{
		OverlapCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		OverlapCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		OverlapCapsule->SetCollisionResponseToChannel(ECC_IsPickable, ECollisionResponse::ECR_Block);
		OverlapCapsule->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	}

	if (WeaponSKMesh)
	{
		WeaponSKMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponSKMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		WeaponSKMesh->SetCollisionResponseToChannel(ECC_IsPickable, ECollisionResponse::ECR_Ignore);
		WeaponSKMesh->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);

	}

}

void AWeapon_Gun::OnEquipped()
{
	Super::OnEquipped();

	OverlapCapsule->SetCollisionResponseToChannel(ECC_IsPickable, ECollisionResponse::ECR_Ignore);
	OverlapCapsule->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);


	WeaponSKMesh->SetSimulatePhysics(false);
	WeaponSKMesh->SetEnableGravity(false);
	WeaponSKMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (WeaponType == EWeaponType::EWT_SMG)
	{
		WeaponSKMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponSKMesh->SetEnableGravity(true);
		//WeaponSKMesh->SetSimulatePhysics(true);
		WeaponSKMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}

}

void AWeapon_Gun::OnDropped()
{
	Super::OnDropped();

	OverlapCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapCapsule->SetCollisionResponseToChannel(ECC_IsPickable, ECollisionResponse::ECR_Block);
	OverlapCapsule->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);


	WeaponSKMesh->SetSimulatePhysics(true);
	WeaponSKMesh->SetEnableGravity(true);
	WeaponSKMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponSKMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponSKMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	//WeaponSKMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	//WeaponSKMesh->SetCollisionResponseToChannel(ECC_IsPickable, ECollisionResponse::ECR_Block);
	ShowPickupWidget(false);
}

void AWeapon_Gun::OnEquippedSecondary()
{
	Super::OnEquippedSecondary();

	OverlapCapsule->SetCollisionResponseToChannel(ECC_IsPickable, ECollisionResponse::ECR_Ignore);
	OverlapCapsule->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);


	WeaponSKMesh->SetSimulatePhysics(false);
	WeaponSKMesh->SetEnableGravity(false);
	WeaponSKMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (WeaponType == EWeaponType::EWT_SMG)
	{
		WeaponSKMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponSKMesh->SetEnableGravity(true);
		//WeaponSKMesh->SetSimulatePhysics(true);
		WeaponSKMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}

	if (WeaponSKMesh)
	{
		WeaponSKMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_TAN);
		WeaponSKMesh->MarkRenderStateDirty();
	}
}

//void AWeapon_Gun::ShowPickupWidget(bool bShowWidget)
//{
//	UE_LOG(LogTemp, Display, TEXT("ShowPickupWidget1"));
//	if (PickupWidget)
//	{
//		UE_LOG(LogTemp, Display, TEXT("ShowPickupWidget"));
//		PickupWidget->SetVisibility(bShowWidget);
//	}
//}




//void AWeapon_Gun::OnRep_Ammo()
//{
//	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
//	if (BlasterOwnerCharacter && BlasterOwnerCharacter->GetCombatComponent() && IsFull())
//	{
//		BlasterOwnerCharacter->GetCombatComponent()->JumpToShotgunEnd();
//	}
//	SetHUDAmmo();
//}

void AWeapon_Gun::Fire(const FVector& HitTarget)
{

	if (FireAnimation)
	{
		//UE_LOG(LogTemp, Display, TEXT("Fire"));
		WeaponSKMesh->PlayAnimation(FireAnimation, false);

		if (WeaponType == EWeaponType::EWT_SniperRifle)
		{
			ACharacterBase* CharacterOwner = Cast<ACharacterBase>(GetOwner());
			if (CharacterOwner)
			{
				//UE_LOG(LogTemp, Display, TEXT("PlayBoltActionMontage"));
				CharacterOwner->PlayBoltActionMontage(TEXT("BoltAction"));
			}
		}
	}

	if (WeaponType != EWeaponType::EWT_SniperRifle && WeaponType != EWeaponType::EWT_Shotgun) EjectCasing();

	if (true) // HasAutority 를 지움으로서 Client-Side Prediction 구현 하지만 Jitter 발생 => Replicated 가 아닌 RPC 로 해결
	{
		SpendRound();
	}

	
}

void AWeapon_Gun::AddAmmo(int32 AmmoToAdd)
{
	//if (BlasterOwnerCharacter)
	//{
	//	FString Str = UEnum::GetDisplayValueAsText(BlasterOwnerCharacter->GetLocalRole()).ToString();
	//	UE_LOG(LogTemp, Display, TEXT("AddAmmo, %s"), *Str);
	//}


	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagCapacity);
	SetHUDAmmo();
	ClientAddAmmo(AmmoToAdd);
}

void AWeapon_Gun::ClientAddAmmo_Implementation(int32 AmmoToAdd)
{


	if (HasAuthority()) return;

	//if (BlasterOwnerCharacter)
	//{
	//	FString Str = UEnum::GetDisplayValueAsText(BlasterOwnerCharacter->GetLocalRole()).ToString();
	//	UE_LOG(LogTemp, Display, TEXT("ClientAddAmmo_Implementation, %s"), *Str);
	//}

	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagCapacity);
	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ACharacterBase>(GetOwner()) : BlasterOwnerCharacter;
	if (BlasterOwnerCharacter && BlasterOwnerCharacter->GetCombatComponent() && IsFull())
	{
		BlasterOwnerCharacter->JumpToShotgunEnd();
	}

	SetHUDAmmo();
}

void AWeapon_Gun::ClientUpdateAmmo_Implementation(int32 ServerAmmo)
{

	if (HasAuthority()) return;

	//if (BlasterOwnerCharacter)
	//{
	//	FString Str = UEnum::GetDisplayValueAsText(BlasterOwnerCharacter->GetLocalRole()).ToString();
	//	UE_LOG(LogTemp, Display, TEXT("ClientUpdateAmmo_Implementation, %s"), *Str);
	//}

	//if (BlasterOwnerCharacter)
	//{
	//	FString Str = UEnum::GetDisplayValueAsText(BlasterOwnerCharacter->GetLocalRole()).ToString();
	//	UE_LOG(LogTemp, Warning, TEXT("ClientUpdateAmmo_Implementation_ %s : Sequence: %d, Ammo : %d"), *Str, Sequence, ServerAmmo);
	//}

	Ammo = ServerAmmo;
	--Sequence;
	Ammo -= Sequence;
	SetHUDAmmo();

	//UE_LOG(LogTemp, Warning, TEXT("ClientUpdateAmmo_Implementation_ Ammo : %d"), Ammo);
}

void AWeapon_Gun::SpendRound()
{
	//if (BlasterOwnerCharacter)
	//{
	//	FString Str = UEnum::GetDisplayValueAsText(BlasterOwnerCharacter->GetLocalRole()).ToString();
	//	UE_LOG(LogTemp, Display, TEXT("SpendRound, %s"), *Str);
	//}
	Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
	SetHUDAmmo();
	if (HasAuthority())
	{
		ClientUpdateAmmo(Ammo);
	}
	else if (BlasterOwnerCharacter && BlasterOwnerCharacter->IsLocallyControlled())
	{
		++Sequence;

		//if (BlasterOwnerCharacter)
		//{
		//	FString Str = UEnum::GetDisplayValueAsText(BlasterOwnerCharacter->GetLocalRole()).ToString();
		//	UE_LOG(LogTemp, Error, TEXT("SpendRound_ %s : Sequence: %d, Ammo : %d"), *Str, Sequence, Ammo);
		//}
	}




}



void AWeapon_Gun::EnableCustomDepth(bool bEnable)
{
	if (WeaponSKMesh)
	{
		WeaponSKMesh->SetRenderCustomDepth(bEnable);
	}
}

FVector AWeapon_Gun::TraceEndWithScatter(const FVector& HitTarget)
{
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(TEXT("MuzzleFlash"));
	if (MuzzleFlashSocket == nullptr) return FVector();

	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	const FVector TraceStart = SocketTransform.GetLocation();


	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	const FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;
	//DrawDebugLine(GetWorld(), TraceStart, SphereCenter, FColor::Magenta, false, 5.f);
	const FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, AdditionalSphereRadius);
	const FVector EndLoc = SphereCenter + RandVec;
	const FVector ToEndLoc = EndLoc - TraceStart;

	//DrawDebugSphere(GetWorld(), SphereCenter, AdditionalSphereRadius, 12, FColor::Red, false, 5.f);
	//DrawDebugSphere(GetWorld(), EndLoc, 4.f, 12, FColor::Orange, false, 5.f);
	//DrawDebugLine(GetWorld(), TraceStart, FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size()), FColor::Cyan, false, 5.f);


	FHitResult OutHitResult;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());
	UKismetSystemLibrary::LineTraceSingle(this, TraceStart, TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size(), ETraceTypeQuery::TraceTypeQuery3, false, IgnoreActors, EDrawDebugTrace::None, OutHitResult, true, FLinearColor::Yellow);

	if (bUseScatter && OutHitResult.bBlockingHit)
	{
		return OutHitResult.ImpactPoint;
	}
	else
	{
		return FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size());
	}

}

