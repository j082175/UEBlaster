// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChatSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UChatSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChatSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
public:

	FORCEINLINE TSubclassOf<class UChatBox> GetChatBoxClassToInit() const { return ChatBoxClass; }
	FORCEINLINE void SetChatBox(class UChatBox* InChatBox) { ChatBox = InChatBox; }

	UFUNCTION(Client, Reliable)
	void ClientChatCommitted(const FText& Text, const FString& PlayerName);

	void AddChatBox();

	void ChatButtonPressed();
protected:
	UFUNCTION()
	void OnChatCommittedFunc(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION(Server, Reliable)
	void ServerChatCommitted(const FText& Text, const FString& PlayerName);

	void OpenChatBox();


	class APlayerController* OwingPlayerController;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UChatBox> ChatBoxClass;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UChatBox> ChatBox;
};
