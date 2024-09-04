// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CoolTimeAnnouncement.h"
#include "Components/SkillComponent.h"
#include "Components/TextBlock.h"

void UCoolTimeAnnouncement::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);
}

void UCoolTimeAnnouncement::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsActive)
	{
		if (DisableTimeCount >= DisableTimeThreshold)
		{
			DisableTimeCount = 0.f;
			SetVisibility(ESlateVisibility::Collapsed);
			bIsActive = false;
		}
		else
		{
			DisableTimeCount += InDeltaTime;
		}
	}

}

void UCoolTimeAnnouncement::SetCoolTimeAnnouncementText(int32 InSkillIndex)
{
	if (USkillComponent* SC = GetOwningPlayerPawn()->GetComponentByClass<USkillComponent>())
	{
		FCoolTimeCheckStruct* CTCS = SC->CoolTimeMap.Find(UEnum::GetDisplayValueAsText(ESkillAssistant(InSkillIndex)).ToString());

		if (CTCS && !CTCS->bCanExecute)
		{
			FString Str = FString::Printf(TEXT("%s Is CoolTime or No Point"), *UEnum::GetDisplayValueAsText(ESkillAssistant(InSkillIndex)).ToString());

			CoolTimeAnnouncementText->SetText(FText::FromString(Str));
			SetVisibility(ESlateVisibility::Visible);
			bIsActive = true;
			DisableTimeCount = 0.f;
		}
	}

}
