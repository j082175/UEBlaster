// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LagCompensationComponent.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "Components/CapsuleComponent.h"
#include "Characters/BlasterCharacter.h"
#include "Item/Pickable/Weapon/Weapon_Gun.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InventoryComponent.h"
#include "Blaster/Blaster.h"

// Sets default values for this component's properties
ULagCompensationComponent::ULagCompensationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;

	// ...
}

void ULagCompensationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//if (Character && Character->GetController() && Character->GetController()->IsLocalPlayerController())
	{
		SaveFramePackage();
		Draw(bDrawPhysAssets);

		//AB_SUBLOG(LogABDisplay, Warning, TEXT(""));
	}


	//if (Character && Character->HasAuthority() && Character->IsLocallyControlled() && HitCollisionCapsules.Contains(TEXT("head")))
	//{
	//	UCapsuleComponent* C = HitCollisionCapsules[TEXT("head")];
	//	if (C)
	//	{
	//		FTransform T;
	//		T.SetLocation(FVector(0.f, 0.f, 0.f));
	//		C->SetWorldTransform(T);
	//	}
	//}

}

// Called when the game starts
void ULagCompensationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (Character)
	{
		HitCapsuleConstruction(Character->GetMesh());

		InventoryComponent = Character->GetComponentByClass<UInventoryComponent>();
	}
	//FFramePackage Package;
	//SaveFramePackage(Package);
	//ShowFramePackage(Package, FColor::Orange);
}

void ULagCompensationComponent::ServerScoreRequest_Implementation(AActor* InHitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime)
{

	//UE_LOG(LogTemp, Display, TEXT("TraceStart : %f, %f, %f"), TraceStart.X, TraceStart.Y, TraceStart.Z);
	//UE_LOG(LogTemp, Display, TEXT("HitLocation : %f, %f, %f"), HitLocation.X, HitLocation.Y, HitLocation.Z);

	ACharacterBase* HitCharacter = Cast<ACharacterBase>(InHitCharacter);
	if (HitCharacter && HitCharacter->IsElimmed()) return;

	FServerSideRewindResult Confirm = ServerSideRewind(HitCharacter, TraceStart, HitLocation, HitTime);

	if (Character && HitCharacter && Confirm.bHitComfirmed && InventoryComponent->GetEquippedWeapon())
	{
		//UE_LOG(LogTemp, Display, TEXT("HitCharacter : %s"), *HitCharacter->GetName());
		const float Damage = Confirm.bHeadShot ? InventoryComponent->GetEquippedWeapon()->GetHeadShotDamage() : InventoryComponent->GetEquippedWeapon()->GetDamage();

		//UE_LOG(LogTemp, Display, TEXT("Damage : %f"), Character->GetEquippedWeapon()->GetDamage());
		UGameplayStatics::ApplyDamage(HitCharacter, Damage, Character->GetController(), InventoryComponent->GetEquippedWeapon(), UDamageType::StaticClass());
	}
}

void ULagCompensationComponent::ProjectileServerScoreRequest_Implementation(ACharacterBase* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
	if (HitCharacter && HitCharacter->IsElimmed()) return;

	FServerSideRewindResult Confirm = ProjectileServerSideRewind(HitCharacter, TraceStart, InitialVelocity, HitTime);

	//UE_LOG(LogTemp, Warning, TEXT("Confirm.bHitComfirmed : %d"), Confirm.bHitComfirmed);

	if (Character && HitCharacter && Confirm.bHitComfirmed && InventoryComponent->GetEquippedWeapon())
	{
		const float Damage = Confirm.bHeadShot ? InventoryComponent->GetEquippedWeapon()->GetHeadShotDamage() : InventoryComponent->GetEquippedWeapon()->GetDamage();

		UGameplayStatics::ApplyDamage(HitCharacter, Damage, Character->GetController(), InventoryComponent->GetEquippedWeapon(), UDamageType::StaticClass());
	}
}

