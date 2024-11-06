// Copyright 2024 Han Lin - linjimmy@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopDownPlayerController.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
enum class EThreatLevel : int
{
	Purple = 1,
	Red = 0,
	Yellow = -1,
	Blue = -2,
	Green = -3
};

UCLASS()
class TOPDOWN_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	EThreatLevel ThreatLevel = EThreatLevel::Red;
	void UpdateThreatLevel(int NewThreatLevel);
	void UpdateThreatLevelColor();
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> HealthAmount;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor DefaultHealthColor = FLinearColor::Green;
	
	UPROPERTY(EditDefaultsOnly)
	FLinearColor DangerHealthColor = FLinearColor::Yellow;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor CriticalHealthColor = FLinearColor::Red;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> ThreatLevelIndicator;

	float LastMovementTime = -100.0f;
};
