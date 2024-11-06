// Copyright 2024 Han Lin - linjimmy@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "StealthBoxComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPDOWN_API UStealthBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UStealthBoxComponent();

	virtual void BeginPlay() override;
	
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
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/**
	 * Handle OnEndOverlap for Generator
	 * @param OverlappedComponent - The component being overlapped with otherComp
	 * @param OtherActor - The actor that owns the other overlapping component
	 * @param OtherComp - Overlapping Component
	 * @param OtherBodyIndex - Index of the body that triggered the overlap event
	 */
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
