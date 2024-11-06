// Copyright 2024 Han Lin - linjimmy@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CombatTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API UCombatTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowDamage(int Damage);

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DamageTextBlock;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> TextFly;
};
