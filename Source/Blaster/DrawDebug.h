#pragma once

#define DRAW_LINE(Start, End, Thickness) if(GetWorld()) DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1.f, 0, Thickness);
#define DRAW_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false, -1.f);
#define DRAW_VECTOR(Start, End, Thickness) if(GetWorld()) DRAW_LINE(Start, End, Thickness) DRAW_POINT(End);


static void AddOnScreenDebugMessage(UPrimitiveComponent * OverlappedComponent = nullptr, AActor * OtherActor = nullptr, UPrimitiveComponent * OtherComp = nullptr, int32 OtherBodyIndex = 0, bool bFromSweep = false, const FHitResult & SweepResult = FHitResult())
{
	FString DebugMessage = FString::Printf(TEXT("OverlappedComponent : %s\n OtherActor : %s\n OtherComp : %s\n OtherBodyIndex : %d\n"), *OverlappedComponent->GetName(), *OtherActor->GetName(), *OtherComp->GetName(), OtherBodyIndex);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Cyan, DebugMessage);
	}
}

static void NullChecker(UObject * InObject, const FString& ObjectName, const FString & Name)
{
	if (!InObject) UE_LOG(LogTemp, Error, TEXT("NullChecker : %s's %s is NULL"), *Name, *ObjectName);
}
