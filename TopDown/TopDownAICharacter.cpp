// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "TopDownAICharacter.h"
#include "TopDownAIController.h"

ATopDownAICharacter::ATopDownAICharacter()
{
	AIControllerClass = ATopDownAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
