// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "MultiCharacter.h"
#include "MultiProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	SetIsReplicatedByDefault(true);
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (Character->GetAmmo() > 0)
	{
		// Try and fire a projectile
        if (ProjectileClass != nullptr)
        {
        	UWorld* const World = GetWorld();
        	if (World != nullptr)
        	{
        		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
        		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
        		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
        		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
        
        		//Set Spawn Collision Handling Override
        		FActorSpawnParameters ActorSpawnParams;
        		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
        		ActorSpawnParams.Instigator = Character;
        		// Spawn the projectile at the muzzle
        		World->SpawnActor<AMultiProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
        	}
        }
        
        MulticastFireEffects();
	}
	
}

USkeletalMeshComponent* UTP_WeaponComponent::GetMesh3P()
{
	if (Character)
	{
		return Character->GetMesh3P();
	}

	return nullptr;

}

bool UTP_WeaponComponent::AttachWeapon(AMultiCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}
}

void UTP_WeaponComponent::MulticastFireEffects_Implementation()
{
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	
	UAnimInstance* AnimInstance3P = GetMesh3P()->GetAnimInstance();
	if (FireAnimation3P!=nullptr && AnimInstance3P!=nullptr)
	{
		GetMesh3P()->GetAnimInstance()->Montage_Play(FireAnimation3P);
	}
	
}
