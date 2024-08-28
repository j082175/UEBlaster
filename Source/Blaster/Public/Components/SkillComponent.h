// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillStartedDelegate, const FString&, InPrefix, int32, InIndex, float, InPlaybackSpeed);

USTRUCT(BlueprintType)
struct FCoolTimeCheckStruct
{
	GENERATED_BODY()
	FCoolTimeCheckStruct(float CoolTime = 10.f)
		: CoolTime(CoolTime), CoolTimeCount(0.f), bCanExecute(true)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float CoolTimeCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float CoolTime = 2.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bCanExecute : 1 = true;
};

UENUM(BlueprintType)
enum class ESkillAssistant : uint8
{
	ESA_HealArea = 0 UMETA(DisplayName = "HealAreaClass"),
	ESA_ShieldRecovery UMETA(DisplayName = "ShieldRecovery"),

	ESA_MAX UMETA(DisplayName = "Default_MAX")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Total Actives (Skill)
	FOnSkillStartedDelegate OnSkillStarted;

	FORCEINLINE int32 GetSkillPoint() const { return SkillPoint; }
	FORCEINLINE void SetSkillPoint(int32 InSkillPoint) { SkillPoint = InSkillPoint; }
	FORCEINLINE void AddSkillPoint(int32 InAddAmount) { SkillPoint += InAddAmount; }

	void SkillButtonPressed(int32 InIndex);


	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSpawnAttributeAssistant(ESkillAssistant InSkillAssistant);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSpawnAttributeAssistantDetach(ESkillAssistant InSkillAssistant);

private:
	//////////////////
	// Owner Settings
	class ACharacterBase* CharacterOwner;
	///////////////////


	void SpawnAttributeAssistant(ESkillAssistant InSkillAssistant);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnAttributeAssistant(ESkillAssistant InSkillAssistant);

	void SpawnAttributeAssistantDetach(ESkillAssistant InSkillAssistant);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnAttributeAssistantDetach(ESkillAssistant InSkillAssistant);

	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AHealArea> HealAreaClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<class AHealArea> HealArea;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 SkillPoint;


	// CoolTime
	void CoolTimeChecker(float DeltaTime);

	void InitializeCoolTimeMap();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TMap<FString, FCoolTimeCheckStruct> CoolTimeMap;

	TArray<bool> SkillButtonPressedChecker;

};
