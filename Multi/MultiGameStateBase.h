// Copyright 2024 Jimmy Lin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MultiGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class MULTI_API AMultiGameStateBase : public AGameState
{
	GENERATED_BODY()

public:
	AMultiGameStateBase();

	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(Replicated)
	int RedTeamScore = 0;

	UPROPERTY(Replicated)
	int BlueTeamScore = 0;

	UPROPERTY(Replicated)
	float WaitingToStartTime = 0.0f;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void HandleMatchIsWaitingToStart() override;

	UFUNCTION(Reliable, NetMulticast)
	void ShowStartMatch(const FString& StartText, FLinearColor Color, float Duration);

	UFUNCTION(Reliable, NetMulticast)
	void ShowEndMatch(const FString& WinningTeam, FLinearColor TeamColor, float Duration);
	

};
