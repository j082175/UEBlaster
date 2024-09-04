// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
#include "Actor/PooledCharacter.h"

// Enums
#include "BlasterTypes/AnimState.h"
#include "BlasterTypes/CombatState.h"
#include "BlasterTypes/Team.h"
#include "BlasterTypes/TurningInPlace.h"
#include "BlasterTypes/CharacterState.h"
#include "BlasterTypes/AttackTypes.h"
#include "Item/Pickable/Weapon/WeaponTypes.h"

#include "HUD/BlasterHUD.h"

// Interfaces
#include "Interfaces/HitInterface.h"
#include "Interfaces/WidgetBindDelegateInterface.h"
#include "Interfaces/CanParryInterface.h"
#include "Interfaces/TeamInterface.h"
//#include "Interfaces/ObjectPoolInterface.h"
//#include "Actor/PooledObject.h"

// Structs
#include "Components/TimelineComponent.h"

#include "CharacterBase.generated.h"

typedef int32 AmmoAmountInt;


UCLASS()
class BLASTER_API ACharacterBase : public APooledCharacter, public IHitInterface, public IWidgetBindDelegateInterface, public ICanParryInterface, public ITeamInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Overrides
protected:
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Destroyed() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	virtual void OnConstruction(const FTransform& Transform);
	virtual void PostLoad() override;


public:
	// Getters
	FORCEINLINE class UCombatComponent* GetCombatComponent() const { return CombatComponent; }
	FORCEINLINE class UAttributeComponent* GetAttributeComponent() const { return AttributeComponent; }
	FORCEINLINE class UBuffComponent* GetBuffComponent() const { return BuffComponent; }
	FORCEINLINE class ULagCompensationComponent* GetLagCompensationComponent() const { return LagCompensation; }
	FORCEINLINE class UHpBarWidgetComponent* GetHpBarWidgetComponent() const { return HpBarWidgetComponent; }
	FORCEINLINE class UMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComponent; }
	FORCEINLINE class UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }

	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE uint8 ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE uint8 GetDisableGameplay() const { return bDisableGameplay; }
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE uint8 GetIsRagdollStateStopped() const { return bIsRagdollStateStopped; }

	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }
	FORCEINLINE class UOverheadWidget* GetOverheadWidget() const { return OverheadWidget; }

	ECombatState GetCombatState() const;
	class AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
	class AWeapon* GetSecondaryEquippedWeapon() const { return SecondaryWeapon; }

	ECharacterState GetCharacterState() const;
	EAnimState GetAnimState() const { return AnimState; }
	FVector GetHitTarget() const;

	UFUNCTION(BlueprintCallable)
	virtual ETeam IGetTeam() const override;

	FORCEINLINE bool IsSprint() const { return bIsSprint; }
	FORCEINLINE bool IsElimmed() const { return bIsElimmed; }
	bool IsWeaponEquipped();
	bool IsAiming();
	bool IsLocallyReloading();
	bool IsHoldingTheFlag() const;

	// Setters
	UFUNCTION(BlueprintCallable)
	void ISetTeam(ETeam InNewTeam) override;
	void SetHoldingTheFlag(bool bHolding);
	FORCEINLINE void SetOverlappingWeapon(class AWeapon* InWeapon) { OverlappingWeapon = InWeapon; }


	FORCEINLINE void SetCombatState(ECombatState InCombatState) { CombatState = InCombatState; }

	void SetEquippedWeapon(class AWeapon* W) { EquippedWeapon = W; }
	void SetSecondaryEquippedWeapon(class AWeapon* W) { SecondaryWeapon = W; }



	// Initializes
protected:
	virtual void InitializeCollisionStates();
	virtual void InitializeWidgets();
	virtual void InitializeDelegates();
	virtual void InitializeDefaults();
	virtual void PollInit();
	void InitializeCarriedAmmo();

	// For Binding
protected:
	UFUNCTION()
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	//virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual void OnPlayMontageNotifyBeginFunc(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION(BlueprintCallable)
	virtual void OnMontageEndedFunc(UAnimMontage* Montage, bool bInterrupted);


	// Pre-Stored Variables
protected:
	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;

	UPROPERTY()
	class ABlasterPlayerState* BlasterPlayerState;

	// Interface Functions