void ULagCompensationComponent::ShotgunServerScoreRequest_Implementation(const TArray<ACharacterBase*>& HitCharacters, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime)
{

	//UE_LOG(LogTemp, Display, TEXT("ShotgunServerScoreRequest_Implementation"));
	FShotgunServerSideRewindResult Confirm = ShotgunServerSideRewind(HitCharacters, TraceStart, HitLocations, HitTime);		
	//UE_LOG(LogTemp, Display, TEXT("HitCharacter Num : %d"), HitCharacters.Num());

	for (auto& HitCharacter : HitCharacters)
	{
		if (HitCharacter == nullptr || Character == nullptr || InventoryComponent->GetEquippedWeapon() == nullptr) continue;
		if (HitCharacter->IsElimmed()) continue;

		float TotalDamage = 0.f;

		if (Confirm.HeadShots.Contains(HitCharacter))
		{

			float HeadShotDamage = Confirm.HeadShots[HitCharacter] * InventoryComponent->GetEquippedWeapon()->GetHeadShotDamage();
			TotalDamage += HeadShotDamage;
		}
		if (Confirm.BodyShots.Contains(HitCharacter))
		{

			float BodyShotDamage = Confirm.BodyShots[HitCharacter] * InventoryComponent->GetEquippedWeapon()->GetDamage();
			//UE_LOG(LogTemp, Display, TEXT("BodyShotDamage : %f"), BodyShotDamage);
			TotalDamage += BodyShotDamage;

		}

		//UE_LOG(LogTemp, Display, TEXT("TotalDamage : %f, %d"), TotalDamage, Confirm.BodyShots.Num());

		UGameplayStatics::ApplyDamage(HitCharacter, TotalDamage, Character->GetController(), InventoryComponent->GetEquippedWeapon(), UDamageType::StaticClass());
	}
}

void ULagCompensationComponent::HitCapsuleConstruction(USkeletalMeshComponent* Mesh)
{
	if (Mesh == nullptr) return;
	bool IsInitialized = false;
	if (HitCapsuleInfo.Num() != 0) IsInitialized = true;

	for (const auto& x : Mesh->GetPhysicsAsset()->SkeletalBodySetups)
	{
		FName BName = x->BoneName;
		//if (!BName.IsEqual(TEXT("pelvis"))) continue;

		FTransform BoneWorldTransform = Mesh->GetBoneTransform(Mesh->GetBoneIndex(BName));

		for (const auto& y : x->AggGeom.SphylElems)
		{
			FTransform LocalTransform = y.GetTransform();
			FTransform WorldTransform = LocalTransform * BoneWorldTransform;

			FPhysAssetInformation AssetInfo;
			AssetInfo.BoneName = BName;
			AssetInfo.HalfHeight = y.Length / 2 + y.Radius;
			AssetInfo.Radius = y.Radius;
			AssetInfo.BoneWorldTransform = WorldTransform;

			//PhysAssetInfo.Add(AssetInfo);
			HitCapsuleInfo.Emplace(BName, AssetInfo);

			if (!IsInitialized) SetupHitCapsule(AssetInfo, Mesh);
		}
	}


}

void ULagCompensationComponent::SetupHitCapsule(FPhysAssetInformation PhysicsAssetInfo, USkeletalMeshComponent* Mesh)
{
	if (Character == nullptr) return;
	//UE_LOG(LogTemp, Display, TEXT("Initializing"));
	UCapsuleComponent* HitCapsule;
	HitCapsule = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), PhysicsAssetInfo.BoneName, RF_Transient);
	//HitCapsule = CreateDefaultSubobject<UCapsuleComponent>(PhysicsAssetInfo.BoneName);

	HitCapsule->SetupAttachment(Mesh, PhysicsAssetInfo.BoneName);
	HitCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitCapsule->SetCollisionObjectType(ECC_HitCapsule);
	HitCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HitCapsule->SetCollisionResponseToChannel(ECC_HitCapsule, ECollisionResponse::ECR_Block);



	HitCapsule->SetWorldTransform(PhysicsAssetInfo.BoneWorldTransform);
	HitCapsule->SetCapsuleHalfHeight(PhysicsAssetInfo.HalfHeight);
	HitCapsule->SetCapsuleRadius(PhysicsAssetInfo.Radius);

	HitCapsule->RegisterComponent();
	//HitCapsuleBones.Add(HitCapsule);
	Character->HitCollisionCapsules.Add(PhysicsAssetInfo.BoneName, HitCapsule);

}

void ULagCompensationComponent::SaveFramePackage(FFramePackage& Package)
{
	// 
	Character = Character == nullptr ? Cast<ACharacterBase>(GetOwner()) : Character;
	if (Character)
	{
		HitCapsuleConstruction(Character->GetMesh());

		Package.Time = GetWorld()->GetTimeSeconds();

		Package.HitCapsuleInfo = HitCapsuleInfo;
		Package.Character = Character;
	}
}

