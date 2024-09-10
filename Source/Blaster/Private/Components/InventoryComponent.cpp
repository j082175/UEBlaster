// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Characters/CharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "Item/Pickable/Weapon/Weapon.h"
#include "Item/Pickable/Weapon/Weapon_Gun.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickInterval = 1.f;


	// ...
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, CarriedAmmo, COND_OwnerOnly);
	DOREPLIFETIME(ThisClass, EquippedWeapon);
	DOREPLIFETIME(ThisClass, SecondaryWeapon);
	DOREPLIFETIME(ThisClass, Grenades);
}

void UInventoryComponent::PostLoad()
{
	Super::PostLoad();

	SetIsReplicated(true);

	StartingARAmmo = 300.f;
	StartingRocketAmmo = 10.f;
	StartingPistolAmmo = 300.f;
	StartingSMGAmmo = 320.f;
	StartingShotgunAmmo = 64.f;
	StartingSniperAmmo = 30.f;
	StartingGrenadeLauncherAmmo = 30.f;

	Grenades = MaxGrenades;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	OwingCharacter = Cast<ACharacterBase>(GetOwner());

	if (GetOwner()->HasAuthority())
	{
		InitializeCarriedAmmo();
	}

	Init();
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::SubtractCarriedAmmoMap(EWeaponType InWeaponType, int32 InDecreaseAmount)
{
	if (CarriedAmmoMap.Contains(InWeaponType))
	{
		CarriedAmmoMap[InWeaponType] -= InDecreaseAmount;
		OnCarriedAmmoChanged.Broadcast(CarriedAmmoMap[InWeaponType]);
	}
}

void UInventoryComponent::InitializeCarriedAmmo()
{
	CarriedAmmoMap.Emplace(EWeaponType::AssaultRifle, StartingARAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::RocketLauncher, StartingRocketAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::Pistol, StartingPistolAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::SMG, StartingSMGAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::Shotgun, StartingShotgunAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::SniperRifle, StartingSniperAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::GrenadeLauncher, StartingGrenadeLauncherAmmo);

}

void UInventoryComponent::UpdateCarriedAmmo()
{
	if (EquippedWeapon == nullptr) return;
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
		OnCarriedAmmoChanged.Broadcast(CarriedAmmo);

	}
}

void UInventoryComponent::OnRep_EquippedWeapon()
{
	OwingCharacter = Cast<ACharacterBase>(GetOwner());
	OwingCharacter->EquipWeaponFunc();
}

void UInventoryComponent::OnRep_SecondaryWeapon()
{
	OwingCharacter = Cast<ACharacterBase>(GetOwner());
	OwingCharacter->EquipSecondaryFunc();
}

void UInventoryComponent::OnRep_Grenades()
{
	OnGrenadeCountChanged.Broadcast(GetGrenades());
}

void UInventoryComponent::OnRep_CarriedAmmo()
{
	if (CarriedAmmo == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("JumpToShotgunEnd"));
		OwingCharacter->JumpToShotgunEnd();
	}

	OnCarriedAmmoChanged.Broadcast(CarriedAmmo);

	//UpdateCarriedAmmo();
}

void UInventoryComponent::Init()
{
	GetWorld()->GetTimerManager().SetTimer(InitTimer, this, &ThisClass::InitFunc, 0.2f, true);
}

void UInventoryComponent::InitFunc()
{
	if (GetOwner() && GetWorld() && GetOwner()->GetInstigatorController() != GetWorld()->GetFirstPlayerController())
	{
		GetWorld()->GetTimerManager().ClearTimer(InitTimer);
		InitTimer.Invalidate();
		return;
	}

	if (OnCarriedAmmoChanged.IsBound() && OnGrenadeCountChanged.IsBound())
	{
		OnCarriedAmmoChanged.Broadcast(CarriedAmmo);
		OnGrenadeCountChanged.Broadcast(Grenades);

		GetWorld()->GetTimerManager().ClearTimer(InitTimer);
		InitTimer.Invalidate();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("UInventoryComponent Initializing..."));
		UE_LOG(LogTemp, Display, TEXT("GetOwner()->GetInstigatorController() : %s"), *GetOwner()->GetInstigatorController()->GetName());
	}
}
