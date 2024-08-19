// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Casing.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Blaster/Blaster.h"

// Sets default values
ACasing::ACasing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMeshComponent"));
	SetRootComponent(CasingMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("/Script/Engine.StaticMesh'/Game/Assets/MilitaryWeapSilver/Weapons/AssaultRifleA_Ammo.AssaultRifleA_Ammo'"));
	if (SM.Succeeded())
	{
		CasingMesh->SetStaticMesh(SM.Object);
	}

	CasingMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);


	CasingMesh->SetSimulatePhysics(false);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true);
	ShellEjectionImpulse = 10.f;

	CasingMesh->SetCollisionProfileName(PROFILE_IgnoreOnlyPawn);

	CasingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ACasing::BeginPlay()
{
	Super::BeginPlay();

	//CasingMesh->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	//CasingMesh->AddImpulse(UKismetMathLibrary::RandomUnitVectorInConeInDegrees(GetActorForwardVector(), 20.f) * ShellEjectionImpulse);

}

// Called every frame
void ACasing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACasing::SetIsActive(bool InIsActive)
{
	Super::SetIsActive(InIsActive);

	if (InIsActive)
	{
		//UE_LOG(LogTemp, Display, TEXT("Active"));

		CasingMesh->SetCollisionProfileName(PROFILE_IgnoreOnlyPawn);
		CasingMesh->SetSimulatePhysics(true);

		CasingMesh->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnHit);
		CasingMesh->AddImpulse(UKismetMathLibrary::RandomUnitVectorInConeInDegrees(GetActorForwardVector(), 20.f) * ShellEjectionImpulse);

		//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f, FColor::Red, false, 3.f);
	}
	else
	{
		CasingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CasingMesh->SetSimulatePhysics(false);
	}

}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bIsHit)
	{
		if (ShellSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
		}

		//SetLifeSpan(DestroyTime);
		bIsHit = true;
		CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		CasingMesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);

	}

}

