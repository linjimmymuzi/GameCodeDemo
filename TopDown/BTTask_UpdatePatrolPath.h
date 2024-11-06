// Copyright 2024 Han Lin - linjimmy@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UpdatePatrolPath.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API UBTTask_UpdatePatrolPath : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_UpdatePatrolPath();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
