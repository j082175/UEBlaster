// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/Pickable/Weapon/WeaponTypes.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentAmmoChangedDelegate, int32 /*InAmmo*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCarriedAmmoChangedDelegate, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGrenadeCountChangedDelegate, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponNameChangedDelegate, EWeaponName);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostLoad() override;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE class AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
	FORCEINLINE class AWeapon* GetSecondaryEquippedWeapon() const { return SecondaryWeapon; }
	FORCEINLINE float GetCarriedAmmo() const { return CarriedAmmo; }
	FORCEINLINE class AFlag* GetFlag() const { return Flag; }

	FORCEINLINE void SetEquippedWeapon(class AWeapon* W) { EquippedWeapon = W; }
	FORCEINLINE void SetSecondaryEquippedWeapon(class AWeapon* W) { SecondaryWeapon = W; }
	FORCEINLINE float GetGrenades() const { return Grenades; }
	FORCEINLINE void SetFlag(class AFlag* InFlag) { Flag = InFlag; }

	void SubtractCarriedAmmoMap(EWeaponType InWeaponType, int32 InDecreaseAmount);

	FOnCurrentAmmoChangedDelegate OnCurrentAmmoChanged;
	FOnCarriedAmmoChangedDelegate OnCarriedAmmoChanged;
	FOnGrenadeCountChangedDelegate OnGrenadeCountChanged;
	FOnWeaponNameChangedDelegate OnWeaponNameChanged;


	TWeakObjectPtr<class ACharacterBase> OwingCharacter;
public:	


	void InitializeCarriedAmmo();

	virtual void UpdateCarriedAmmo();


	UFUNCTION()
	void OnRep_EquippedWeapon();
	UFUNCTION()
	void OnRep_SecondaryWeapon();

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class AWeapon> EquippedWeapon;
	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon, EditAnywhere)
	TObjectPtr<class AWeapon> SecondaryWeapon;

	UFUNCTION()
	void OnRep_Grenades();
	UPROPERTY(ReplicatedUsing = OnRep_Grenades, EditAnywhere)
	float Grenades;
	UPROPERTY(EditAnywhere)
	float MaxGrenades = 4;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo, EditAnywhere)
	int32 CarriedAmmo;


	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 StartingARAmmo;
	UPROPERTY(EditAnywhere)
	int32 StartingRocketAmmo;
	UPROPERTY(EditAnywhere)
	int32 StartingPistolAmmo;
	UPROPERTY(EditAnywhere)
	int32 StartingSMGAmmo;
	UPROPERTY(EditAnywhere)
	int32 StartingShotgunAmmo;
	UPROPERTY(EditAnywhere)
	int32 StartingSniperAmmo;
	UPROPERTY(EditAnywhere)
	int32 StartingGrenadeLauncherAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class AFlag> Flag;

private:
	void Init();
	void InitFunc();

	FTimerHandle InitTimer;

};
