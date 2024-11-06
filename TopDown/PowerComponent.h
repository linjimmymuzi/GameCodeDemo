// Copyright 2024 Han Lin - linjimmy@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PowerComponent.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPowerType : uint8
{
	Generator,
	Receiver,
	Output
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangePowerState, bool, bNewPoweredState);

UCLASS(meta=(BlueprintSpawnableComponent))
class TOPDOWN_API UPowerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UPowerComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsPowered;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPowerType PowerType;

	UPROPERTY(BlueprintAssignable)
	FOnChangePowerState OnChangePowerState;
	
	void SetPower(bool IsOnPower);
	
	void PropagatePower();

protected:
	virtual void BeginPlay() override;
	/**
	 * Handle OnBeginOverlap for Generator
	 * @param OverlappedComponent - The component being overlapped with otherComp
	 * @param OtherActor - The actor that owns the other overlapping component
	 * @param OtherComp - Overlapping Component
	 * @param OtherBodyIndex - Index of the body that triggered the overlap event
	 * @param bFromSweep - if triggered by a sweeping movement
	 * @param SweepResult - Result of the overlap event
	 */
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

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
