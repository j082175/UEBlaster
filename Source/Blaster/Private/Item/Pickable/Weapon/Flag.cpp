// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickable/Weapon/Flag.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Characters/BlasterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/OverlapItemInterface.h"
#include "Components/WidgetComponent.h"


AFlag::AFlag()
{
	//PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));

	//SetRootComponent(PickupMesh);
	//OverlapCapsule->SetupAttachment(PickupMesh);
	//PickupWidget->SetupAttachment(PickupMesh);
	PickupMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupMesh->SetIsReplicated(true);
}

void AFlag::ResetFlag()
{

	ABlasterCharacter* FlagBearer = Cast<ABlasterCharacter>(GetOwner());
	if (FlagBearer)
	{
		FlagBearer->SetHoldingTheFlag(false);
		FlagBearer->SetOverlappingWeapon(nullptr);
		FlagBearer->UnCrouch();
	}


	if (!HasAuthority()) return; // the code below only executed when has authority
	SetWeaponState(EWeaponState::EWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	PickupMesh->DetachFromComponent(DetachRules);

	SetWeaponState(EWeaponState::EWS_Initial);
	OverlapCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;

	SetActorTransform(InitialTransform);
}

void AFlag::MulticastResetFlag_Implementation()
{
	//if (!HasAuthority()) UE_LOG(LogTemp, Display, TEXT("MulticastResetFlag_Implementation"));
	ABlasterCharacter* FlagBearer = Cast<ABlasterCharacter>(GetOwner());
	if (FlagBearer)
	{
		FlagBearer->SetHoldingTheFlag(false);
		FlagBearer->SetOverlappingWeapon(nullptr);
		FlagBearer->UnCrouch();
	}


	SetWeaponState(EWeaponState::EWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	PickupMesh->DetachFromComponent(DetachRules);

	SetWeaponState(EWeaponState::EWS_Initial);
	OverlapCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	if (!HasAuthority()) return; // the code below only executed when has authority


	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;

	SetActorTransform(InitialTransform);
}

//void AFlag::OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//
//	IOverlapItemInterface* CheckCharacterInterface = Cast<IOverlapItemInterface>(OtherActor);
//	if (CheckCharacterInterface == nullptr)
//	{
//		return;
//	}
//
//	if (PickupWidget)
//	{
//		//PickupWidget->SetVisibility(true);
//
//		ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
//		if (BlasterCharacter)
//		{
//			if (WeaponType == EWeaponType::EWT_Flag && BlasterCharacter->GetTeam() == Team)	return;
//			if (BlasterCharacter->IsHoldingTheFlag()) return;
//		}
//		CheckCharacterInterface->IGetItem(this);
//	}
//}
//
//void AFlag::OnCapsuleEndOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	//UE_LOG(LogTemp, Display, TEXT("OnCapsuleEndOverlapFunc"));
//
//	IOverlapItemInterface* CheckCharacterInterface = Cast<IOverlapItemInterface>(OtherActor);
//	if (CheckCharacterInterface == nullptr)
//	{
//		return;
//	}
//
//	if (PickupWidget)
//	{
//		//PickupWidget->SetVisibility(false);
//
//		ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
//		if (BlasterCharacter)
//		{
//			if (WeaponType == EWeaponType::EWT_Flag && BlasterCharacter->GetTeam() == Team)	return;
//			if (BlasterCharacter->IsHoldingTheFlag()) return;
//		}
//		CheckCharacterInterface->IGetItem(nullptr);
//	}
//}

void AFlag::BeginPlay()
{
	Super::BeginPlay();

	InitialTransform = GetActorTransform();
}

void AFlag::Dropped()
{
	UE_LOG(LogTemp, Display, TEXT("Flag : Dropped"));

	SetWeaponState(EWeaponState::EWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	PickupMesh->DetachFromComponent(DetachRules);
	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;

}

void AFlag::OnEquipped()
{
	ShowPickupWidget(false);
	OverlapCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupMesh->SetSimulatePhysics(false);
	PickupMesh->SetEnableGravity(false);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	EnableCustomDepth(false);
}

void AFlag::OnDropped()
{
	//UE_LOG(LogTemp, Display, TEXT("Flag : OnDropped"));
	if (HasAuthority())
	{
		OverlapCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	PickupMesh->SetSimulatePhysics(true);
	PickupMesh->SetEnableGravity(true);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickupMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	PickupMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	PickupMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	EnableCustomDepth(true);
}
