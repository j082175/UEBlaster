// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ShieldBarrier.h"
#include "Components/AttributeComponent.h"

// Sets default values
AShieldBarrier::AShieldBarrier()
{
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

			GetWorldTimerManager().SetTimer(ReplenishTimer, FTimerDelegate::CreateLambda([&]()
				{
					AttributeComponent->SetCurrentShield(AttributeComponent->GetMaxShield());
				}), ReplenishInterval, true, 0.f);
		}


	}
}