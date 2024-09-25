// Fill out your copyright notice in the Description page of Project Settings.
#include "HUD/HpBarWidgetComponent.h"
#include "HUD/HpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Blaster/DrawDebug.h"

#include "Net/UnrealNetwork.h"

//#include "Interfaces/WidgetBindDelegateInterface.h"

UHpBarWidgetComponent::UHpBarWidgetComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.bStartWithTickEnabled = false;
	//PrimaryComponentTick.TickInterval = 0.1f;
	//SetIsReplicated(true);
	SetGenerateOverlapEvents(false);
}

void UHpBarWidgetComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UHpBarWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UE_LOG(LogTemp, Display, TEXT("HpBarWidgetComponent Tick"));

	//UE_LOG(LogTemp, Display, TEXT("%s : HasLocalNetOwner : %d"), *UEnum::GetDisplayValueAsText(GetOwner()->GetLocalRole()).ToString(), GetOwner()->HasLocalNetOwner());

	//if (bShieldChanged) SetShieldBar(0, ShieldRate, DeltaTime);

	CheckHpBarWidget(DeltaTime);
}

void UHpBarWidgetComponent::InitWidget()
{
	Super::InitWidget();

	HpBarWidget = Cast<UHpBarWidget>(GetUserWidgetObject());
	//NullChecker(HpBarWidget, TEXT("HpBarWidget"), *GetName());
	SetHpBar(1.f, 1.f);
	SetShieldBar(1.f, 1.f);

	
	//IWidgetBindDelegateInterface* WidgetOwner = Cast<IWidgetBindDelegateInterface>(GetOwner());
	//if (WidgetOwner)
	//{
	//	WidgetOwner->IBindWidget(this);
	//}
}

void UHpBarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHpBarWidgetComponent::SetHpBar(float InCurrent, float InMax)
{
	if (HpBarWidget)
	{
		//UE_LOG(LogTemp, Warning, TEXT("SetHpBar"));

		HpBarWidget->GetHpBar()->SetPercent(InCurrent / InMax);
	}
}

void UHpBarWidgetComponent::SetShieldBar(float InCurrent, float InMax)
{
	if (HpBarWidget == nullptr) return;

	//if (!GetOwner()->HasAuthority()) UE_LOG(LogTemp, Display, TEXT("SetShieldBar"));

	HpBarWidget->GetShieldBar()->SetPercent(InCurrent / InMax);



	//ShieldRate = Rate;


	//float ReplenishThisFrame = Rate * DeltaTime;

	//float FillAmount = FMath::Clamp(mCurrentShield + ReplenishThisFrame, 0.f, mMaxShield);

	////ABlasterCharacter* BC = Cast<ABlasterCharacter>(GetOwner());
	////if (BC)
	////{
	////	BC->UpdateHUDShield();
	////}

	////OwnerAttribute->SetCurrentShield(FillAmount);
	////BlasterPlayerController->UpdateHUDShield();

	////UE_LOG(LogTemp, Display, TEXT("FillAmount : %f"), FillAmount);
	//mAmount -= ReplenishThisFrame;
	//mCurrentShield = FillAmount;

	//if (mAmount < 0.f || mCurrentShield >= mMaxShield)
	//{
	//	//bReplenishingShield = false;
	//	mAmount = 0.f;
	//	bShieldChanged = false;
	//}


}

void UHpBarWidgetComponent::SetParryBar(float InCurrentP, float InMaxP)
{
	if (HpBarWidget == nullptr) return;

	HpBarWidget->GetParryBar()->SetPercent(InCurrentP / InMaxP);
}

void UHpBarWidgetComponent::ParryGaugeAnimStart(bool InCheck)
{
	HpBarWidget->ParryGaugeAnimStart(InCheck);
}

void UHpBarWidgetComponent::ResetHpBarTimer()
{
	if (GetOwner()->HasAuthority())
	{
		HpCountdown = 0.f;
		MulticastHpBarVisible(true);
	}
}

void UHpBarWidgetComponent::CheckHpBarWidget(float DeltaTime)
{
	if (!GetOwner()->HasAuthority()) return;

	//UE_LOG(LogTemp, Display, TEXT("HpCountdown : %f"), HpCountdown);

	if (IsVisible())
	{
		HpCountdown += DeltaTime;

		if (HpCountdown >= 3.f)
		{
			MulticastHpBarVisible(false);
			HpCountdown = 0.f;

		}

	}
}

void UHpBarWidgetComponent::MulticastHpBarVisible_Implementation(bool InIsVisible)
{
	if (InIsVisible)
	{
		SetComponentTickEnabled(true);
	}
	SetVisibility(InIsVisible);
}
