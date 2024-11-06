// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MultiPlayerState.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MultiCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMultiCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;
public:
	AMultiCharacter();

	USkeletalMeshComponent* GetMesh3P() const { return Mesh3P; }
	
protected:
	virtual void BeginPlay() override ;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//The indentation is correct in my Rider, it may be the problem of Google Drive
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> WeaponOnSpawn;

	//The indentation is correct in my Rider, it may be the problem of Google Drive
    UPROPERTY(ReplicatedUsing=OnRep_WeaponActor, BlueprintReadOnly, Transient)
    AActor* WeaponActor;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh3P;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> Gun3P;

	UFUNCTION()
	void OnRep_WeaponActor();

	void OnWeaponFireAction();

	UFUNCTION(Server, Reliable)
	void ServerFire();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(ReplicatedUsing=OnRep_Team)
	EMultiTeam MultiTeam;

	UFUNCTION()
	void OnRep_Team();
	
	
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Combat")
	int Ammo = 5;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	int GetAmmo() const { return Ammo; }

	void ReduceAmmo() {Ammo--;}

	void AddAmmo(int PlusAmmo);
};

