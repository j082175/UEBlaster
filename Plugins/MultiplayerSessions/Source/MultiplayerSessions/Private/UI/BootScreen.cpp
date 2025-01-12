// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BootScreen.h"
#include "Animation/WidgetAnimation.h"
//#include "Interfaces/CommonPCFuncInterface.h"
#include "Enums/MapNames.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Loading.h"

UBootScreen::UBootScreen(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<ULoading> LoadingClassRef(TEXT("/MultiplayerSessions/Widgets/WBP_Loading.WBP_Loading_C"));
	ensure(LoadingClassRef.Class);
	LoadingClass = LoadingClassRef.Class;
}

void UBootScreen::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimationForward(FadeIn);

	//if (ICommonPCFuncInterface* CommonFunc = Cast<ICommonPCFuncInterface>(GetOwningPlayer()))
	//{
	//	CommonFunc->IShowLoading();

	//}

}

void UBootScreen::Travel()
{
	LoadingWidget->RemoveFromParent();
}

void UBootScreen::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (FadeIn == Animation)
	{
		LoadingWidget = CreateWidget<ULoading>(this, LoadingClass);
		LoadingWidget->AddToViewport();

		UGameplayStatics::OpenLevel(this, *UEnum::GetDisplayValueAsText(EDefaultMaps::StartupMap_SciFi_Dynamic).ToString(), true, TEXT("listen"));

		FTimerHandle H;
		GetWorld()->GetTimerManager().SetTimer(H, this, &ThisClass::Travel, 2.f);
	}
}
