// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ReceiveDamageHUDComponent.h"
#include "HUD/ComponentHUD/DamageScroll.h"
#include "GameFramework/Character.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blaster.h"

UReceiveDamageHUDComponent::UReceiveDamageHUDComponent()
{
	static ConstructorHelpers::FClassFinder<UDamageScroll> DamageScrollRef(TEXT("/Game/A_Blaster/Blueprints/HUD/ComponentHUD/WBP_DamageScroll.WBP_DamageScroll_C"));
	ensure(DamageScrollRef.Class);
	SetWidgetClass(DamageScrollRef.Class);

	SetRelativeLocation(FVector(60.f, -130.f, 100.f));
	SetRelativeScale3D(FVector(0.2f));
	SetWidgetSpace(EWidgetSpace::Screen);

	SetGenerateOverlapEvents(false);
}

void UReceiveDamageHUDComponent::BeginPlay()
{
	Super::BeginPlay();

	DamageScroll = Cast<UDamageScroll>(GetWidget());


}

void UReceiveDamageHUDComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Display, TEXT("UReceiveDamageHUDComponent Tick"));

	SetComponentTickInterval(100.f);
}

void UReceiveDamageHUDComponent::SetDamageInfo(int32 InDamage, class AController* InController, const FLinearColor& InColor)
{
	if (DamageScroll.IsValid())
	{
		USceneComponent* Axis = GetAttachParent();
		FRotator FinalRot = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), InController->GetPawn()->GetActorLocation());
		//Axis->SetRelativeRotation(FinalRot);
		Axis->SetWorldRotation(FinalRot);

		DamageScroll->AddDamageText(InDamage, InColor);
	}
}