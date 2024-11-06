// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUDWidget.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "TopDownCharacter.generated.h"

enum class EThreatLevel : int;

UCLASS(Blueprintable)
class ATopDownCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATopDownCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable)
	void OnEnterBlueTrigger();

	UFUNCTION(BlueprintCallable)
	void OnExitBlueTrigger();

	void StartDash();

	bool GetIsDashing();

	void EndDash();

	virtual bool CanCrouch() const override;

	virtual void BeginPlay() override;

	float GetCurrentHealth() const;
	float GetHealthPct() const {return CurrentHealth/MaxHealth;}

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual bool ShouldTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const override;

	void FinishDeath();

	void AddRequestors(UObject* Requestor);

	void RemoveRequestors(UObject* Requestor);

	void StartProne();

	void EndProne();

	bool GetIsProne() const;

	void IncreaseStealthCount(){StealthCount++;}
	void DecreaseStealthCount(){StealthCount = FMath::Max(StealthCount - 1, 0);}
	int GetStealthCount() const {return StealthCount;}

	bool IsDead() const {return bIsDead;}

	EThreatLevel GetThreatLevel() const {return CurrentThreatLevel;}
	void SetThreatLevel(EThreatLevel threatLevel) {CurrentThreatLevel = threatLevel;}
	
protected:
	UPROPERTY(BlueprintReadOnly)
	int NumBlueTriggers = 0;

	UPROPERTY(Transient, BlueprintReadOnly)
	bool bIsDashing = false;

	UPROPERTY(EditAnywhere)
	float DashSpeedMultiplier = 2.0f;

	UPROPERTY(EditAnywhere)
	float DashAccelMultiplier = 5.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.0f;

	UPROPERTY(Transient, BlueprintReadOnly)
	float CurrentHealth;

	bool bIsDead = false;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<UObject>> InvulnerableRequestors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsProne = false;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UTopDownMenu> TopDownMenuClass;

	UPROPERTY(Transient)
	TObjectPtr<class UTopDownMenu> TopDownMenuInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> CombatTextWidget;

	EThreatLevel CurrentThreatLevel = EThreatLevel::Red;
	
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor DefaultColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor BlueColor = FLinearColor::Blue;

	UPROPERTY(EditDefaultsOnly)
	FName BodyColorParameter = FName(TEXT("Tint"));

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> DashMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> DeathMontage;
	
	FTimerHandle TimerHandle;

	FTimerHandle DeathTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	int StealthCount = 0;
};
