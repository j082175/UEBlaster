// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureTheFlag/FlagZone.h"
#include "Components/SphereComponent.h"
#include "Item/Pickable/Weapon/Flag.h"
#include "GameMode/CaptureTheFlagGameMode.h"
#include "Characters/BlasterCharacter.h"
#include "Components/InventoryComponent.h"
#include "Blaster/Blaster.h"

// Sets default values
AFlagZone::AFlagZone()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 1.f;

	ZoneSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ZoneSphere"));
	SetRootComponent(ZoneSphere);
	//SetReplicateMovement(true);
}

void AFlagZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGameMode* GM = GetWorld()->GetAuthGameMode<ACaptureTheFlagGameMode>();

	AB_LOG(LogABDisplay, Warning, TEXT("hasautority : %d"), HasAuthority());
}

void AFlagZone::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	if (HasAuthority())
	ZoneSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
}

void AFlagZone::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Display, TEXT("OnSphereOverlap, Authority : %d"), HasAuthority());

	//UE_LOG(LogTemp, Display, TEXT("%s : OnSphereOverlap"), *UEnum::GetDisplayValueAsText(OtherActor->GetLocalRole()).ToString());

	if (UInventoryComponent* IC = OtherActor->GetComponentByClass<UInventoryComponent>())
	{
		if (AFlag* OverlappingFlag = IC->GetFlag())
		{
			if (OverlappingFlag && OverlappingFlag->IGetTeam() != Team)
			{
				ACaptureTheFlagGameMode* GameMode = GetWorld()->GetAuthGameMode<ACaptureTheFlagGameMode>();
				if (GameMode)
				{

					GameMode->FlagCaptured(OverlappingFlag, this);
					//ABlasterCharacter* FlagBearer = Cast<ABlasterCharacter>(OverlappingFlag->GetOwner());
					//if (FlagBearer)
					//{
					//	FlagBearer->SetHoldingTheFlag(false);
					//}

					//OverlappingFlag->ResetFlag();

					OverlappingFlag->Drop();
					OverlappingFlag->GetPickupMesh()->SetSimulatePhysics(false);
					OverlappingFlag->MulticastResetFlag();

				}
				else
				{
					UE_LOG(LogTemp, Display, TEXT("ACaptureTheFlagGameMode is null"));
				}
			}
		}

		
	}



}