void ULagCompensationComponent::SaveFramePackage()
{
	if (Character == nullptr || !Character->HasAuthority()) return; // We are going to doing all of these only on server.

	if (FrameHistory.Num() <= 1)
	{
		FFramePackage ThisFrame;
		SaveFramePackage(ThisFrame);
		FrameHistory.AddHead(ThisFrame);
	}
	else
	{
		// Subtract oldest frame time and newest frame time
		float HistoryLength = FrameHistory.GetHead()->GetValue().Time - FrameHistory.GetTail()->GetValue().Time;
		while (HistoryLength > MaxRecordTime)
		{
			FrameHistory.RemoveNode(FrameHistory.GetTail());
			HistoryLength = FrameHistory.GetHead()->GetValue().Time - FrameHistory.GetTail()->GetValue().Time;
		}

		FFramePackage ThisFrame;
		SaveFramePackage(ThisFrame);
		FrameHistory.AddHead(ThisFrame);

		//ShowFramePackage(ThisFrame, FColor::Magenta);
	}

	//UE_LOG(LogTemp, Display, TEXT("FrameHistory num : %d"), FrameHistory.Num());
}

void ULagCompensationComponent::Draw(bool bDraw)
{
	if (bDraw && Character)
	{
		//for (size_t i = 0; i < HitCapsuleBones.Num(); i++)
		//{
		//	DrawDebugCapsule(GetWorld(), HitCapsuleBones[i]->GetComponentLocation(), HitCapsuleBones[i]->GetScaledCapsuleHalfHeight(), HitCapsuleBones[i]->GetScaledCapsuleRadius(), HitCapsuleBones[i]->GetComponentRotation().Quaternion(), FColor::Orange);

		//}

		for (const auto& i : Character->HitCollisionCapsules)
		{
			DrawDebugCapsule(GetWorld(), i.Value->GetComponentLocation(), i.Value->GetScaledCapsuleHalfHeight(), i.Value->GetScaledCapsuleRadius(), i.Value->GetComponentRotation().Quaternion(), FColor::Orange);
		}
	}
}

void ULagCompensationComponent::ShowFramePackage(const FFramePackage& Package, const FColor& Color)
{
	//UE_LOG(LogTemp, Display, TEXT("ShowFramePackage"));
	for (const auto& CapsuleInfo : Package.HitCapsuleInfo)
	{
		DrawDebugCapsule(GetWorld(), CapsuleInfo.Value.BoneWorldTransform.GetLocation(), CapsuleInfo.Value.HalfHeight, CapsuleInfo.Value.Radius, CapsuleInfo.Value.BoneWorldTransform.GetRotation(), Color, false, 2.f);
	}
}

FServerSideRewindResult ULagCompensationComponent::ServerSideRewind(ACharacterBase* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime)
{
	//UE_LOG(LogTemp, Display, TEXT("ServerSideRewind"));
	FFramePackage FrameToCheck = GetFrameToCheck(HitCharacter, HitTime);

	return ConfirmHit(FrameToCheck, HitCharacter, TraceStart, HitLocation);
}

void ULagCompensationComponent::ServerSideRewind_Iter(ACharacterBase* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime)
{
	bool bReturn = HitCharacter == nullptr || HitCharacter->GetLagCompensationComponent() == nullptr || HitCharacter->GetLagCompensationComponent()->FrameHistory.GetHead() == nullptr || HitCharacter->GetLagCompensationComponent()->FrameHistory.GetTail() == nullptr;
	if (bReturn) return;

	FFramePackage FrameToCheck; // Frame package that we check to verify a hit
	bool bShouldInterpolate = true;

	// Frame history of the HitCharacter
	const TDoubleLinkedList<FFramePackage>& History = HitCharacter->GetLagCompensationComponent()->FrameHistory;
	const float OldestHistoryTime = History.GetTail()->GetValue().Time;
	const float NewestHistoryTime = History.GetHead()->GetValue().Time;

	// HitTime locates between edges
	if (OldestHistoryTime > HitTime)
	{
		// too far back - too laggy to do server-side rewind
		// Don't process server-side rewind.
		return;
	}
	else if (OldestHistoryTime == HitTime) // ÀÏ¾î³¯ È®·ü ±Ø¾Ç
	{
		FrameToCheck = History.GetTail()->GetValue();
		bShouldInterpolate = false;
		return;
	}
	else if (NewestHistoryTime <= HitTime)
	{
		FrameToCheck = History.GetHead()->GetValue();
		bShouldInterpolate = false;
		return;
	}

	// HitTime is somewhere in middle
	TDoubleLinkedList<FFramePackage>::TDoubleLinkedListNode* Older = History.GetHead();

	for (TDoubleLinkedList<FFramePackage>::TConstIterator iter = begin(History); iter; ++iter)
	{
		FFramePackage CurrentFrame = *iter;
		if (CurrentFrame.Time <= HitTime)
		{
			Older = iter.GetNode();
		}
	}
	TDoubleLinkedList<FFramePackage>::TDoubleLinkedListNode* Younger = Older->GetPrevNode();

	if (Older->GetValue().Time == HitTime) // highly unlikely, but we found our frame to check (ÀÏ¾î³¯ È®·ü ±Ø¾Ç)
	{
		FrameToCheck = Older->GetValue();
		bShouldInterpolate = false;
		return;
	}

	if (bShouldInterpolate)
	{
		// Interpolate between Younger and Older
	}
}

