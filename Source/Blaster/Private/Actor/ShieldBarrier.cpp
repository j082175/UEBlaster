// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ShieldBarrier.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/Character.h"
//#include "Characters/BlasterCharacter.h"
#include "Enums/CharacterTypes.h"
#include "Blaster.h"

// Sets default values
AShieldBarrier::AShieldBarrier()
{

}

void AShieldBarrier::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());

	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(CharacterOwner->GetMesh(), Rules, TEXT("pelvis"));
}

void AShieldBarrier::Destroyed()
{
	Super::Destroyed();

	GetWorldTimerManager().ClearTimer(ReplenishTimer);
	ReplenishTimer.Invalidate();
}

void AShieldBarrier::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::SphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor == GetOwner())
	{
		AttributeComponent = GetOwner()->GetComponentByClass<UAttributeComponent>();
		if (AttributeComponent)
		{
			//AttributeComponent->SetCurrentShield(AttributeComponent->GetMaxShield());

			GetWorldTimerManager().SetTimer(ReplenishTimer, this, &ThisClass::SetShieldFunc, ReplenishInterval, true, 0.f);
		}


	}
}

void AShieldBarrier::SetShieldFunc()
{
	AttributeComponent->SetCurrentShield(AttributeComponent->GetMaxShield());
}
