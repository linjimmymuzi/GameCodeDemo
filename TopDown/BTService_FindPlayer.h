// Copyright 2024 Han Lin - linjimmy@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindPlayer.generated.h"

enum class EThreatLevel;
/**
 * 
 */
UCLASS()
class TOPDOWN_API UBTService_FindPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_FindPlayer();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	FName GetSelectedPlayerKey() const {return PlayerKey.SelectedKeyName;}
private:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector PlayerKey;
	
	TMap<EThreatLevel, FVector2D> ThreatLevelSettings;
};
