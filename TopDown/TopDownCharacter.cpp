// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownCharacter.h"

#include "CombatTextWidget.h"
#include "PuzzleRotate.h"
#include "TopDownMenu.h"
#include "TopDownPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Components/WidgetComponent.h"

ATopDownCharacter::ATopDownCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	CombatTextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CombatTextWidget"));
	CombatTextWidget->SetupAttachment(RootComponent);
	CombatTextWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ATopDownCharacter::Tick(float DeltaSeconds)
{
	if (bIsDashing)
	{
		AddMovementInput(GetActorForwardVector(), 1.0f);
	}

	for (int Index = InvulnerableRequestors.Num() - 1; Index > 0; Index--)
	{
		if (!InvulnerableRequestors[Index].IsValid())
		{
			InvulnerableRequestors.RemoveAt(Index);
		}
	}
    Super::Tick(DeltaSeconds);
}

void ATopDownCharacter::OnEnterBlueTrigger()
{
	NumBlueTriggers++;
	if (NumBlueTriggers == 1)
	{
		FVector NewColor(BlueColor.R, BlueColor.G, BlueColor.B);
		GetMesh()->SetVectorParameterValueOnMaterials(BodyColorParameter, NewColor);
	}
}

void ATopDownCharacter::OnExitBlueTrigger()
{
	NumBlueTriggers--;
	if (NumBlueTriggers == 0)
	{
		FVector NewColor(DefaultColor.R, DefaultColor.G, DefaultColor.B);
		GetMesh()->SetVectorParameterValueOnMaterials(BodyColorParameter, NewColor);
	}
}

void ATopDownCharacter::StartDash()
{
	if (!bIsDashing)
	{
		if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
		{
			if (MovementComponent->IsCrouching())
			{
				UnCrouch();
			}
			MovementComponent->MaxWalkSpeed *= DashSpeedMultiplier;
			MovementComponent->MaxAcceleration *= DashAccelMultiplier;
		}
		
		GetMesh()->GetAnimInstance()->Montage_Play(DashMontage);
		bIsDashing = true;
		float MontageDuration = GetMesh()->GetAnimInstance()->Montage_Play(DashMontage);
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ATopDownCharacter::EndDash,MontageDuration, false);
	}
}

bool ATopDownCharacter::GetIsDashing()
{
	return bIsDashing;
}

void ATopDownCharacter::EndDash()
{
	bIsDashing = false;
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed /= DashSpeedMultiplier;
		MovementComponent->MaxAcceleration /= DashAccelMultiplier;
	}
}

bool ATopDownCharacter::CanCrouch() const
{
	if (bIsDashing)
	{
		return false;
	}
	return Super::CanCrouch();
}

void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

float ATopDownCharacter::GetCurrentHealth() const
{
	return CurrentHealth;
}

float ATopDownCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CombatTextWidget && CombatTextWidget->GetWidget())
	{
		UCombatTextWidget* CombatText = Cast<UCombatTextWidget>(CombatTextWidget->GetWidget());
		if (CombatText)
		{
			CombatText->ShowDamage(Damage);
		}
	}
	
	if (Damage > 0)
	{
		CurrentHealth -= Damage;
		if (CurrentHealth <= 0)
		{
			bIsDead = true;
			float DeathAnimDuration = GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);
			// Get this pawn's controller as a APlayerController
			APlayerController* PC = GetController<APlayerController>();
			// Call disable input on the controller (it's silly, but you have to pass in itself)
			//PC->DisableInput(PC);
			//Call finish death 0.25 seconds before montage ends
			GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ATopDownCharacter::FinishDeath,DeathAnimDuration - 0.25f, false);

			if (TopDownMenuClass)
			{
				TopDownMenuInstance = NewObject<UTopDownMenu>(this, TopDownMenuClass);
				TopDownMenuInstance->AddToViewport();
				TopDownMenuInstance->SetOwningPlayer(PC);
				UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, TopDownMenuInstance);
			}
		}
	}

	return Damage;
}

bool ATopDownCharacter::ShouldTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser) const
{
	if (bIsDead || !InvulnerableRequestors.IsEmpty())
	{
		return false;
	}
	return Super::ShouldTakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}


void ATopDownCharacter::FinishDeath()
{
	GetMesh()->Deactivate();
}

void ATopDownCharacter::AddRequestors(UObject* Requestor)
{
	InvulnerableRequestors.Add(Requestor);
}

void ATopDownCharacter::RemoveRequestors(UObject* Requestor)
{
	InvulnerableRequestors.Remove(Requestor);
}

void ATopDownCharacter::StartProne()
{
	bIsProne = true;
	GetCharacterMovement()->StopMovementImmediately();
}

void ATopDownCharacter::EndProne()
{
	bIsProne = false;
}

bool ATopDownCharacter::GetIsProne() const
{
	return bIsProne;
}
