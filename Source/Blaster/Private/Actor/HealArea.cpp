// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/HealArea.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Components/BuffComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/SkillComponent.h"

// Sets default values
AHealArea::AHealArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	ShieldEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShieldEffect"));

	SetRootComponent(SphereCollision);
	ShieldEffect->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AHealArea::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::SphereBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::SphereEndOverlap);

	SetLifeSpan(LifeSpan);
}

// Called every frame
void AHealArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealArea::Destroyed()
{
	Super::Destroyed();

	if (GetOwner())
	{
		USkillComponent* SkillComponent = GetOwner()->GetComponentByClass<USkillComponent>();
		if (SkillComponent)
		{
			SkillComponent->OnSkillStarted.Broadcast(TEXT("Skill"), 0, SkillComponent->CoolTimeMap.Find(UEnum::GetDisplayValueAsText(ESkillAssistant::ESA_HealArea).ToString())->CoolTime);
		}
	}
}

void AHealArea::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UBuffComponent* BuffComponent = OtherActor->GetComponentByClass<UBuffComponent>();
	UAttributeComponent* AttributeComponent = OtherActor->GetComponentByClass<UAttributeComponent>();

	if (AttributeComponent && BuffComponent)
	{
		BuffComponent->Heal(AttributeComponent->GetMaxHp(), HealingRate);
	}
}

void AHealArea::SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UBuffComponent* BuffComponent = OtherActor->GetComponentByClass<UBuffComponent>();
	if (BuffComponent)
	{
		BuffComponent->SetIsHealing(false);
	}
}

