// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponHUDComponent.h"
#include "HUD/OverlayModules/WeaponStatus.h"

UWeaponHUDComponent::UWeaponHUDComponent()
{
	static ConstructorHelpers::FClassFinder<UWeaponStatus> WeaponStatusRef(TEXT("/Game/A_Blaster/Blueprints/HUD/OverlayModules/WBP_WeaponStatus.WBP_WeaponStatus_C"));
	if (WeaponStatusRef.Succeeded()) SetWidgetClass(WeaponStatusRef.Class);
	else ensure(false);

	SetRelativeScale3D(FVector(0.1f));
	SetRelativeRotation(FRotator(-90.f, 0.f, 90.f));
	SetRelativeLocation(FVector(20.f, 0.f, 0.f));
	bDrawAtDesiredSize = true;
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

	//UE_LOG(LogTemp, Display, TEXT("UWeaponHUDComponent::TickComponent"));
}
