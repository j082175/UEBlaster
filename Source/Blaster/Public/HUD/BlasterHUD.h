// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	FHUDPackage()
		: CrosshairSpread(0.f), CrosshairsColor(FLinearColor::Black)
	{}
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> CrosshairCenter;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> CrosshairLeft;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> CrosshairRight;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> CrosshairTop;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> CrosshairBottom;
	UPROPERTY(EditAnywhere)
	float CrosshairSpread;
	UPROPERTY(EditAnywhere)
	FLinearColor CrosshairsColor;
};

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()
public:
	ABlasterHUD();

protected:
	virtual void BeginPlay() override;


public:
	virtual void DrawHUD() override;
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	UPROPERTY()
	class UAnnouncement* Announcement;

	void AddAnnouncement();
	void AddElimAnnouncement(FString Attacker, FString Victim);
	void AddCharacterOverlay();




public:
	UFUNCTION(BlueprintCallable)
	void DrawCrosshairs();
private:
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = true))
	void DrawCrosshair(class UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor InCrosshairsColor);

private:
	UFUNCTION()
	void ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove);

	UPROPERTY()
	class APlayerController* OwingPlayer;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FHUDPackage HUDPackage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float CrosshairSpreadMax = 16.f;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> AnnouncementClass;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UElimAnnouncement> ElimAnnouncementClass;

	UPROPERTY(EditAnywhere)
	float ElimAnnouncementTime = 3.f;

	UPROPERTY()
	TArray<TObjectPtr<UElimAnnouncement>> ElimMessages;

	
};
