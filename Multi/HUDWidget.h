// Copyright 2024 Jimmy Lin

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTI_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetStartMatchText(const FString& StartText, FLinearColor Color, float Duration);

	void SetEndMatchText(const FString& EndText, FLinearColor Color, float Duration);
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> RedTeamScoreTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> BlueTeamScoreTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> AmmoTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Timer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> StartMatchText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> EndMatchText;
private:
	float StartMatchDuration = 0.0f;

	float EndMatchDuration = 0.0f;
};
