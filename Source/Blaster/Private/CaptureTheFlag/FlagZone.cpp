// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureTheFlag/FlagZone.h"
#include "Components/SphereComponent.h"
#include "Item/Pickable/Weapon/Flag.h"
#include "GameMode/CaptureTheFlagGameMode.h"
#include "Characters/BlasterCharacter.h"

// Sets default values
AFlagZone::AFlagZone()
{
	PrimaryActorTick.bCanEverTick = false;

	ZoneSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ZoneSphere"));
	SetRootComponent(ZoneSphere);
	//SetReplicateMovement(true);
}

void AFlagZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("FlagZone Tick"));
}

void AFlagZone::BeginPlay()
{
	Super::BeginPlay();

	//if (HasAuthority())
	ZoneSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
}

void AFlagZone::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//UE_LOG(LogTemp, Display, TEXT("%s : OnSphereOverlap"), *UEnum::GetDisplayValueAsText(OtherActor->GetLocalRole()).ToString());

	AFlag* OverlappingFlag = Cast<AFlag>(OtherActor);
	if (OverlappingFlag && OverlappingFlag->GetTeam() != Team)
	{


		ACaptureTheFlagGameMode* GameMode = GetWorld()->GetAuthGameMode<ACaptureTheFlagGameMode>();
		if (GameMode)
		{

			GameMode->FlagCaptured(OverlappingFlag, this);
			ABlasterCharacter* FlagBearer = Cast<ABlasterCharacter>(OverlappingFlag->GetOwner());
			if (FlagBearer)
			{
				FlagBearer->SetHoldingTheFlag(false);
			}

			//OverlappingFlag->ResetFlag();

			OverlappingFlag->MulticastResetFlag();

		}
	}
}