FServerSideRewindResult ULagCompensationComponent::ProjectileServerSideRewind(ACharacterBase* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
	FFramePackage FrameToCheck = GetFrameToCheck(HitCharacter, HitTime);

	return ProjectileConfirmHit(FrameToCheck, HitCharacter, TraceStart, InitialVelocity, HitTime);
}

FShotgunServerSideRewindResult ULagCompensationComponent::ShotgunServerSideRewind(const TArray<ACharacterBase*>& HitCharacters, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime)
{
	TArray<FFramePackage> FramesToCheck;
	//UE_LOG(LogTemp, Display, TEXT("HitCharacter Num : %d"), HitCharacters.Num());
	for (ACharacterBase* HitCharacter : HitCharacters)
	{
		FramesToCheck.Add(GetFrameToCheck(HitCharacter, HitTime));
	}

	return ShotgunConfirmHit(FramesToCheck, TraceStart, HitLocations);
}

FFramePackage ULagCompensationComponent::GetFrameToCheck(ACharacterBase* HitCharacter, float HitTime)
{
	//UE_LOG(LogTemp, Display, TEXT("HitCharacter : %x"), HitCharacter);
	bool bReturn = HitCharacter == nullptr || HitCharacter->GetLagCompensationComponent() == nullptr || HitCharacter->GetLagCompensationComponent()->FrameHistory.GetHead() == nullptr || HitCharacter->GetLagCompensationComponent()->FrameHistory.GetTail() == nullptr;
	if (bReturn) return FFramePackage();

	FFramePackage FrameToCheck; // Frame package that we check to verify a hit

	bool bShouldInterpolate = true;

	// Frame history of the HitCharacter
	const TDoubleLinkedList<FFramePackage>& History = HitCharacter->GetLagCompensationComponent()->FrameHistory;
	const float OldestHistoryTime = History.GetTail()->GetValue().Time;
	const float NewestHistoryTime = History.GetHead()->GetValue().Time;

	//UE_LOG(LogTemp, Display, TEXT("OldestHistoryTime : %f"), HitTime);
	//UE_LOG(LogTemp, Display, TEXT("NewestHistoryTime : %f"), HitTime);
	//UE_LOG(LogTemp, Display, TEXT("HitTime : %f"), HitTime);

	// HitTime locates between edges
	if (OldestHistoryTime > HitTime)
	{
		// too far back - too laggy to do server-side rewind
		// Don't process server-side rewind.
		return FFramePackage();
	}
	else if (OldestHistoryTime == HitTime) // ÀÏ¾î³¯ È®·ü ±Ø¾Ç
	{
		FrameToCheck = History.GetTail()->GetValue();
		bShouldInterpolate = false;
		return FrameToCheck;
	}
	else if (NewestHistoryTime <= HitTime)
	{
		FrameToCheck = History.GetHead()->GetValue();
		bShouldInterpolate = false;
		return FrameToCheck;
	}

	// HitTime is somewhere in middle
	TDoubleLinkedList<FFramePackage>::TDoubleLinkedListNode* Younger = History.GetHead();
	TDoubleLinkedList<FFramePackage>::TDoubleLinkedListNode* Older = Younger;

	while (Older->GetValue().Time > HitTime) // is Older still younger than HitTime?
	{
		// March back until => OlderTime < HitTime < YoungerTime
		if (Older->GetNextNode() == nullptr) break;
		Older = Older->GetNextNode(); // if left is tail and right is head, NextNode is going to the left.
		if (Older->GetValue().Time > HitTime)
		{
			Younger = Older;
		}
	}

	if (Older->GetValue().Time == HitTime) // highly unlikely, but we found our frame to check (ÀÏ¾î³¯ È®·ü ±Ø¾Ç)
	{
		FrameToCheck = Older->GetValue();
		bShouldInterpolate = false;
		return FFramePackage();
	}

	if (bShouldInterpolate)
	{
		// Interpolate between Younger and Older
		FrameToCheck = InterpBetweenFrames(Older->GetValue(), Younger->GetValue(), HitTime);
	}


	//UE_LOG(LogTemp, Display, TEXT("FrameToCheck.Character : %x"), FrameToCheck.Character);
	FrameToCheck.Character = HitCharacter;

	return FrameToCheck;
}

