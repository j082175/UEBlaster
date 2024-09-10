// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DefenceActor.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Components/BuffComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/SkillComponent.h"

// Sets default values
ADefenceActor::ADefenceActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	ShieldEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShieldEffect"));

	SetRootComponent(SphereCollision);
	ShieldEffect->SetupAttachment(GetRootComponent());

	SphereCollision->SetSphereRadius(150.f);

}

void ADefenceActor::PostLoad()
{
	Super::PostLoad();
	SetReplicates(true);
}


// Called when the game starts or when spawned
void ADefenceActor::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::SphereBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::SphereEndOverlap);
}

// Called every frame
void ADefenceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADefenceActor::Destroyed()
{
	Super::Destroyed();

	if (GetOwner())
	{
		//USkillComponent* SkillComponent = GetOwner()->GetComponentByClass<USkillComponent>();
		//if (SkillComponent)
		//{
		//	SkillComponent->OnSkillAnimStarted.Broadcast(TEXT("Skill"), Index, SkillComponent->CoolTimeMap.Find(UEnum::GetDisplayValueAsText(ESkillAssistant::ESA_HealArea).ToString())->CoolTime);
		//}
	}
}

void ADefenceActor::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ADefenceActor::SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

