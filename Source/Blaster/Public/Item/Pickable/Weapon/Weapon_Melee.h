// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Item/Item.h"
#include "Item/Pickable/Weapon/Weapon.h"

#include "Weapon_Melee.generated.h"

/**
 *
 */
 //UENUM(BlueprintType)
 //enum class EWeaponType : uint8
 //{
 //	EWT_OneHanded = 0 UMETA(DisplayName = "OneHanded"),
 //	EWT_TwoHanded UMETA(DisplayName = "TwoHanded"),
 //	EWT_None UMETA(DisplayName = "None")
 //};

UCLASS()
class BLASTER_API AWeapon_Melee : public AWeapon
{
	GENERATED_BODY()
public:
	AWeapon_Melee();

	virtual void BeginPlay() override;

	//FORCEINLINE class USoundBase* GetEquipSound() { return PickupSound; }
	FORCEINLINE class UBoxComponent* GetBoxComponent() { return BoxComponent; }
	FORCEINLINE void ClearActorsToIgnore() { ActorsToIgnore.Empty(); }
	FORCEINLINE class UStaticMeshComponent* GetPickupMesh() const { return PickupMesh; }

	virtual void ItemAttachToComponent(USceneComponent* SceneComponent, FAttachmentTransformRules& AttachmentRules, FName InSocketName) override;
	virtual void ItemDetachToComponent(FDetachmentTransformRules& DetachmentRules) override;


protected:
	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	//UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	//TObjectPtr<class USoundBase> PickupSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> BoxTraceStart;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> BoxTraceEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaticMesh", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> PickupMesh;



	TArray<class AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = true))
	float DamageAmount = 20.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	uint8 bShowBoxDebug : 1;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	FVector TraceHalfSize = FVector(5.f);

protected:
	virtual void InitializeCollisionStates() override;
	virtual void InitializeDelegates();

	void HitTrace();
};
