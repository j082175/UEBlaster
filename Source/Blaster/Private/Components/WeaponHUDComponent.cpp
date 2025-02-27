// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponHUDComponent.h"
#include "HUD/OverlayModules/WeaponStatus.h"
#include "Blaster/Blaster.h"
#include "GameFramework/Character.h"

UWeaponHUDComponent::UWeaponHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;

	static ConstructorHelpers::FClassFinder<UWeaponStatus> WeaponStatusRef(TEXT("/Game/A_Blaster/Blueprints/HUD/OverlayModules/WBP_WeaponStatus.WBP_WeaponStatus_C"));
	if (WeaponStatusRef.Succeeded()) SetWidgetClass(WeaponStatusRef.Class);
	else ensure(false);

	SetRelativeScale3D(FVector(0.1f));
	SetRelativeRotation(FRotator(-90.f, 0.f, 90.f));
	SetRelativeLocation(FVector(20.f, 0.f, 0.f));
	bDrawAtDesiredSize = true;

	SetGenerateOverlapEvents(false);
}

void UWeaponHUDComponent::PostLoad()
{
	Super::PostLoad();

	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UWeaponHUDComponent::InitWidget()
{
	Super::InitWidget();

	WeaponStatus = Cast<UWeaponStatus>(GetWidget());
}

void UWeaponHUDComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//AB_SUBLOG(LogABDisplay, Warning, TEXT(""));
}