FFramePackage ULagCompensationComponent::InterpBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime)
{
	const float Distance = YoungerFrame.Time - OlderFrame.Time;
	const float InterpFraction = FMath::Clamp((HitTime - OlderFrame.Time) / Distance, 0.f, 1.f);

	FFramePackage InterpFramePackage;
	InterpFramePackage.Time = HitTime;

	for (auto& YoungerPair : YoungerFrame.HitCapsuleInfo)
	{
		const FName& CapsuleInfoName = YoungerPair.Key;

		const FPhysAssetInformation& OlderCapsule = OlderFrame.HitCapsuleInfo[CapsuleInfoName];
		const FPhysAssetInformation& YoungerCapsule = YoungerFrame.HitCapsuleInfo[CapsuleInfoName];

		FPhysAssetInformation InterpCapsuleInfo;

		FVector Location(FMath::VInterpTo(OlderCapsule.BoneWorldTransform.GetLocation(), YoungerCapsule.BoneWorldTransform.GetLocation(), 1.f, InterpFraction));
		InterpCapsuleInfo.BoneWorldTransform.SetLocation(Location);
		FRotator Rotation(FMath::RInterpTo(OlderCapsule.BoneWorldTransform.GetRotation().Rotator(), YoungerCapsule.BoneWorldTransform.GetRotation().Rotator(), 1.f, InterpFraction).Quaternion());
		InterpCapsuleInfo.BoneWorldTransform.SetRotation(Rotation.Quaternion());

		//InterpCapsuleInfo.BoneWorldTransform.SetLocation(FMath::Lerp(OlderCapsule.BoneWorldTransform.GetLocation(), YoungerCapsule.BoneWorldTransform.GetLocation(), InterpFraction));

		InterpCapsuleInfo.HalfHeight = YoungerCapsule.HalfHeight;
		InterpCapsuleInfo.Radius = YoungerCapsule.Radius;

		InterpFramePackage.HitCapsuleInfo.Add(CapsuleInfoName, InterpCapsuleInfo);
	}

	return InterpFramePackage;
}

FServerSideRewindResult ULagCompensationComponent::ConfirmHit(const FFramePackage& Package, ACharacterBase* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation)
{
	if (HitCharacter == nullptr) return FServerSideRewindResult();
	//UE_LOG(LogTemp, Display, TEXT("HitCharacter : %s"), *HitCharacter->GetName());
	FFramePackage CurrentFrame;
	CacheCapsulePositions(HitCharacter, CurrentFrame);
	MoveCapsules(HitCharacter, Package);
	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::NoCollision);

	// Enable collision for the head first

	UCapsuleComponent* HeadCapsule = HitCharacter->HitCollisionCapsules[TEXT("head")];
	//if (HeadCapsule->GetOwner()) UE_LOG(LogTemp, Error, TEXT("Owner : %s"), *HeadCapsule->GetOwner()->GetName());
	HeadCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HeadCapsule->SetCollisionResponseToChannel(ECC_HitCapsule, ECollisionResponse::ECR_Block);

	FHitResult ConfirmHitResult;
	const FVector TraceEnd = TraceStart + (HitLocation - TraceStart) * 1.25f;
	UWorld* World = GetWorld();
	if (World)
	{
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, true);

		World->LineTraceSingleByChannel(ConfirmHitResult, TraceStart, TraceEnd, ECC_HitCapsule);
		if (ConfirmHitResult.bBlockingHit) // we hit the head, return early
		{
			//UE_LOG(LogTemp, Display, TEXT("headshot : %s"), *ConfirmHitResult.GetActor()->GetName());
			//UE_LOG(LogTemp, Warning, TEXT("headshot : %s"), *HitCharacter->GetName());
			if (ConfirmHitResult.GetComponent())
			{
				UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(ConfirmHitResult.GetComponent());
				if (Capsule)
				{
					DrawDebugCapsule(GetWorld(), Capsule->GetComponentLocation(), Capsule->GetScaledCapsuleHalfHeight(), Capsule->GetScaledCapsuleRadius(), Capsule->GetComponentRotation().Quaternion(), FColor::Red, false, 8.f);
				}
			}

			ResetHitCapsules(HitCharacter, CurrentFrame);
			EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryOnly);
			return FServerSideRewindResult{ true, true };
		}
		else // didn't hit head, check the rest of the boxes
		{
			for (auto& HitCapsulePair : HitCharacter->HitCollisionCapsules)
			{
				if (HitCapsulePair.Value != nullptr)
				{
					HitCapsulePair.Value->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
					HitCapsulePair.Value->SetCollisionResponseToChannel(ECC_HitCapsule, ECollisionResponse::ECR_Block);
				}
			}

			World->LineTraceSingleByChannel(ConfirmHitResult, TraceStart, TraceEnd, ECC_HitCapsule);

			if (ConfirmHitResult.bBlockingHit)
			{
				//UE_LOG(LogTemp, Display, TEXT("bodyshot : %s"), *ConfirmHitResult.GetActor()->GetName());
				if (ConfirmHitResult.GetComponent())
				{
					UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(ConfirmHitResult.GetComponent());
					if (Capsule)
					{
						DrawDebugCapsule(GetWorld(), Capsule->GetComponentLocation(), Capsule->GetScaledCapsuleHalfHeight(), Capsule->GetScaledCapsuleRadius(), Capsule->GetComponentRotation().Quaternion(), FColor::Blue, false, 8.f);
					}
				}

				ResetHitCapsules(HitCharacter, CurrentFrame);
				EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryOnly);
				return FServerSideRewindResult{ true, false };
			}
		}
	}

	ResetHitCapsules(HitCharacter, CurrentFrame);
	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryOnly);
	return FServerSideRewindResult{ false, false };
}

