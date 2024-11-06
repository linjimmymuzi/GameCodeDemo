// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MultiPlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class MULTI_API AMultiPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHUDWidget> HUDWidgetClass;

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(Client, Reliable)
	void ClientOnPossess();
	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// Begin Actor interface
	
	virtual void BeginPlay() override;

public:
	void SetStartMatchText(const FString& StartText, FLinearColor Color, float Duration);
	void SetEndMatchText(const FString& EndText, FLinearColor Color, float Duration);

	UPROPERTY(Transient)
	TObjectPtr<class UHUDWidget> HUDWidgetInstance;
	
};
