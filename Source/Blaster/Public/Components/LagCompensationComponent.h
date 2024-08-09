// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LagCompensationComponent.generated.h"

USTRUCT(BlueprintType)
struct FPhysAssetInformation
{
	GENERATED_BODY()
	FPhysAssetInformation()
		: BoneName(TEXT("")), HalfHeight(0.f), Radius(0.f), BoneWorldTransform(FTransform())
	{
	}

	UPROPERTY()
	FName BoneName;

	UPROPERTY()
	float HalfHeight;

	UPROPERTY()
	float Radius;

	UPROPERTY()
	FTransform BoneWorldTransform;
};

USTRUCT(BlueprintType)
struct FFramePackage
{
	GENERATED_BODY()
	FFramePackage()
		:Time(0.f), Character(nullptr)
	{
	}

	UPROPERTY()
	float Time;

	UPROPERTY()
	TMap<FName, FPhysAssetInformation> HitCapsuleInfo;

	UPROPERTY()
	class ACharacterBase* Character;
};

USTRUCT(BlueprintType)
struct FServerSideRewindResult
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 bHitComfirmed : 1 = 0;

	UPROPERTY()
	uint8 bHeadShot : 1 = 0;
};

USTRUCT(BlueprintType)
struct FShotgunServerSideRewindResult
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<ACharacterBase*, uint32> HeadShots;

	UPROPERTY()
	TMap<ACharacterBase*, uint32> BodyShots;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLASTER_API ULagCompensationComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class ACharacterBase;
public:
	ULagCompensationComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;


	UFUNCTION(Server, Reliable)
	void ServerScoreRequest(AActor* InHitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime);

	UFUNCTION(Server, Reliable)
	void ProjectileServerScoreRequest(ACharacterBase* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime);

	UFUNCTION(Server, Reliable)
	void ShotgunServerScoreRequest(const TArray<ACharacterBase*>& HitCharacters, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime);

private:
	////////////////
	// Initialize //
	////////////////
	void HitCapsuleConstruction(USkeletalMeshComponent* Mesh);
	void SetupHitCapsule(FPhysAssetInformation PhysicsAssetInfo, USkeletalMeshComponent* Mesh);
	void SaveFramePackage(FFramePackage& Package);
	void SaveFramePackage();
	void Draw(bool bDraw);
	void ShowFramePackage(const FFramePackage& Package, const FColor& Color);


	FServerSideRewindResult ServerSideRewind(class ACharacterBase* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime);

	void ServerSideRewind_Iter(class ACharacterBase* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime);

	FServerSideRewindResult ProjectileServerSideRewind(ACharacterBase* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime);

	// Shotgun
	FShotgunServerSideRewindResult ShotgunServerSideRewind(const TArray<ACharacterBase*>& HitCharacters, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime);

	FFramePackage GetFrameToCheck(ACharacterBase* HitCharacter, float HitTime);



	FFramePackage InterpBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime);

	FServerSideRewindResult ConfirmHit(const FFramePackage& Package, ACharacterBase* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation);

	FServerSideRewindResult ProjectileConfirmHit(const FFramePackage& Package, ACharacterBase* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime);

	FShotgunServerSideRewindResult ShotgunConfirmHit(
		const TArray<FFramePackage>& FramePackages,
		const FVector_NetQuantize& TraceStart,
		const TArray<FVector_NetQuantize>& HitLocations
	);

	void CacheCapsulePositions(ACharacterBase* HitCharacter, FFramePackage& OutFramePackage);
	void MoveCapsules(ACharacterBase* HitCharacter, const FFramePackage& Package);
	void ResetHitCapsules(ACharacterBase* HitCharacter, const FFramePackage& Package);
	void EnableCharacterMeshCollision(ACharacterBase* HitCharacter, ECollisionEnabled::Type CollisionEnable);




	UPROPERTY()
	class ACharacterBase* Character;
	//UPROPERTY()
	//class ABlasterPlayerController* Controller;

	TDoubleLinkedList<FFramePackage> FrameHistory;
	UPROPERTY(EditAnywhere)
	float MaxRecordTime = 4.f;


	//UPROPERTY()
	//TMap<FName, class UBoxComponent*> HitCollisionBoxes;

	//UPROPERTY()
	//TMap<FName, class UCapsuleComponent*> HitCollisionCapsules;

	//UPROPERTY(EditAnywhere)
	//TArray<UCapsuleComponent*> HitCapsuleBones;

	//UPROPERTY(EditAnywhere)
	//TArray<FPhysAssetInformation> PhysAssetInfo;
	UPROPERTY()
	TMap<FName, FPhysAssetInformation> HitCapsuleInfo;

	UPROPERTY(EditAnywhere)
	bool bDrawPhysAssets = false;
};
