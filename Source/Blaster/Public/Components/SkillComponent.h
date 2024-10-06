// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlasterTypes/SkillAssistant.h"
#include "GameData/SkillStat.h"
#include "GameData/SkillData.h"
#include "SkillComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnSkillAnimStartedDelegate, ESkillAnimType /*InPrefix*/, int32 /*InIndex*/, float /*InPlaybackSpeed*/);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSkillCostChangedDelegate, int32 /*NumCost*/, const FString& /*InMessage*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSoulCountChangedDelegate, int32 /*NumCount*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillCoolTimeCheckDelegate, int32 /*SkillIndex*/);


USTRUCT(BlueprintType)
struct FCoolTimeCheckStruct
{
	GENERATED_BODY()

	FCoolTimeCheckStruct()
		: bCanExecute(true), bSkillPointEnough(true)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bCanExecute : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bSkillPointEnough : 1;

};

USTRUCT(BlueprintType)
struct FSkillManagementStruct
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FSkillData SkillData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FSkillStat SkillStat;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FCoolTimeCheckStruct CoolTimeCheckStruct;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLASTER_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USkillComponent();

protected:
	// Called when the game starts
	virtual void InitializeComponent() override;

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void PostLoad() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void OnPlayMontageNotifyBeginFunc(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);


	// Total Actives (Skill)
	FOnSkillAnimStartedDelegate OnSkillAnimStarted;
	FOnSkillCostChangedDelegate OnSkillCostChanged;
	FOnSoulCountChangedDelegate OnSoulCountChanged;
	FOnSkillCoolTimeCheckDelegate OnSkillCoolTimeCheck;

	FORCEINLINE int32 GetSkillPoint() const { return SkillPoint; }
	void SetSkillPoint(int32 InSkillPoint);
	void AddSkillPoint(int32 InAddAmount);



	void SkillButtonPressed(int32 InIndex);


	void UltimateCast();

	void SkillCast(int32 InCurrentSkillIndex);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastCastEnd(int32 InCurrentSkillIndex);


	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSpawnAttributeAssistantDetach(int32 InCurrentSkillIndex);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastMaterialChange();

	void SkillAnimFinished(const class UWidgetAnimation* InWidgetAnimation);
private:
	//////////////////
	// Owner Settings
	TWeakObjectPtr<class ABlasterCharacter> CharacterOwner;
	///////////////////




	void SpawnAttributeAssistant(int32 InCurrentSkillIndex);

	void Inflict();

	void Inflict_Belica();
	void Inflict_Belica_SkillQ();
	void Inflict_Belica_FireBullet();

	void Inflict_Murdock();
	void Inflict_Murdock_SkillQ();
	void Inflict_Murdock_SkillW();


	void SpawnAttributeAssistantDetach(int32 InCurrentSkillIndex);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnAttributeAssistantDetach(int32 InCurrentSkillIndex);

	void ProcedureFunc(int32 InCurrentSkillIndex, UAnimMontage* InMontage);
	UFUNCTION(Server, Reliable)
	void ServerProcedure(int32 InCurrentSkillIndex, class UAnimMontage* InMontage);

	//UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	//TSubclassOf<class AActor> SpawnActorClass;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	//TObjectPtr<class AActor> SpawnActor;


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AHealArea> HealAreaClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class AHealArea> Spawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AShieldBarrier> ShieldBarrierClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class AShieldBarrier> ShieldBarrier;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AEnemyRange> EnemyRangeClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class AEnemyRange> EnemyRange;

	UFUNCTION()
	void OnRep_SkillPoint();
	UPROPERTY(ReplicatedUsing = OnRep_SkillPoint, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 SkillPoint;


	// CoolTime
	void CoolTimeChecker(float DeltaTime);

	void InitializeCoolTimeMap();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TMap<int32, ESkillType> SkillList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TMap<int32, FSkillManagementStruct> CoolTimeMap;

	UPROPERTY()
	TArray<bool> SkillButtonPressedChecker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<int32> NeededSkillPoints;



	ESkillType CurrentSkill;

	int32 CurrentSkillIndex;

private:
	void InitForWaiting();

	uint8 IsSkillCostChangedBroadcasted : 1;

	// Anim
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayMontage(class UAnimMontage* InMontage);

public:
	TObjectPtr<class UAnimMontage> CurrentMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> SkillCastingMontage_A;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> SkillCastingMontage_B;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> SkillCastingMontage_C;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> UltimateMontage;

	// Ultimate Skill
private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastUltimateCastFinished();
	void UltimateCastFinishedDelay();


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	int32 RedWeaponMaterialIndex = 4;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	int32 BlueWeaponMaterialIndex = 4;

	int32 WeaponMaterialIndex = 0;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMaterial> UltimateWeaponMaterial_Red;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMaterial> UltimateWeaponMaterial_Blue;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMaterial> TransparentMaterial;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UNiagaraComponent> UltimateEffectComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AWeapon> TransparentWeaponClass;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class AWeapon> TransparentWeapon;

	UPROPERTY(Replicated, VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class AWeapon> TempWeapon;
};
