// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverlayModules/PingStatus.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UPingStatus::UpdatePing(float InPing)
{
	PingText->SetText(FText::FromString(FString::Printf(TEXT("%f"), InPing)));
}


void UPingStatus::HighPingWarning()
{
	HighPingImage->SetOpacity(1.f);
	PlayAnimation(HighPingAnimation, 0.f, 10);
}

void UPingStatus::StopHighPingWarning()
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	HighPingImage->SetOpacity(0.f);
	if (IsAnimationPlaying(HighPingAnimation))
	{
		StopAnimation(HighPingAnimation);
	}
}
