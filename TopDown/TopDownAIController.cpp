// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "TopDownAIController.h"
#include "TopDownAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void ATopDownAIController::OnPossess(APawn* InPawn)
{
	OwnerCharacter = Cast<ATopDownAICharacter>(InPawn);

	bool bInitializedBB = false;
	if (OwnerCharacter && OwnerCharacter->BlackboardAsset && OwnerCharacter->BTAsset)
	{
		UBlackboardComponent* BBComponent = nullptr;
		if (UseBlackboard(OwnerCharacter->BlackboardAsset, BBComponent))
		{
			BBComponent->SetValueAsInt("PatrolIndex", -1);
			RunBehaviorTree(OwnerCharacter->BTAsset);
			bInitializedBB = true;
		}
	}

	if (!bInitializedBB)
	{
		UE_LOG(LogTemp,Warning, TEXT("ATopDownAIController Can't Initialize Behavior Tree for %s"), *InPawn->GetName())
	}
	Super::OnPossess(InPawn);
}
