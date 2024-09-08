// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlasterTypes/SkillAssistant.h"
#include "SkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillCoolTimeStartedDelegate, const FString&, InPrefix, int32, InIndex, float, InPlaybackSpeed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillCostChangedDelegate, int32, NumCost, const FString&, InMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoulCountChangedDelegate, int32, NumCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillCoolTimeCheckDelegate, ESkillAssistant, SkillIndex);


USTRUCT(BlueprintType)
struct FCoolTimeCheckStruct
{
	GENERATED_BODY()
	FCoolTimeCheckStruct(int32 InRequiredPoint = 0, float InCoolTime = 10.f)
		: RequiredPoint(InRequiredPoint), CoolTime(InCoolTime), bCanExecute(true), bSkillPointEnough(true)
	{
	}

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	//float CoolTimeCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float CoolTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 RequiredPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bCanExecute : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bSkillPointEnough : 1;

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
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void PostLoad() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void OnPlayMontageNotifyBeginFunc(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);


	// Total Actives (Skill)
	FOnSkillCoolTimeStartedDelegate OnSkillCoolTimeStarted;
	FOnSkillCostChangedDelegate OnSkillCostChanged;
	FOnSoulCountChangedDelegate OnSoulCountChanged;
	FOnSkillCoolTimeCheckDelegate OnSkillCoolTimeCheck;


	FORCEINLINE int32 GetSkillPoint() const { return SkillPoint; }
	void SetSkillPoint(int32 InSkillPoint);
	void AddSkillPoint(int32 InAddAmount);



	void SkillButtonPressed(int32 InIndex);


	void UltimateCast();

	void SkillCast(ESkillAssistant InSkillAssistant);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastCastEnd(ESkillAssistant InSkillAssistant);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSpawnAttributeAssistant(ESkillAssistant InSkillAssistant);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSpawnAttributeAssistantDetach(ESkillAssistant InSkillAssistant);


	void SkillCoolTimeEnded(const class UWidgetAnimation* InWidgetAnimation);
private:
	//////////////////
	// Owner Settings
	TWeakObjectPtr<class ACharacterBase> CharacterOwner;
	///////////////////




	void SpawnAttributeAssistant(ESkillAssistant InSkillAssistant);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnAttributeAssistant(ESkillAssistant InSkillAssistant);

	void SpawnAttributeAssistantDetach(ESkillAssistant InSkillAssistant);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnAttributeAssistantDetach(ESkillAssistant InSkillAssistant);

	UFUNCTION(Server, Reliable)
	void ServerProcedure(ESkillAssistant InSkillAssistant, class UAnimMontage* InMontage);


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AHealArea> HealAreaClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class AHealArea> HealArea;

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
	TMap<int32, ESkillAssistant> SkillList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TMap<ESkillAssistant, FCoolTimeCheckStruct> CoolTimeMap;

	TArray<bool> SkillButtonPressedChecker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<int32> NeededSkillPoints;

	UFUNCTION()
	void OnRep_CurrentSkill();
	UPROPERTY(ReplicatedUsing = OnRep_CurrentSkill)

	ESkillAssistant CurrentSkill;

private:
	void InitForWaiting();

	uint8 IsSkillCostChangedBroadcasted : 1;


	// Anim
public:
	UFUNCTION()
	void OnRep_CurrentMontage();

	UPROPERTY(ReplicatedUsing = OnRep_CurrentMontage)
	TObjectPtr<class UAnimMontage> CurrentMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> SkillCastingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> UltimateMontage;

private:
	void UltimateCastFinished();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	int32 WeaponMaterialIndex = 4.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	int32 UltimateSustainingTime = 10.f;

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

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class AWeapon> TempWeapon;
};
