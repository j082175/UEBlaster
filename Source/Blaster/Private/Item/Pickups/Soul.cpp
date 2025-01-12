// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickups/Soul.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkillComponent.h"
#include "Interfaces/OverlapItemInterface.h"
#include "Blaster/Blaster.h"

ASoul::ASoul()
{
	SoulEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SoulEffectComponent"));
	SoulEffect->SetupAttachment(GetRootComponent());
}

void ASoul::OnCapsuleBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	USkillComponent* SkillComponent = OtherActor->GetComponentByClass<USkillComponent>();
	if (SkillComponent && OtherActor->Implements<UOverlapItemInterface>())
	{
		Super::OnCapsuleBeginOverlapFunc(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

		//AB_LOG(LogABDisplay, Log, TEXT(""));
		UGameplayStatics::PlaySoundAtLocation(this, TakeSound, SoulEffect->GetComponentLocation());
		SkillComponent->AddSkillPoint(Soul);
		MulticastSpawnEmitter();

		Destroy();
	}
}

void ASoul::MulticastSpawnEmitter_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SoulEffectEnd, SoulEffect->GetComponentTransform());
}
