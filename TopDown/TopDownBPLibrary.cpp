// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "TopDownBPLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "TopDownCharacter.h"

ATopDownCharacter* UTopDownBPLibrary::GetTopDownCharacter(const UObject* WorldContextObject, int32 PlayerIndex)
{
	if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(WorldContextObject, PlayerIndex))
	{
		return Cast<ATopDownCharacter>(Character);
	}
	return nullptr;
}

ATopDownPlayerController* UTopDownBPLibrary::GetTopDownController(const UObject* WorldContextObject, int32 PlayerIndex)
{
	if (APlayerController* Controller = UGameplayStatics::GetPlayerController(WorldContextObject, PlayerIndex))
	{
		return Cast<ATopDownPlayerController>(Controller);
	}
	return nullptr;
}

ATopDownGameMode* UTopDownBPLibrary::GetTopDownGameMode(const UObject* WorldContextObject)
{
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(WorldContextObject))
	{
		return Cast<ATopDownGameMode>(GameMode);
	}
	return nullptr;
}
