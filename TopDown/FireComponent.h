// Copyright 2024 Han Lin - linjimmy@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "FireComponent.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFireComponentCatchFire);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPDOWN_API UFireComponent : public USphereComponent
{
	GENERATED_BODY()
	
public:
	UFireComponent();

	/**
	* Logic for overlapping
	* @param OverlappedComponent - The component that initiated the overlap.
	* @param OtherActor - The other actor being overlapped.
	* @param OtherComp - The other component being overlapped.
	* @param OtherBodyIndex - The body index of the other component.
	* @param bFromSweep - Indicates if the overlap was triggered by a sweep movement.
	* @param SweepResult - Provides additional information about the sweep.
	*/
	UFUNCTION(BlueprintCallable)
	void HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void CatchFireNative();

	UPROPERTY(BlueprintAssignable)
	FFireComponentCatchFire OnCatchFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOnFire;
	
	UPROPERTY(EditAnywhere)
	bool bStartOnFire;

	UPROPERTY(EditAnywhere)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* FireParticle;
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
