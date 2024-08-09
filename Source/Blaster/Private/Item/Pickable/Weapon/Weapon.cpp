// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickable/Weapon/Weapon.h"

// Components
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"

#include "Net/UnrealNetwork.h"


#include "Characters/BlasterCharacter.h"
#include "PlayerController/BlasterPlayerController.h"

// FieldSystem
#include "Field/FieldSystemComponent.h"
#include "NiagaraComponent.h"

#include "Blaster/DrawDebug.h"


AWeapon::AWeapon()
{
	FieldSystemComponent = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("FieldSystemComponent"));
	ensure(FieldSystemComponent);
	FieldSystemComponent->SetupAttachment(RootComponent);

	//OverlapCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	//ensure(OverlapCapsule);

	//PersistEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PersistEffectComponent"));

	InitializeCollisionStates();
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, WeaponState);
	//DOREPLIFETIME(AWeapon, Ammo);
	DOREPLIFETIME_CONDITION(ThisClass, bUseServerSideRewind, COND_OwnerOnly);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}

	NullChecker(FieldSystemComponent, TEXT("FieldSystemComponent"), GetName());
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Moving(DeltaTime);
}

void AWeapon::OnRep_Owner()
{
	Super::OnRep_Owner();


}

void AWeapon::ItemAttachToComponent(USceneComponent* SceneComponent, FAttachmentTransformRules& AttachmentRules, FName InSocketName)
{
	ItemState = EItemState::EIS_Equipped;
	WeaponState = EWeaponState::EWS_Equipped;

	if (OverlapCapsule) OverlapCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//if (PickupMesh)
	//{
	//	if (!PickupMesh->AttachToComponent(SceneComponent, AttachmentRules, InSocketName))
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("AttachToComponent Failed"));
	//	}
	//}

	if (SceneComponent && SceneComponent->GetOwner())
	{
		SetOwner(SceneComponent->GetOwner());

		APawn* InstigatorPawn = Cast<APawn>(SceneComponent->GetOwner());
		SetInstigator(InstigatorPawn);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item : No Owner"));
	}

	OnEquipped();
	OnEquippedSecondary();
}

void AWeapon::ItemDetachToComponent(FDetachmentTransformRules& DetachmentRules)
{
	ItemState = EItemState::EIS_Hovering;
	WeaponState = EWeaponState::EWS_Initial;

	if (OverlapCapsule) OverlapCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//if (PickupMesh) PickupMesh->DetachFromComponent(DetachmentRules);

	SetOwner(nullptr);
	SetInstigator(nullptr);

	WeaponState = EWeaponState::EWS_Dropped;
	OnDropped();
}

void AWeapon::Dropped()
{
	//UE_LOG(LogTemp, Display, TEXT("Dropped"));

}

void AWeapon::OnPingTooHigh(bool bPingTooHigh)
{
	bUseServerSideRewind = !bPingTooHigh;
}


void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	OnWeaponStateSet();
}

void AWeapon::OnWeaponStateSet()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Initial:
		break;
	case EWeaponState::EWS_Equipped:
		OnEquipped();
		break;
	case EWeaponState::EWS_EquippedSecondary:
		OnEquippedSecondary();
		break;
	case EWeaponState::EWS_Dropped:
		OnDropped();
		break;
	case EWeaponState::EWS_MAX:
		break;
	default:
		break;
	}
}

void AWeapon::OnRep_WeaponState()
{
	OnWeaponStateSet();
}

void AWeapon::OnEquipped()
{
	//UE_LOG(LogTemp, Display, TEXT("OnEquipped"));
	EnableCustomDepth(false);
	ShowPickupWidget(false);

	OverlapCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
	if (BlasterOwnerCharacter && bUseServerSideRewind)
	{
		BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->GetController()) : BlasterOwnerController;

		if (BlasterOwnerController && HasAuthority() && !BlasterOwnerController->HighPingDelegate.IsBound())
		{
			BlasterOwnerController->HighPingDelegate.AddDynamic(this, &ThisClass::OnPingTooHigh);
		}
	}

}

void AWeapon::OnDropped()
{
	if (HasAuthority())
	{
		OverlapCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	EnableCustomDepth(true);

	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
	if (BlasterOwnerCharacter && bUseServerSideRewind)
	{
		BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->GetController()) : BlasterOwnerController;

		if (BlasterOwnerController && HasAuthority() && BlasterOwnerController->HighPingDelegate.IsBound())
		{
			BlasterOwnerController->HighPingDelegate.RemoveDynamic(this, &ThisClass::OnPingTooHigh);
		}
	}
}

void AWeapon::OnEquippedSecondary()
{

	//EnableCustomDepth(true); 
	ShowPickupWidget(false);

	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
	if (BlasterOwnerCharacter && bUseServerSideRewind)
	{
		BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->GetController()) : BlasterOwnerController;

		if (BlasterOwnerController && HasAuthority() && BlasterOwnerController->HighPingDelegate.IsBound())
		{
			BlasterOwnerController->HighPingDelegate.RemoveDynamic(this, &ThisClass::OnPingTooHigh);
		}
	}
}

void AWeapon::EnableCustomDepth(bool bEnable)
{

}

void AWeapon::InitializeCollisionStates()
{
}

void AWeapon::ApplyForce(UFieldSystemComponent* InFieldSystemComponent, const FHitResult& InHitResult)
{
	if (InFieldSystemComponent)
	{
		//UE_LOG(LogTemp, Display, TEXT("%s : FieldSystem Actived"), *UEnum::GetDisplayValueAsText(GetLocalRole()).ToString());

		float ForceDamage = 1000'00000.f;
		float ForceRadius = 50.f;

		//UE_LOG(LogTemp, Display, TEXT("Normal : %f, %f, %f"), -OutHitResult.Normal.X, -OutHitResult.Normal.Y, -OutHitResult.Normal.Z);
		InFieldSystemComponent->ApplyStrainField(true, InHitResult.ImpactPoint, ForceRadius, ForceDamage, 0);
		//UKismetSystemLibrary::DrawDebugArrow(this, OutHitResult.ImpactPoint, OutHitResult.ImpactPoint + OutHitResult.ImpactNormal * 50.f, 10.f, FLinearColor::Red, 2.f);
		//InFieldSystemComponent->ApplyLinearForce(true, -InHitResult.ImpactNormal, ForceDamage);
	}
}

void AWeapon::Moving(float DeltaTime)
{
	if (WeaponState == EWeaponState::EWS_Initial)
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

void AWeapon::ShowPickupWidget(bool bShowWidget)
{
	//UE_LOG(LogTemp, Display, TEXT("ShowPickupWidget : %d"), bShowWidget);
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}