// Copyright Epic Games, Inc. All Rights Reserved.


#include "MultiPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "HUDWidget.h"
#include "MultiCharacter.h"
#include "MultiPlayerState.h"

void AMultiPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMultiCharacter* MyCharacter = Cast<AMultiCharacter>(InPawn);
	AMultiPlayerState* state = InPawn->GetPlayerState<AMultiPlayerState>();

	if (MyCharacter && state)
	{
		MyCharacter->MultiTeam = state->MultiTeam;
	}
	
	ClientOnPossess();
}

void AMultiPlayerController::ClientOnPossess_Implementation()
{
	
}

void AMultiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	if (HUDWidgetClass && IsLocalController())
	{
		HUDWidgetInstance = NewObject<UHUDWidget>(this, HUDWidgetClass);
		HUDWidgetInstance->AddToViewport();
	}
}

void AMultiPlayerController::SetStartMatchText(const FString& StartText, FLinearColor Color, float Duration)
{
	HUDWidgetInstance->SetStartMatchText(StartText, Color, Duration);
}

void AMultiPlayerController::SetEndMatchText(const FString& EndText, FLinearColor Color, float Duration)
{
	HUDWidgetInstance->SetEndMatchText(EndText, Color, Duration);
}
