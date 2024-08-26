// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"
#include "Characters/CharacterBase.h"
#include "PlayerController/BlasterPlayerController.h"
#include "Net/UnrealNetwork.h"

UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetIsReplicatedByDefault(true);
	//SetIsReplicated(true);


	CurrentHp = MaxHp;
	CurrentSp = MaxSp;
	CurrentShield = MaxShield;
	CurrentParryGauge = 0.f;
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//SpRecoveringRate = 1.f;

	Owner = Cast<ACharacterBase>(GetOwner());

	OnParryGaugeChanged.Broadcast(0.f, MaxParryGauge);

	Init();
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Owner) UE_LOG(LogTemp, Display, TEXT("%s : AttributeComponent tick"), *Owner->GetName());

	// ...
	Owner = Owner == nullptr ? Cast<ACharacterBase>(GetOwner()) : Owner;

	if (bHasSpChanged)
	{
		RecoveringStartTimeCheck = 0.f;
		bHasSpChanged = false;
	}

	if (RecoveringStartTimeCheck >= RecoveringStartThreshold)
	{
		CurrentSp = RecoveringResourceRate(CurrentSp, MaxSp, SpRecoveringRate, DeltaTime);
	}
	else
	{
		RecoveringStartTimeCheck += DeltaTime;
	}


	if (Owner->HasAuthority()) CheckParryGaugeMaximum(DeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("%s : bIsParryGaugeAnimPlaying : %d"), *UEnum::GetDisplayValueAsText(Owner->GetLocalRole()).ToString(),  bIsParryGaugeAnimPlaying);

	
}

void UAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentHp);
	DOREPLIFETIME(ThisClass, CurrentShield);
	DOREPLIFETIME(ThisClass, CurrentParryGauge);
	DOREPLIFETIME(ThisClass, bIsParryGaugeAnimPlaying);
}

void UAttributeComponent::Init()
{
	//FTimerHandle T1;
	//GetWorld()->GetTimerManager().SetTimer(T1, FTimerDelegate::CreateLambda([&]()
	//	{
	//		OnHpChanged.Broadcast(GetCurrentHp(), GetMaxHp());
	//		OnShieldChanged.Broadcast(CurrentShield, MaxShield);
	//		OnSpChanged.Broadcast(CurrentSp, MaxSp);
	//		OnParryGaugeChanged.Broadcast(CurrentParryGauge, MaxParryGauge);
	//		//UE_LOG(LogTemp, Warning, TEXT("Attribute Initialized!"));
	//	}), 0.5f, false);

	OnHpChanged.Broadcast(GetCurrentHp(), GetMaxHp());
	OnShieldChanged.Broadcast(CurrentShield, MaxShield);
	OnSpChanged.Broadcast(CurrentSp, MaxSp);
	OnParryGaugeChanged.Broadcast(CurrentParryGauge, MaxParryGauge);
}

void UAttributeComponent::InitStatus()
{
	CurrentHp = MaxHp;
	CurrentShield = MaxShield;
	CurrentSp = MaxSp;
	CurrentParryGauge = 0.f;

	OnParryGaugeChanged.Broadcast(0.f, MaxParryGauge);
}


float UAttributeComponent::RecoveringResourceRate(float CurrentVal, float MaxVal, float Rate, float InDeltaTime)
{
	OnSpChanged.Broadcast(GetCurrentSp(), GetMaxSp());
	return FMath::Clamp(CurrentVal + Rate * InDeltaTime, 0, MaxVal);
}

float UAttributeComponent::DecreaseResourceRate(float CurrentVal, float MaxVal, float Rate, float InDeltaTime)
{
	OnParryGaugeChanged.Broadcast(GetCurrentParryGauge(), GetMaxParryGauge());
	return FMath::Clamp(CurrentVal - Rate * InDeltaTime, 0, MaxVal);
}

void UAttributeComponent::CheckParryGaugeMaximum(float DeltaTime)
{
	//UE_LOG(LogTemp, Display, TEXT("CurrentParryGauge : %f"), CurrentParryGauge);
	if (CurrentParryGauge + 1.f > MaxParryGauge)
	{
		ParryGaugeCoolDownCheck += DeltaTime;
		if (ParryGaugeCoolDownCheck >= ParryGaugeCoolDown)
		{
			ParryGaugeCoolDownCheck = 0.f;
			CurrentParryGauge -= 1.f;
			OnParryGaugeAnim.Broadcast(false);
			bIsParryGaugeAnimPlaying = false;
			SetComponentTickEnabled(false);
		}
		else
		{
			if (!bIsParryGaugeAnimPlaying) OnParryGaugeAnim.Broadcast(true);
			bIsParryGaugeAnimPlaying = true;
			return;
		}
	}

	if (CurrentParryGauge != 0.f)
	{
		CurrentParryGauge = DecreaseResourceRate(CurrentParryGauge, MaxParryGauge, ParryGaugeDecreasingRate, DeltaTime);

	}
}

void UAttributeComponent::OnRep_Health(float LastHealth)
{


	/*Hp has been changed->Broadcast!*/


	if (CurrentHp != LastHealth)
	{
		//HpBarWidgetComponent->SetHpBar(CurrentHp / MaxHp);
		OnHpChanged.Broadcast(CurrentHp , MaxHp);
		//PlayerController->UpdateHUDHealth();
		//Owner->PlayHitReactMontage();

		if (Owner)
		{
			ABlasterPlayerController* PlayerController = Cast<ABlasterPlayerController>(Owner->GetController());
			if (PlayerController)
			{
				PlayerController->SetHUDHealth(CurrentHp, MaxHp);
				OnHpChanged.Broadcast(CurrentHp , MaxHp);
			}
		}

	}
}

void UAttributeComponent::OnRep_Shield(float LastShield)
{
	//UE_LOG(LogTemp, Display, TEXT("%s : OnRep_Shield : Cu : %f, La : %f"), *UEnum::GetDisplayValueAsText(GetOwner()->GetLocalRole()).ToString(), CurrentShield, LastShield);
	/*Hp has been changed->Broadcast!*/

	//UE_LOG(LogTemp, Display, TEXT("CurrentShield : %f"), CurrentShield);
	//UE_LOG(LogTemp, Display, TEXT("LastShield : %f"), LastShield);



	if (CurrentShield != LastShield)
	{
		//PlayerController->UpdateHUDShield();
		//UE_LOG(LogTemp, Display, TEXT("OnRep_Shield, LastShield : %f"), LastShield);
		//Owner->PlayCombatHitReactMontage();

		OnShieldChanged.Broadcast(CurrentShield , MaxShield);

		if (Owner)
		{
			ABlasterPlayerController* PlayerController = Cast<ABlasterPlayerController>(Owner->GetController());
			if (PlayerController)
			{
				PlayerController->SetHUDShield(CurrentShield, MaxShield);
			}
		}

	}


}

void UAttributeComponent::OnRep_ParryGauge(float LastParryGauge)
{
	if (CurrentParryGauge != LastParryGauge)
	{
		OnParryGaugeChanged.Broadcast(CurrentParryGauge, MaxParryGauge);
	}
}

void UAttributeComponent::OnRep_ParryGaugeAnim()
{
	//UE_LOG(LogTemp, Display, TEXT("OnRep_ParryGaugeAnim : %d"), bIsParryGaugeAnimPlaying);
	if (bIsParryGaugeAnimPlaying)
	{
		OnParryGaugeAnim.Broadcast(true);
	}
	else
	{
		OnParryGaugeAnim.Broadcast(false);
	}
}