// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiCharacter.h"
#include "MultiProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MultiPlayerState.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"
#include "TP_WeaponComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMultiCharacter

AMultiCharacter::AMultiCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	
	//3P Character mesh
	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh3P"));
	Mesh3P->SetOwnerNoSee(true);
	Mesh3P->SetupAttachment(RootComponent);
	Mesh3P->bCastDynamicShadow = true;
	Mesh3P->CastShadow = true;
	Mesh3P->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Mesh3P->SetRelativeLocation(FVector(0.0f, 0.0f, -100.f));

	//3P Gun mesh
	Gun3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh3P"));
	Gun3P->SetOwnerNoSee(true);
	Gun3P->SetupAttachment(Mesh3P, FName(TEXT("GripPoint")));
	Gun3P->bCastDynamicShadow = true;
	Gun3P->CastShadow = true;
}

void AMultiCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	if (GetLocalRole() == ROLE_Authority && WeaponOnSpawn)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;
		WeaponActor = GetWorld()->SpawnActor<AActor>(WeaponOnSpawn, GetActorLocation(), FRotator::ZeroRotator, Params);
		UTP_WeaponComponent* WeaponComp = WeaponActor ? WeaponActor->FindComponentByClass<UTP_WeaponComponent>() : nullptr;
		
		if (WeaponComp)
		{
			WeaponComp->AttachWeapon(this);
		}
		
	}
}

void AMultiCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiCharacter, WeaponActor);
	DOREPLIFETIME(AMultiCharacter, MultiTeam);
	DOREPLIFETIME(AMultiCharacter, Ammo);
}

//////////////////////////////////////////////////////////////////////////// Input

void AMultiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMultiCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMultiCharacter::Look);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMultiCharacter::OnWeaponFireAction);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMultiCharacter::AddAmmo(int PlusAmmo)
{
	Ammo += PlusAmmo;
}


void AMultiCharacter::OnRep_WeaponActor()
{
	UTP_WeaponComponent* WeaponComp = WeaponActor ? WeaponActor->FindComponentByClass<UTP_WeaponComponent>() : nullptr;
		
	if (WeaponComp)
	{
		WeaponComp->AttachWeapon(this);
	}
}

void AMultiCharacter::OnWeaponFireAction()
{
	ServerFire();
}

void AMultiCharacter::ServerFire_Implementation()
{
	UTP_WeaponComponent* WeaponComp = WeaponActor ? WeaponActor->FindComponentByClass<UTP_WeaponComponent>() : nullptr;
		
	if (WeaponComp && Ammo > 0)
	{
		WeaponComp->Fire();
		ReduceAmmo();
	}
}

void AMultiCharacter::OnRep_Team()
{
	FVector TeamColor = (MultiTeam == EMultiTeam::Red) ? FVector(1.0f, 0.0f, 0.0f) : FVector(0.0f, 0.0f, 1.0f);
	Mesh3P->SetVectorParameterValueOnMaterials("BodyColor", TeamColor);
}

void AMultiCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMultiCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}