public:
	// IHitInterface
	virtual void IGetHit(const FVector& HitPoint, const FHitResult& InHitResult) override;
	virtual void IBindWidget(class UUserWidget* InUserWidget) override;

	//virtual void IBindWidget(class UWidgetComponent* InWidgetComponent) override;

	virtual bool IIsParring() override;
	virtual bool ICheckParry(AActor* OtherActor) override;
	//virtual void IBindDespawnFunc(class UObjectPoolComponent* InActorComponent) override;


	// Components
public:
	UPROPERTY(VisibleAnywhere)
	TMap<FName, class UCapsuleComponent*> HitCollisionCapsules;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBuffComponent> BuffComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<class ULagCompensationComponent> LagCompensation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMantleVaultComponent> MantleVaultComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HpBarWidget", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UHpBarWidgetComponent> HpBarWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HpBarWidget", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UPawnNoiseEmitterComponent> PawnNoiseEmitterComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMyAIPerceptionStimuliSource> AIPerceptionStimuliSource;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USkillComponent> SkillComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInventoryComponent> InventoryComponent;

	// Sounds
protected:
	void PlayEquipWeaponSound();
	void PlaySecondaryWeaponSound();

	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundCue> ElimBotSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundBase> GroundSound;


	// Particles
protected:
	UPROPERTY(EditAnywhere, Category = "Effect", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystem> BloodEffect;

	UPROPERTY(EditAnywhere, Category = "Effect", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystem> ElimBotEffect;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UParticleSystemComponent> ElimBotEffectComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystem> CharacterSpawnEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystem> ParryEffect;

	// Elimmed
protected:
	//UFUNCTION(BlueprintNativeEvent)
	virtual void SetDead();

	// Elim bot
public:
	virtual void Elim(bool bPlayerLeftGame);

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastElim(bool bPlayerLeftGame);

protected:
	void DropOrDestroyWeapon(AWeapon* InWeapon);



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State", meta = (AllowPrivateAccess = true))
	uint8 bIsElimmed : 1 = false;

	// Animations
protected:
	void PlayHitReactMontage(const FVector& HitPoint);

	virtual void PlayMontage(class UAnimInstance* AnimInstance, class UAnimMontage* Montage, FName SectionName, int32 Rand);
	virtual void PlayRandomMontage(class UAnimInstance* AnimInstance, class UAnimMontage* Montage, FString Prefix);

public:
	void PlayFireMontage(bool bIsAiming);
	void PlayDeadMontage();
	//void PlayCombatHitReactMontage();
	void PlayReloadMontage();
	void PlayThrowGrenadeMontage();
	void PlaySwapMontage();
	void PlaySlideMontage();
	void PlayDodgeMontage(FName SectionName);
	void PlayVaultMontage(FName SectionName);
	void PlayMantleMontage(FName SectionName);
	void PlayBoltActionMontage(FName SectionName);

public:
	UFUNCTION(NetMulticast, Reliable)
	void JumpToShotgunEnd();

	bool bFinishedSwapping = false;

	virtual void RandomAttack(int32 Index, const FString& AttackType);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRandomAttack(int32 Index, const FString& AttackType);

protected:
	UPROPERTY(EditAnywhere, Category = "Anim", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Anim", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> EquipMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> ReloadMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> ThrowGrenadeMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> SwapMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> SlideMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> DodgeMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> VaultMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> MantleMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> BoltActionMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> DashMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> StunMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> ParriedMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> MeleeMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> SweepFallMontage;

	// About Items
protected:
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(ECollisionEnabled::Type Type);

	void AttachToSocket(class AItem* Item, class USkeletalMeshComponent* SkeletalMesh, FName SocketName);


	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = true))
	TObjectPtr<class AWeapon_Melee> Weapon;

	// About State
protected:
	void SetState(ECharacterState InCharacterState, ECombatState InCombatState);

	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadWrite, Category = "State", meta = (AllowPrivateAccess = true))
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UFUNCTION()
	void OnRep_CombatState();

	UPROPERTY(ReplicatedUsing = OnRep_CombatState, VisibleInstanceOnly, BlueprintReadWrite, Category = "State", meta = (AllowPrivateAccess = true))
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	// For Calculate
protected:
	FName CalculateHitDirection(const FVector& HitPoint);
	void AO_PitchMappingForClient();
	void RotateInPlace(float DeltaTime); // for Server's RotateInPlace
	void SimProxiesTurn();// for Client's RotateInPlace
	float CalculateSpeed();

	void TurnInPlace(float DeltaTime);



	float InterpAO_Yaw;

	UPROPERTY(EditAnywhere)
	float CameraThreshold = 100.f;

	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotationCurrentFrame;
	float ProxyYawDelta;
	float TimeSinceLastMovementReplication;

	UPROPERTY(Replicated)
	float AO_Yaw;

	UPROPERTY(Replicated)
	float AO_Pitch;

	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	// 
