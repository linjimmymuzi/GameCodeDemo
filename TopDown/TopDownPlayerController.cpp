// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "TopDownCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "HUDWidget.h"
#include "InteractSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ATopDownPlayerController::ATopDownPlayerController()
{
	bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ATopDownPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void ATopDownPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = NewObject<UHUDWidget>(this, HUDWidgetClass);
		HUDWidgetInstance->AddToViewport();
	}
}

void ATopDownPlayerController::OnJumpAction()
{
	ACharacter* MyCharactor = GetCharacter();
	if (MyCharactor != nullptr && !bLockMovement)
	{
		MyCharactor->GetCharacterMovement()->DoJump(false);
	}
}

void ATopDownPlayerController::OnMoveForward(const FInputActionInstance& Instance)
{
	if (APawn* MyPawn = GetPawn())
	{
		if (!Cast<ATopDownCharacter>(MyPawn)->GetIsDashing() && !bLockMovement)
		{
			float AxisValue = Instance.GetValue().Get<float>();
			MyPawn->AddMovementInput(FVector::ForwardVector, AxisValue);
		}
	}
}

void ATopDownPlayerController::OnMoveRight(const FInputActionInstance& Instance)
{
	if (APawn* MyPawn = GetPawn())
	{
		if (!Cast<ATopDownCharacter>(MyPawn)->GetIsDashing() && !bLockMovement)
		{
			float AxisValue = Instance.GetValue().Get<float>();
			MyPawn->AddMovementInput(FVector::RightVector, AxisValue);
		}
	}
}

void ATopDownPlayerController::OnInteractAction()
{
	if (UInteractSubsystem* Subsystem = GetWorld()->GetSubsystem<UInteractSubsystem>())
	{
		Subsystem->PerformInteract();
	}
}

void ATopDownPlayerController::OnToggleCrouchAction()
{
	if (ACharacter* MyCharactor = GetCharacter())
	{
		ATopDownCharacter* MyCharacter = Cast<ATopDownCharacter>(MyCharactor);
		if (!MyCharacter->bIsCrouched && !MyCharacter->GetIsProne())
		{
			MyCharactor->Crouch();
		}
		else if (MyCharacter->bIsCrouched)
		{
			MyCharactor->UnCrouch();
		}
	}
}

void ATopDownPlayerController::OnDashAction()
{
	ATopDownCharacter* MyCharacter = GetPawn<ATopDownCharacter>();
	if (MyCharacter != nullptr && !bLockMovement)
	{
		MyCharacter->StartDash();
	}
}

void ATopDownPlayerController::OnToggleProneAction()
{
	if (ATopDownCharacter* MyCharacter = GetPawn<ATopDownCharacter>())
	{
		if (!MyCharacter->GetIsProne() && !MyCharacter->bIsCrouched)
		{
			MyCharacter->StartProne();
			bLockMovement = true;
		}
		else if (MyCharacter->GetIsProne())
		{
			MyCharacter->EndProne();
			bLockMovement = false;
		}
	}
}

void ATopDownPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
	
	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		//Call OnMoveForward whenever the MoveForwardAction is triggered
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::OnMoveForward);
		//Call OnMoveRight whenever the MoveRightAction is triggered
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::OnMoveRight);
		//Call OnJumpAction whenever the JumpAction is triggered
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::OnJumpAction);
		//Call OnInteractAction whenever the Interaction Action is triggered
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::OnInteractAction);
		//Call OnToggleCrouchAction whenever the ToggleCrouch Action is triggered
		EnhancedInputComponent->BindAction(ToggleCrouchAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::OnToggleCrouchAction);
		//Call OnDashAction whenever the Dash Action is triggered
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::OnDashAction);
		//Call OnToggleProneAction whenever the Prone Action is triggered
		EnhancedInputComponent->BindAction(ToggleProneAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::OnToggleProneAction);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
