// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "Item/Item.h"
#include "Item/Pickable/Weapon/Weapon.h"
#include "WeaponTypes.h"
#include "BlasterTypes/Team.h"

#include "GameData/WeaponData.h"
#include "Weapon_Gun.generated.h"


UENUM(BlueprintType)
enum class EFireType : uint8
{
	EFT_HitScan = 0 UMETA(DisplayName = "HitScanWeapon"),
	EFT_Projectile UMETA(DisplayName = "ProjectileWeapon"),
	EFT_Shotgun UMETA(DisplayName = "ShotgunWeapon"),

	EFT_MAX UMETA(DisplayName = "MAX")

};

UCLASS()
class BLASTER_API AWeapon_Gun : public AWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon_Gun();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void OnRep_Owner() override;
	virtual void PostLoad() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	virtual void ItemAttachToComponent(USceneComponent* SceneComponent, FAttachmentTransformRules& AttachmentRules, FName InSocketName) override;
	virtual void ItemDetachToComponent(FDetachmentTransformRules& DetachmentRules) override;


	// Getters
	FORCEINLINE class USkeletalMeshComponent* GetWeaponMesh() const { return WeaponSKMesh; }
	FORCEINLINE EFireType GetFireType() const { return FireType; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; }
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }
	FORCEINLINE float GetFireDelay() const { return FireDelay; }
	FORCEINLINE float GetDistanceToSphere() const { return DistanceToSphere; }
	FORCEINLINE float GetSphereRadius() const { return SphereRadius; }
	FORCEINLINE float GetAdditionalSphereRadius() const { return AdditionalSphereRadius; }
	float GetRandomRecoilPitch() const;
	float GetRandomRecoilYaw() const;


	FORCEINLINE bool IsAutomatic() const { return bAutomatic; }
	FORCEINLINE bool IsEmpty() const { return Ammo <= 0; }
	FORCEINLINE bool IsFull() const { return Ammo == MagCapacity; }

	FORCEINLINE void SetDistanceToSphere(float InDistanceToSphere) { DistanceToSphere = InDistanceToSphere; }
	FORCEINLINE void SetAdditionalSphereRadius(float InRadius) { AdditionalSphereRadius = InRadius; }

	FORCEINLINE void SetIsAutomatic(bool torf) { bAutomatic = torf; }


	void SetHUDAmmo();
	virtual void SetHUD() override;

	virtual void Dropped() override;

	UFUNCTION(BlueprintCallable)
	void EjectCasing();



	// Interfaces
	virtual void IBindWidget(class UUserWidget* InUserWidget) override;


protected:
	virtual void InitializeCollisionStates() override;


	virtual void OnEquipped() override;
	virtual void OnDropped() override;
	virtual void OnEquippedSecondary() override;

private:
	void InitData();

	// Else ETC.
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class APooledObject> CasingClass;

	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UTexture2D> CrosshairCenter;

	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UTexture2D> CrosshairLeft;

	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UTexture2D> CrosshairRight;

	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UTexture2D> CrosshairTop;

	UPROPERTY(EditAnywhere, Category = "Crosshair", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UTexture2D> CrosshairBottom;

public:


	UPROPERTY(EditAnywhere)
	EFireType FireType;




	// For Zoom
private:
	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;

	// automatic fire
private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bAutomatic = true;

	// Recoil System
private:
	UPROPERTY(VisibleAnywhere)
	float DefaultRecoilPitch = -0.5f;

	UPROPERTY(EditAnywhere)
	float PitchRange = 0.2f;

	UPROPERTY(VisibleAnywhere)
	float DefaultRecoilYaw = 0.5f;

	UPROPERTY(EditAnywhere)
	float YawRange = 0.2f;


	// Components
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon|Weapon Properties")
	TObjectPtr<class USkeletalMeshComponent> WeaponSKMesh;

	//UPROPERTY(VisibleAnywhere, Category = "Weapon|Weapon Properties")
	//TObjectPtr<class USphereComponent> AreaSphere;







	// Animation
public:
	virtual void Fire(const FVector& HitTarget);
	void AddAmmo(int32 AmmoToAdd);
private:
	UFUNCTION(Client, Reliable)
	void ClientUpdateAmmo(int32 ServerAmmo);

	UFUNCTION(Client, Reliable)
	void ClientAddAmmo(int32 AmmoToAdd);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TObjectPtr<class UAnimationAsset> FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TObjectPtr<class USoundBase> FireSound;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TObjectPtr<class UParticleSystem> FireEffect;

	//UFUNCTION()
	//void OnRep_Ammo();

	void SpendRound();

	// The number of unprocessed server requests for Ammo.
	// Incremented in SpendRound, decremented in ClientUpdateAmmo
	int32 Sequence = 0;

	//UPROPERTY(ReplicatedUsing = OnRep_Ammo, EditAnywhere, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	UPROPERTY(EditAnywhere, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	int32 Ammo;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	int32 MagCapacity;

	int32 CarriedAmmo;

	// Sound
//private:
//	UPROPERTY(EditAnywhere)
//	TObjectPtr<class USoundCue> PickupSound;

	// Enable of disable custom depth
public:
	virtual void EnableCustomDepth(bool bEnable) override;
	FVector TraceEndWithScatter(const FVector& HitTarget);

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	bool bUseScatter = false;
protected:
	// Trace end with scatter
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float DistanceToSphere = 800.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float SphereRadius = 10.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float AdditionalSphereRadius = 0.f;


};
