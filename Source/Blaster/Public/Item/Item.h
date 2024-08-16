// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Hovering = 0 UMETA(DisplayName = "Hovering"),
	EIS_Equipped UMETA(DisplayName = "Equipped")
};

UCLASS()
class BLASTER_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//// When this function called, it means that this class is going to be attached to another actor's mesh.
	//virtual void ItemAttachToComponent(USceneComponent* SceneComponent, FAttachmentTransformRules& AttachmentRules, FName InSocketName);
	//virtual void ItemDetachToComponent(FDetachmentTransformRules& DetachmentRules);


	FORCEINLINE class USoundCue* GetTakeSound() { return PickupSound; }
	//FORCEINLINE class UCapsuleComponent* GetCapsuleComponent() { return OverlapCapsule; }
	FORCEINLINE class UCapsuleComponent* GetAreaCapsule() const { return OverlapCapsule; }
	FORCEINLINE class UWidgetComponent* GetPickupWidget() const { return PickupWidget; }


	void ShowPickupWidget(bool bShowWidget);

	// Initialize
protected:
	virtual void InitializeCollisionStates();
	virtual void InitializeDelegates();
	virtual void InitializeDefaults();

protected:
	//UFUNCTION()
	//virtual void OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//virtual void OnCapsuleEndOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Components
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCapsuleComponent> OverlapCapsule;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaticMesh", meta = (AllowPrivateAccess = true))
	//TObjectPtr<class UStaticMeshComponent> PickupMesh;

	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<class USceneComponent> DefaultComponent;

	// Item Movement
protected:
	void Moving(float DeltaTime);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = true))
	float Amplitude = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = true))
	float Wavelength = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = true))
	float Speed = 0.1f;

	// State
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemState", meta = (AllowPrivateAccess = true))
	EItemState ItemState = EItemState::EIS_Hovering;


protected:

	// Pickup Effect
	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<class UNiagaraComponent> PersistEffectComponent;

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<class UNiagaraSystem> PickupEffect;

	// Sound
protected:
	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundCue> PickupSound;

	// Widgets
protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon|Weapon Properties")
	TObjectPtr<class UWidgetComponent> PickupWidget;
};
