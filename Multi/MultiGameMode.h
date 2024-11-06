// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiGameMode.generated.h"

UCLASS(minimalapi)
class AMultiGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMultiGameMode();

	virtual void GenericPlayerInitialization(AController* C) override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override {return false;}
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	//Time in sec before the game start
	UPROPERTY(EditDefaultsOnly)
	float WaitingToStartDuration = 5.0f;

	virtual bool ReadyToEndMatch_Implementation() override;

	virtual void HandleMatchHasEnded() override;

private:
	int RedTeamCount = 0;
	int BlueTeamCount = 0;

	FTimerHandle RestartTimerHandle;
};



