// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BuffComponent.h"
#include "Components/AttributeComponent.h"
#include "Characters/BlasterCharacter.h"
#include "Characters/CharacterBase.h"
#include "PlayerController/BlasterPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UBuffComponent::UBuffComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Character = Cast<ABlasterCharacter>(GetOwner());
	if (Character)
	{
		OwnerAttribute = Cast<ABlasterCharacter>(GetOwner())->GetAttributeComponent();
		BlasterPlayerController = Cast<ABlasterPlayerController>(Character->GetController());
	}
}


// Called every frame
void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//UE_LOG(LogTemp, Display, TEXT("%s : BuffComponent Tick"), *UEnum::GetDisplayValueAsText(GetOwner()->GetLocalRole()).ToString());
	HealRampUp(DeltaTime);
	ShieldRampUp(DeltaTime);
}

void UBuffComponent::SetInitialSpeeds(float BaseSpeed, float CrouchSpeed)
{
	InitialBaseSpeed = BaseSpeed;
	InitialCrouchSpeed = CrouchSpeed;
}

void UBuffComponent::SetInitialJumpVelocity(float Velocity)
{
	InitialJumpVelocity = Velocity;
}

void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
	bHealing = true;
	HealingRate = HealAmount / HealingTime;
	AmountToHeal += HealAmount;
}

void UBuffComponent::ReplenishShield(float ShieldAmount, float ReplenishingTime)
{
	//UE_LOG(LogTemp, Display, TEXT("%s : ReplenishShield"), *UEnum::GetDisplayValueAsText(GetOwner()->GetLocalRole()).ToString());
	bReplenishingShield = true;
	ShieldReplenishRate = ShieldAmount / ReplenishingTime;
	AmountToShield += ShieldAmount;

}

void UBuffComponent::BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime)
{
	if (Character == nullptr) return;
	bIsBuffingSpeed = true;

	Character->GetWorldTimerManager().SetTimer(SpeedBuffTimer, this, &ThisClass::ResetSpeeds, BuffTime);
	if (Character->GetCharacterMovement())
	{
		MulticastSpeedBuff(Character->GetCharacterMovement()->MaxWalkSpeed + BuffBaseSpeed, Character->GetCharacterMovement()->MaxWalkSpeedCrouched + BuffCrouchSpeed);
	}


}

void UBuffComponent::BuffJump(float BuffJumpVelocity, float BuffTime)
{
	if (Character == nullptr) return;

	Character->GetWorldTimerManager().SetTimer(JumpBuffTimer, this, &ThisClass::ResetJump, BuffTime);
	if (Character->GetCharacterMovement())
	{
		MulticastJumpBuff(BuffJumpVelocity);
	}
}

void UBuffComponent::HealRampUp(float DeltaTime)
{
	if (!bHealing || Character == nullptr || Character->IsElimmed() || OwnerAttribute == nullptr) return;

	const float HealThisFrame = HealingRate * DeltaTime;

	const float FillAmount = FMath::Clamp(OwnerAttribute->GetCurrentHp() + HealThisFrame, 0.f, OwnerAttribute->GetMaxHp());

	OwnerAttribute->SetCurrentHp(FillAmount);


	if (BlasterPlayerController) BlasterPlayerController->UpdateHUDHealth();
	OwnerAttribute->OnHpChanged.Broadcast(OwnerAttribute->GetCurrentHp() , OwnerAttribute->GetMaxHp());

	AmountToHeal -= HealThisFrame;

	if (AmountToHeal < 1.f || OwnerAttribute->GetCurrentHp() >= OwnerAttribute->GetMaxHp())
	{
		bHealing = false;

		AmountToHeal = 0.f;
	}
}

void UBuffComponent::ShieldRampUp(float DeltaTime)
{
	//if (GetOwner()->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)

	if (!bReplenishingShield || Character == nullptr || Character->IsElimmed()) return;

	//UE_LOG(LogTemp, Display, TEXT("bReplenishingShield : %d"), bReplenishingShield);


	float ReplenishThisFrame = ShieldReplenishRate * DeltaTime;

	float FillAmount = FMath::Clamp(OwnerAttribute->GetCurrentShield() + ReplenishThisFrame, 0.f, OwnerAttribute->GetMaxShield());

	OwnerAttribute->SetCurrentShield(FillAmount);

	if (BlasterPlayerController)
	{
		BlasterPlayerController->UpdateHUDShield();
	}

	//ABlasterCharacter* BC = Cast<ABlasterCharacter>(GetOwner());
	//if (BC)
	//{
	//	BC->UpdateHUDShield();
	//}

	OwnerAttribute->OnShieldChanged.Broadcast(OwnerAttribute->GetCurrentShield() / OwnerAttribute->GetMaxShield());

	//UE_LOG(LogTemp, Display, TEXT("FillAmount : %f"), FillAmount);
	AmountToShield -= ReplenishThisFrame;

	if (AmountToShield < 1.f || OwnerAttribute->GetCurrentShield() >= OwnerAttribute->GetMaxShield())
	{
		bReplenishingShield = false;
		AmountToShield = 0.f;
	}
}

void UBuffComponent::ResetSpeeds()
{
	if (Character == nullptr || Character->GetCharacterMovement() == nullptr) return;
	bIsBuffingSpeed = false;

	MulticastSpeedBuff(InitialBaseSpeed, InitialCrouchSpeed);
}

void UBuffComponent::MulticastJumpBuff_Implementation(float JumpVelocity)
{
	Character->GetCharacterMovement()->JumpZVelocity = JumpVelocity;

}

void UBuffComponent::ResetJump()
{
	MulticastJumpBuff(InitialJumpVelocity);
}

void UBuffComponent::MulticastSpeedBuff_Implementation(float BaseSpeed, float CrouchSpeed)
{
	Character->GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
}
