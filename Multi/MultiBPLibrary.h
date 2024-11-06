// Copyright 2024 Jimmy Lin

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MultiBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MULTI_API UMultiBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Retrieves the multi character for the specified player.
	 * @param WorldContextObject - The world context from which to retrieve the character
	 * @param PlayerIndex - The index of the player (0 for the first player)
	 * @return The AMultiCharacter instance for the specified player
	 */
	UFUNCTION(BlueprintPure)
	static class AMultiCharacter* GetMultiCharacter(const UObject* WorldContextObject, int32 PlayerIndex);
	
	/**
	 * Retrieves the multi player controller for the specified player.
	 * @param WorldContextObject - The world context from which to retrieve the player controller
	 * @param PlayerIndex - The index of the player (0 for the first player)
	 * @return The AMultiPlayerController instance for the specified player
	 */
	UFUNCTION(BlueprintPure)
	static class AMultiPlayerController* GetMultiPlayerController(const UObject* WorldContextObject, int32 PlayerIndex);

	/**
	 * Retrieves the multi game mode instance.
	 * @param WorldContextObject - The world context from which to retrieve the game mode
	 * @return The AMultiGameMode instance
	 */
	UFUNCTION(BlueprintPure)
	static class AMultiGameMode* GetMultiGameMode(const UObject* WorldContextObject);

	/**
	 * Retrieves the multi game mode instance.
	 * @param WorldContextObject - The world context from which to retrieve the game mode
	 * @return The AMultiGameState instance
	 */
	UFUNCTION(BlueprintPure)
	static class AMultiGameStateBase* GetMultiGameState(const UObject* WorldContextObject);
};