FServerSideRewindResult ULagCompensationComponent::ProjectileConfirmHit(const FFramePackage& Package, ACharacterBase* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
	if (HitCharacter == nullptr) return FServerSideRewindResult();
	//UE_LOG(LogTemp, Display, TEXT("HitCharacter : %s"), *HitCharacter->GetName());
	FFramePackage CurrentFrame;
	CacheCapsulePositions(HitCharacter, CurrentFrame);
	MoveCapsules(HitCharacter, Package);
	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::NoCollision);

	// Enable collision for the head first

	UCapsuleComponent* HeadCapsule = HitCharacter->HitCollisionCapsules[TEXT("head")];
	//if (HeadCapsule->GetOwner()) UE_LOG(LogTemp, Error, TEXT("Owner : %s"), *HeadCapsule->GetOwner()->GetName());

	HeadCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	HeadCapsule->SetCollisionResponseToChannel(ECC_HitCapsule, ECollisionResponse::ECR_Block);


	//UE_LOG(LogTemp, Display, TEXT("ProjectileConfirmHit Getowner : %s"), *GetOwner()->GetName());

	FPredictProjectilePathParams PathParams;
	PathParams.bTraceWithCollision = true;
	PathParams.MaxSimTime = MaxRecordTime;
	PathParams.LaunchVelocity = InitialVelocity;
	PathParams.StartLocation = TraceStart;
	PathParams.SimFrequency = 15.f;
	PathParams.ProjectileRadius = 5.f;
	PathParams.TraceChannel = ECC_HitCapsule;
	PathParams.ActorsToIgnore.Add(GetOwner());

	PathParams.DrawDebugType = EDrawDebugTrace::None;
	PathParams.DrawDebugTime = 5.f;


	FPredictProjectilePathResult PathResult;
	UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);


	if (PathResult.HitResult.bBlockingHit) // we hit the head, return early
	{
		if (PathResult.HitResult.GetComponent())
		{
			UE_LOG(LogTemp, Display, TEXT("HitResult : %s"), *PathResult.HitResult.GetActor()->GetName());

			UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(PathResult.HitResult.GetComponent());
			if (Capsule)
			{
				DrawDebugCapsule(GetWorld(), Capsule->GetComponentLocation(), Capsule->GetScaledCapsuleHalfHeight(), Capsule->GetScaledCapsuleRadius(), Capsule->GetComponentRotation().Quaternion(), FColor::Red, false, 8.f);
			}
		}

		ResetHitCapsules(HitCharacter, CurrentFrame);
		EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryOnly);
		return FServerSideRewindResult{ true, true };
	}
	else // we didn't hit the head; check the rest of the boxes
	{
		for (auto& HitCapsulePair : HitCharacter->HitCollisionCapsules)
		{
			if (HitCapsulePair.Value != nullptr)
			{
				HitCapsulePair.Value->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				HitCapsulePair.Value->SetCollisionResponseToChannel(ECC_HitCapsule, ECollisionResponse::ECR_Block);
			}
		}

		//PathParams.TraceChannel = ECC_HitCapsule;
		UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);

		//UE_LOG(LogTemp, Display, TEXT("ProjectileConfirmHit : %s"), *PathResult.HitResult.GetActor()->GetName());

		if (PathResult.HitResult.bBlockingHit) // we hit the head, return early
		{
			if (PathResult.HitResult.GetComponent())
			{
				UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(PathResult.HitResult.GetComponent());
				if (Capsule)
				{
					DrawDebugCapsule(GetWorld(), Capsule->GetComponentLocation(), Capsule->GetScaledCapsuleHalfHeight(), Capsule->GetScaledCapsuleRadius(), Capsule->GetComponentRotation().Quaternion(), FColor::Red, false, 8.f);
				}
			}

			ResetHitCapsules(HitCharacter, CurrentFrame);
			EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryOnly);
			return FServerSideRewindResult{ true, false };
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PathResult.HitResult.bBlockingHit is 0"));
		}
	}

	ResetHitCapsules(HitCharacter, CurrentFrame);
	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryOnly);
	return FServerSideRewindResult{ false, false };
}

