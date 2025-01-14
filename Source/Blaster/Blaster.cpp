// Copyright Epic Games, Inc. All Rights Reserved.

#include "Blaster/Blaster.h"
#include "Modules/ModuleManager.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/UserWidget.h"

class FBlasterGameModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FBlasterGameModule::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();
}

void FBlasterGameModule::ShutdownModule()
{
	FDefaultGameModuleImpl::ShutdownModule();
}


IMPLEMENT_PRIMARY_GAME_MODULE(FBlasterGameModule, Blaster, "Blaster");

DEFINE_LOG_CATEGORY(LogABDisplay);
DEFINE_LOG_CATEGORY(LogABBeginPlay);
DEFINE_LOG_CATEGORY(LogABNetwork);
DEFINE_LOG_CATEGORY(LogABTeleport);


void WidgetAnimHelper::FindWidgetAnimationName(UUserWidget* InUserWidget, TMap<FString, class UWidgetAnimation*>& InWidgetAnimations)
{
	UWidgetBlueprintGeneratedClass* WidgetClass = InUserWidget->GetWidgetTreeOwningClass();

	InWidgetAnimations.Empty();

	for (int i = 0; i < WidgetClass->Animations.Num(); i++)
	{
		FString Name = WidgetClass->Animations[i]->GetName();
		InWidgetAnimations.Add(Name, WidgetClass->Animations[i]);
	}
}

UWidgetAnimation* WidgetAnimHelper::StartAnimation(const FString& InPrefix, const FString& InName, int32 InIndex, float InPlaybackSpeed, UUserWidget* InUserWidget, TMap<FString, class UWidgetAnimation*>& InWidgetAnimations)
{
	FString Str = "";
	if (InIndex == -1)
	{
		Str = FString::Printf(TEXT("%s%sAnim_INST"), *InPrefix, *InName);
	}
	else
	{
		Str = FString::Printf(TEXT("%s%sAnim%d_INST"), *InPrefix, *InName, InIndex);
	}

	//UE_LOG(LogTemp, Display, TEXT("StartAnimation str : %s"), *Str);

	if (InWidgetAnimations.Contains(Str))
	{
		UWidgetAnimation* WidgetAnimation = InWidgetAnimations[Str];

		if (WidgetAnimation)
		{
			InUserWidget->StopAnimation(WidgetAnimation);
			InUserWidget->PlayAnimation(WidgetAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f / InPlaybackSpeed);

			return WidgetAnimation;
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Blaster::StartAnimation Find WidgetAnimation Failed!!! : %s"), *Str);

	}

	return nullptr;
}

