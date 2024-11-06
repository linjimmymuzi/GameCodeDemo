// Copyright 2024 Han Lin - linjimmy@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "TopDownCharacter.h"
#include "TopDownGameMode.h"
#include "TopDownPlayerController.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TopDownBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API UTopDownBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    /**
     * Retrieves the top-down character for the specified player.
     * @param WorldContextObject - The world context from which to retrieve the character
     * @param PlayerIndex - The index of the player (0 for the first player)
     * @return The ATopDownCharacter instance for the specified player
     */
	UFUNCTION(BlueprintPure)
	static ATopDownCharacter* GetTopDownCharacter(const UObject* WorldContextObject, int32 PlayerIndex);
	
	/**
     * Retrieves the top-down player controller for the specified player.
     * @param WorldContextObject - The world context from which to retrieve the player controller
     * @param PlayerIndex - The index of the player (0 for the first player)
     * @return The ATopDownPlayerController instance for the specified player
     */
	UFUNCTION(BlueprintPure)
	static ATopDownPlayerController* GetTopDownController(const UObject* WorldContextObject, int32 PlayerIndex);

    /**
     * Retrieves the top-down game mode instance.
     * @param WorldContextObject - The world context from which to retrieve the game mode
     * @return The ATopDownGameMode instance
     */
	UFUNCTION(BlueprintPure)
	static ATopDownGameMode* GetTopDownGameMode(const UObject* WorldContextObject);
};