FShotgunServerSideRewindResult ULagCompensationComponent::ShotgunConfirmHit(const TArray<FFramePackage>& FramePackages, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations)
{
	//UE_LOG(LogTemp, Display, TEXT("ShotgunConfirmHit"));

	for (auto& Frame : FramePackages)
	{
		if (Frame.Character == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("FramePackage's Character is NULL"));
			return FShotgunServerSideRewindResult();
		}
	}


	FShotgunServerSideRewindResult ShotgunResult;
	TArray<FFramePackage> CurrentFrames;
	for (auto& Frame : FramePackages)
	{
		FFramePackage CurrentFrame;
		CurrentFrame.Character = Frame.Character;
		CacheCapsulePositions(Frame.Character, CurrentFrame);
		MoveCapsules(Frame.Character, Frame);
		EnableCharacterMeshCollision(Frame.Character, ECollisionEnabled::NoCollision);
		CurrentFrames.Add(CurrentFrame);
	}

	for (auto& Frame : FramePackages)
	{
		// Enable collision for the head first
		UCapsuleComponent* HeadCapsule = Frame.Character->HitCollisionCapsules[TEXT("head")];
		HeadCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		HeadCapsule->SetCollisionResponseToChannel(ECC_HitCapsule, ECollisionResponse::ECR_Block);
	}

	UWorld* World = GetWorld();
	// check for head shots
	for (auto& HitLocation : HitLocations)
	{
		FHitResult ConfirmHitResult;
		const FVector TraceEnd = TraceStart + (HitLocation - TraceStart) * 1.25f;
		if (World)
		{
			World->LineTraceSingleByChannel(ConfirmHitResult, TraceStart, TraceEnd, ECC_HitCapsule);
		}

		ACharacterBase* BlasterCharacter = Cast<ACharacterBase>(ConfirmHitResult.GetActor());
		if (BlasterCharacter)
		{

			if (ConfirmHitResult.GetComponent())
			{
				UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(ConfirmHitResult.GetComponent());
				if (Capsule)
				{
					DrawDebugCapsule(GetWorld(), Capsule->GetComponentLocation(), Capsule->GetScaledCapsuleHalfHeight(), Capsule->GetScaledCapsuleRadius(), Capsule->GetComponentRotation().Quaternion(), FColor::Red, false, 8.f);
				}
			}

			if (ShotgunResult.HeadShots.Contains(BlasterCharacter))
			{
				ShotgunResult.HeadShots[BlasterCharacter]++;
			}
			else
			{
				//UE_LOG(LogTemp, Display, TEXT("HeadShots"));
				ShotgunResult.HeadShots.Emplace(BlasterCharacter, 1);
			}
		}
	}

	// enable collision for all capsules, then disable for head capsule
	for (auto& Frame : FramePackages)
	{
		for (auto& HitCapsulePair : Frame.Character->HitCollisionCapsules)
		{
			if (HitCapsulePair.Value != nullptr)
			{
				HitCapsulePair.Value->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				HitCapsulePair.Value->SetCollisionResponseToChannel(ECC_HitCapsule, ECollisionResponse::ECR_Block);
			}
		}
		UCapsuleComponent* HeadCapsule = Frame.Character->HitCollisionCapsules[TEXT("head")];
		HeadCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// check for body shots
	//UE_LOG(LogTemp, Display, TEXT("HitLocations num : %d"), HitLocations.Num());
	for (auto& HitLocation : HitLocations)
	{
		FHitResult ConfirmHitResult;
		const FVector TraceEnd = TraceStart + (HitLocation - TraceStart) * 1.25f;
		if (World)
		{
			World->LineTraceSingleByChannel(ConfirmHitResult, TraceStart, TraceEnd, ECC_HitCapsule);
		}

		ACharacterBase* BlasterCharacter = Cast<ACharacterBase>(ConfirmHitResult.GetActor());
		if (BlasterCharacter)
		{

			if (ConfirmHitResult.GetComponent())
			{
				//UE_LOG(LogTemp, Display, TEXT("BlasterCharacter : %s"), *BlasterCharacter->GetName());
				UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(ConfirmHitResult.GetComponent());
				if (Capsule)
				{
					DrawDebugCapsule(GetWorld(), Capsule->GetComponentLocation(), Capsule->GetScaledCapsuleHalfHeight(), Capsule->GetScaledCapsuleRadius(), Capsule->GetComponentRotation().Quaternion(), FColor::Blue, false, 8.f);
				}
			}

			if (ShotgunResult.BodyShots.Contains(BlasterCharacter))
			{
				ShotgunResult.BodyShots[BlasterCharacter]++;
			}
			else
			{
				ShotgunResult.BodyShots.Emplace(BlasterCharacter, 1);
			}
		}
	}

	for (auto& Frame : CurrentFrames)
	{
		ResetHitCapsules(Frame.Character, Frame);
		EnableCharacterMeshCollision(Frame.Character, ECollisionEnabled::QueryOnly);
	}

	return ShotgunResult;
}