public:
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastGainedTheLead();

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastLostTheLead();


	// Dissolve effect for Elim
protected:
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();
protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTimelineComponent> DissolveTimelineComponent;

	// Dynamic Instance that we can change at runtime
	UPROPERTY(VisibleAnywhere, Category = "Elim")
	TObjectPtr<UMaterialInstanceDynamic> DynamicDissolveMaterialInstance;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;
	// Material instance set on the Blueprint, used with the dynamic material instance
	UPROPERTY(EditAnywhere, Category = "Elim")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	FOnTimelineFloat DissolveTrack;


	// Team Colors
public:
	void SetTeamColor(ETeam InTeam);
protected:

	UPROPERTY(EditAnywhere, Category = "Elim")
	TObjectPtr<class UMaterialInstance> RedDissolveMatInst;

	UPROPERTY(EditAnywhere, Category = "Elim")
	TObjectPtr<class UMaterialInstance> RedMaterial;

	UPROPERTY(EditAnywhere, Category = "Elim")
	TObjectPtr<class UMaterialInstance> BlueDissolveMatInst;

	UPROPERTY(EditAnywhere, Category = "Elim")
	TObjectPtr<class UMaterialInstance> BlueMaterial;

	UPROPERTY(EditAnywhere, Category = "Elim")
	TObjectPtr<class UMaterialInstance> OriginalMaterial;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USkeletalMesh> RedTeamSKMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USkeletalMesh> BlueTeamSKMesh;

	// Respawn
protected:
	void Recover();

	void SetSpawnPoint();

	virtual void ElimTimerFinished();
	FTimerHandle ElimTimer;
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;



	// Niagara Effects
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> CrownNiagaraSystem;

	UPROPERTY()
	TObjectPtr<class UNiagaraComponent> CrownNiagaraComponent;


protected:


	UPROPERTY(Replicated, BlueprintReadOnly)
	uint8 bDisableGameplay : 1;


	UPROPERTY(Replicated)
	EAnimState AnimState = EAnimState::EAS_UnEquipped;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsSprint : 1;


	virtual void CameraShake(const TSubclassOf<UCameraShakeBase>& CameraShake, bool bTrue);

	// Dash
public:

protected:
	void InitDashCurve();
	//각각 Dash를 시작할 때, 끝날 때 사용할 함수 목록
	UFUNCTION()
	void DashStart(float Output);

	UFUNCTION()
	void DashEnd();

	UFUNCTION(Server, Reliable)
	void ServerDash(FName InSectionName);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDash(FName InSectionName);
	void DashFunc(FName InSectionName);
	bool Dash(FName InSectionName);
	bool CheckSpUnder(float InCurrentSp, float InCost);

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCurveFloat> DashCurve;

	FOnTimelineFloat DashTimeLineUpdateDelegate;
	FOnTimelineEvent DashTimeLineFinishDelegate;
	//타임라인
	FTimeline DashTimeline;

	// Parry
protected:

	UFUNCTION(Server, Reliable)
	void ServerCheckParry(AActor* OtherActor);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastCheckParry(AActor* OtherActor);

	uint8 bIsParring : 1;

	UPROPERTY(Replicated)
	uint8 bParryResultCheck : 1;


	AActor* LastDamageCauser;


	virtual void UpdateMotionWarpingTransform();

	FVector CharacterHitPoint;


	// Attacks
protected:
	void GroundAttack(FName OpponentTag);

	UPROPERTY(EditAnywhere)
	float GroundAttackDamage = 200.f;


	// Fire
public:
	virtual void Fire(bool bPressed);

