// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Pickable/Pickable.h"
#include "Item/Pickable/Weapon/WeaponTypes.h"
#include "Interfaces/WidgetBindDelegateInterface.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial = 0 UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped State"),
	EWS_EquippedSecondary UMETA(DisplayName = "Equipped Secondary"),
	EWS_Dropped UMETA(DisplayName = "Dropped State"),
	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};

/**
 *
 */

UCLASS()
class BLASTER_API AWeapon : public APickable, public IWidgetBindDelegateInterface
{
	GENERATED_BODY()
public:
	AWeapon();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_Owner() override;

	// Getters
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE float GetHeadShotDamage() const { return HeadShotDamage; }
	FORCEINLINE class USoundBase* GetTakeSound() { return PickupSound; }
	FORCEINLINE EWeaponName GetWeaponName() const { return WeaponName; }

	void SetHUDVisibility(bool IsVisible);
	virtual void SetHUD();

	virtual void ItemAttachToComponent(USceneComponent* SceneComponent, FAttachmentTransformRules& AttachmentRules, FName InSocketName);
	virtual void ItemDetachToComponent(FDetachmentTransformRules& DetachmentRules);

	virtual void Dropped();
	virtual void EnableCustomDepth(bool bEnable);

	bool bDestroyWeapon = false;

	void BindInit();
	// Interfaces
	virtual void IBindWidget(class UUserWidget* InUserWidget) override;


	TWeakObjectPtr<class UInventoryComponent> OwnerInventory;
protected:
	virtual void InitializeCollisionStates();


	UPROPERTY()
	class ACharacterBase* BlasterOwnerCharacter;
	UPROPERTY()
	class ABlasterPlayerController* BlasterOwnerController;

	 //components
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = true))
	//TObjectPtr<class UCapsuleComponent> OverlapCapsule;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UWeaponHUDComponent> WeaponHUDComponent;


protected:
	UFUNCTION()
	void OnPingTooHigh(bool bPingTooHigh);

protected:


	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere)
	float HeadShotDamage = 40.f;

	UPROPERTY(Replicated, EditAnywhere)
	bool bUseServerSideRewind = false;




	// State
public:
	void SetWeaponState(EWeaponState State);
	virtual void OnWeaponStateSet();
protected:
	UFUNCTION()
	void OnRep_WeaponState();

	virtual void OnEquipped();
	virtual void OnDropped();
	virtual void OnEquippedSecondary();

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon|Weapon Properties", meta = (AllowPrivateAccess = true))
	EWeaponState WeaponState = EWeaponState::EWS_Initial;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

protected:
	void ApplyForce(class UFieldSystemComponent* FieldSystemComponent, const FHitResult& HitPoint);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FieldSystem", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UFieldSystemComponent> FieldSystemComponent;

	// Widgets
protected:
	//UPROPERTY(VisibleAnywhere, Category = "Weapon|Weapon Properties")
	//TObjectPtr<class UWidgetComponent> PickupWidget;

	// Sound
protected:
	//UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = true))
	//TObjectPtr<class USoundCue> PickupSound;

protected:

	// Pickup Effect
	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<class UNiagaraComponent> PersistEffectComponent;

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<class UNiagaraSystem> PickupEffect;

	// Item Movement
protected:
	void Moving(float DeltaTime);

protected:
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = true))
	//float Amplitude = 2.f;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = true))
	//float Wavelength = 5.f;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = true))
	//float Speed = 1.f;

public:
	void ShowPickupWidget(bool bShowWidget);

protected:
	UPROPERTY(EditAnywhere)
	EWeaponName WeaponName;
};
