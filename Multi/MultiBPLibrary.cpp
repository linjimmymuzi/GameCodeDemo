// Copyright 2024 Jimmy Lin


#include "MultiBPLibrary.h"

#include "AI/NavigationSystemBase.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "MultiCharacter.h"
#include "MultiPlayerController.h"
#include "MultiGameMode.h"
#include "MultiGameStateBase.h"

AMultiCharacter* UMultiBPLibrary::GetMultiCharacter(const UObject* WorldContextObject, int32 PlayerIndex)
{
	if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(WorldContextObject, PlayerIndex))
	{
		return Cast<AMultiCharacter>(Character);
	}
	return nullptr;
}

AMultiPlayerController* UMultiBPLibrary::GetMultiPlayerController(const UObject* WorldContextObject, int32 PlayerIndex)
{
	if (APlayerController* Controller = UGameplayStatics::GetPlayerController(WorldContextObject, PlayerIndex))
	{
		return Cast<AMultiPlayerController>(Controller);
	}
	return nullptr;
}

AMultiGameMode* UMultiBPLibrary::GetMultiGameMode(const UObject* WorldContextObject)
{
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(WorldContextObject))
	{
		return Cast<AMultiGameMode>(GameMode);
	}
	return nullptr;
}

AMultiGameStateBase* UMultiBPLibrary::GetMultiGameState(const UObject* WorldContextObject)
{
	if (AGameStateBase* MyGameState = UGameplayStatics::GetGameState(WorldContextObject))
	{
		return Cast<AMultiGameStateBase>(MyGameState);
	}
	return nullptr;
}