protected:
	virtual void FireProjectileWeapon(bool bPressed);
	virtual void FireHitScanWeapon(bool bPressed);
	virtual void FireShotgun(bool bPressed);
	void ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets);
	void StartFireTimer();
	void FireTimerFinished();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire(bool bPressed, const FVector_NetQuantize& TraceHitTarget, float FireDelay);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(bool bPressed, const FVector_NetQuantize& TraceHitTarget);
	void LocalFire(bool bPressed, const FVector_NetQuantize& TraceHitTarget);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets);
	bool CanFire();

	FTimerHandle FireTimer;
	uint8 bIsFiring : 1;
	uint8 bFireButtonPressed : 1;
	uint8 bReloadStopCheck : 1;


protected:
	FVector HitTarget;






	void EnableHitCapsulesCollision();
	void DisableHitCapsulesCollision();

	// Camera Shake

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> GroundCameraShakeClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> FireCameraShakeClass;



	void ShowAttachedGrenade(bool bShowGrenade);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> AttachedGrenade;

	// Dodge
public:

protected:
	bool Dodge(FName InSectionName);
	void DodgeFunc(FName InSectionName);

	UFUNCTION(Server, Reliable)
	void ServerDodge(FName InSectionName);

	UFUNCTION()
	void OnRep_Dodge();

	UPROPERTY(ReplicatedUsing = OnRep_Dodge)
	uint8 bIsDodge : 1;
	UPROPERTY(ReplicatedUsing = OnRep_Sliding)
	uint8 bIsSliding : 1;
	uint8 bLocallyReloading : 1;

	UPROPERTY(Replicated)
	FName KeySectionName;


	// Montage Notifies
protected:
	virtual void FinishReloading();
	void ShotgunShellReload();
	virtual void ThrowGrenadeFinished();
	void DisableAttachedGrenade();
	virtual void FinishSwapAttachWeapons();


	// Mantle or Vault
protected:
	void VaultOrMantle();
	bool VaultOrMantleFunc();
	UFUNCTION(Server, Reliable)
	void ServerVaultMantle();

	UFUNCTION()
	void OnRep_VaultOrMantle();

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	uint8 bIsMorV : 1;
	UPROPERTY(VisibleAnywhere)
	uint8 bCanMorV : 1;


	// Spawn
public:
	void SpawnDefaultWeapon();

protected:
	virtual void SpawnMagic(FName SocketToSpawn);
	void SpawnCharacter(FVector SpawnLocation);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> DefaultWeaponClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> CharacterToSpawn;

	// Ammos
public:
	void PickupAmmo(EWeaponType InWeaponType, int32 AmmoAmount);

protected:
	virtual void UpdateCarriedAmmo();
	UFUNCTION()
	void OnRep_CarriedAmmo();

	virtual void UpdateAmmoValues();
	void UpdateShotgunAmmoValues();

	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo, EditAnywhere)
	int32 CarriedAmmo;

	UPROPERTY(VisibleAnywhere)
	TMap<EWeaponType, int32> CarriedAmmoMap;

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



	// Sniper Rifle's Bolt Action
protected:
	UFUNCTION(Server, Reliable)
	void ServerBolt();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastBolt();
	void Bolt();

	// Attach Helpers
protected:
	void AttachActorToRightHand(AActor* ActorToAttach);
	void AttachActorToBackpack(AActor* ActorToAttach);
	void AttachActorToLeftHand(AActor* ActorToAttach);
	void AttachFlagToLeftHand(AWeapon* InFlag);
	void DropEquippedWeapon();

	// Throw Grenade && Grenade Launcher
protected:
	void LaunchGrenade();

	UFUNCTION(Server, Reliable)
	void ServerLaunchGrenade(const FVector_NetQuantize& Target);

	UFUNCTION()
	void OnRep_Grenades();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectileGrenade> GrenadeClass;

	UFUNCTION(Server, Reliable)
	void ServerThrowGrenade();

	void ThrowGrenade();


	UPROPERTY(ReplicatedUsing = OnRep_Grenades, EditAnywhere)
	float Grenades;
	UPROPERTY(EditAnywhere)
	float MaxGrenades;




	// Flags
