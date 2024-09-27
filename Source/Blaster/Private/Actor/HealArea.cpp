// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/HealArea.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Components/BuffComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/SkillComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AHealArea::AHealArea()
{
}

void AHealArea::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(OwnerCharacter->GetMesh(), Rules, TEXT("pelvis"));
}

void AHealArea::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::SphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UBuffComponent* BuffComponent = OtherActor->GetComponentByClass<UBuffComponent>();
	UAttributeComponent* AttributeComponent = OtherActor->GetComponentByClass<UAttributeComponent>();

	if (AttributeComponent && BuffComponent)
	{
		BuffComponent->Heal(AttributeComponent->GetMaxHp(), HealingRate);
	}
}

void AHealArea::SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::SphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	UBuffComponent* BuffComponent = OtherActor->GetComponentByClass<UBuffComponent>();
	if (BuffComponent)
	{
		BuffComponent->SetIsHealing(false);
	}
}