void ULagCompensationComponent::CacheCapsulePositions(ACharacterBase* HitCharacter, FFramePackage& OutFramePackage)
{
	if (HitCharacter == nullptr) return;

	for (const auto& HitCapsulePair : HitCharacter->HitCollisionCapsules)
	{
		if (HitCapsulePair.Value != nullptr)
		{
			FPhysAssetInformation CapsuleInfo;
			CapsuleInfo.BoneName = HitCapsulePair.Key;
			CapsuleInfo.BoneWorldTransform = HitCapsulePair.Value->GetComponentTransform();
			CapsuleInfo.HalfHeight = HitCapsulePair.Value->GetScaledCapsuleHalfHeight();
			CapsuleInfo.Radius = HitCapsulePair.Value->GetScaledCapsuleRadius();
			OutFramePackage.HitCapsuleInfo.Add(HitCapsulePair.Key, CapsuleInfo);
		}
	}
}

void ULagCompensationComponent::MoveCapsules(ACharacterBase* HitCharacter, const FFramePackage& Package)
{
	if (HitCharacter == nullptr) return;

	for (const auto& HitCapsulePair : HitCharacter->HitCollisionCapsules)
	{
		if (HitCapsulePair.Value != nullptr && Package.HitCapsuleInfo.Find(HitCapsulePair.Key))
		{
			HitCapsulePair.Value->SetWorldTransform(Package.HitCapsuleInfo[HitCapsulePair.Key].BoneWorldTransform);
			HitCapsulePair.Value->SetCapsuleHalfHeight(Package.HitCapsuleInfo[HitCapsulePair.Key].HalfHeight);
			HitCapsulePair.Value->SetCapsuleRadius(Package.HitCapsuleInfo[HitCapsulePair.Key].Radius);
		}
	}
}

void ULagCompensationComponent::ResetHitCapsules(ACharacterBase* HitCharacter, const FFramePackage& Package)
{
	if (HitCharacter == nullptr) return;
	for (const auto& HitCapsulePair : HitCharacter->HitCollisionCapsules)
	{
		if (HitCapsulePair.Value != nullptr && Package.HitCapsuleInfo.Find(HitCapsulePair.Key))
		{
			HitCapsulePair.Value->SetWorldTransform(Package.HitCapsuleInfo[HitCapsulePair.Key].BoneWorldTransform);
			HitCapsulePair.Value->SetCapsuleHalfHeight(Package.HitCapsuleInfo[HitCapsulePair.Key].HalfHeight);
			HitCapsulePair.Value->SetCapsuleRadius(Package.HitCapsuleInfo[HitCapsulePair.Key].Radius);
			HitCapsulePair.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ULagCompensationComponent::EnableCharacterMeshCollision(ACharacterBase* HitCharacter, ECollisionEnabled::Type CollisionEnable)
{
	if (HitCharacter && HitCharacter->GetMesh())
	{
		HitCharacter->GetMesh()->SetCollisionEnabled(CollisionEnable);
	}
}