protected:
	UFUNCTION()
	void OnRep_HoldingTheFlag();

	UPROPERTY(Replicated)
	uint8 bHoldingTheFlag : 1;

	UPROPERTY(Replicated, EditAnywhere)
	TObjectPtr<class AWeapon> Flag;

	// Slide
protected:
	void Slide();
	void SlideFunc();
	UFUNCTION(Server, Reliable)
	void ServerSlide();

	UFUNCTION()
	void OnRep_Sliding();

	// Weapon Aiming
protected:
	void AimOffset(float InDeltaTime);

	virtual void SetAiming(bool InbIsAiming);

	void SetAimingFunc(bool InbIsAiming);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetAiming(bool InbIsAiming);
	UFUNCTION()
	void OnRep_Aiming_Finished();

	uint8 bAimButtonPressed : 1;

	UPROPERTY(Replicated, VisibleAnywhere)
	uint8 bIsAiming : 1;

	// Character Movement
protected:
	void SetMovementSpeed(float WalkSpeed, float CrouchSpeed);

	virtual void OnRep_ReplicatedMovement() override;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float AimWalkSpeed = 450.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float AimCrouchSpeed = 200.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float BaseWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float BaseCrouchSpeed = 300.f;




	virtual void Jump() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileMagicClass;
	float ProjectileMagicDamage = 100.f;


	// Weapon
protected:

	void EquipWeapon(AWeapon* InWeapon);
	void EquipSecondaryFunc();
	virtual void EquipWeaponFunc();
	bool ShouldSwapWeapons();
	void SwapWeapons();
	UFUNCTION()
	void OnRep_EquippedWeapon();
	UFUNCTION()
	void OnRep_SecondaryWeapon();
	void ReloadEmptyWeapon();

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class AWeapon> EquippedWeapon;
	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon, EditAnywhere)
	TObjectPtr<class AWeapon> SecondaryWeapon;


	// Weapon
protected:
	UFUNCTION()
	void OnRep_OverlappingWeapon(class AWeapon* LastWeapon);

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon, VisibleAnywhere, Category = "Repl", meta = (AllowPrivateAccess = true))
	TObjectPtr<class AWeapon> OverlappingWeapon;

	// Reload
protected:
	void Reload();

	void HandleReload();

	UFUNCTION(Server, Reliable)
	void ServerReload();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastReload();

	int32 AmountToReload();


	float CrosshairShootingFactor;

	// Melee
protected:
	void Melee();
	void MeleeFunc();
	UFUNCTION(Server, Reliable)
	void ServerMelee();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastMelee();

	// For Ragdoll
protected:
	void RagdollTick(float DeltaTime);

	void DoRagdollImpulse();
	void CalculateMeshLocation();
	void SetRagdollCollision();

	UFUNCTION(Server, Reliable)
	void ServerExecuteRagdollForMulti();

	void ExecuteRagdollForMulti();

	UFUNCTION(Server, Reliable)
	void ServerExecuteRagdoll();

	UFUNCTION(BlueprintCallable) // For Testing
		void ExecuteRagdoll();
	void ExecuteRagdollFunc();

	UFUNCTION(Server, Reliable)
	void ServerExecutePhysicsRecover();
	UFUNCTION(BlueprintCallable)
	void ExecutePhysicsRecover();// For Testing
	void ExecutePhysicsRecoverFunc();


	void CalculateFacingDirection();
	void SetupGetupOrientation();
	void SetupGetupMontage();
	void CachePose(); // Cache the Ragdoll Pose / Setup Orientation and Montage
	void CheckCapsuleAndMeshEquals(float DeltaTime, float InAccuracy);

	UPROPERTY(VisibleAnywhere)
	float CheckCapsuleAndMeshTimer = 1.f;
	UPROPERTY(EditAnywhere)
	float CheckCapsuleAndMeshThreshold = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SnapshotName = TEXT("FinalPose");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector CapsuleLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector MeshLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector LastCapsuleLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bIsFacingUp : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	uint8 bIsRagdollStateStopped : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	uint8 bIsRagdollComplete : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bRagdollInProgress : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> GetUpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> DownMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> UpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> UpMontageForMultiplayer;


	// Team , for non-local players
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	ETeam Team = ETeam::ET_NoTeam;

	// Widgets
protected:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UOverheadWidget> OverheadWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UOverheadWidgetComponent> OverheadWidgetComponent;



};
