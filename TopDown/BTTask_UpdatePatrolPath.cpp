// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "BTTask_UpdatePatrolPath.h"

#include "TopDownAICharacter.h"
#include "TopDownAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"

UBTTask_UpdatePatrolPath::UBTTask_UpdatePatrolPath()
{
	NodeName = "UpdatePatrolPath";
}

EBTNodeResult::Type UBTTask_UpdatePatrolPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIOwner = OwnerComp.GetAIOwner();
	ATopDownAICharacter* AICharacter = Cast<ATopDownAICharacter>(AIOwner->GetCharacter());
	if (UBlackboardComponent* BBComponent = AIOwner->GetBlackboardComponent())
	{
		TArray<TSoftObjectPtr<AActor>> PPath = AICharacter->GetPatrolPath();
		
		if (PPath.Num() == 0)
		{
			return EBTNodeResult::Failed;
		}
		
		int NextIndex = (BBComponent->GetValueAsInt("PatrolIndex") + 1)%(PPath.Num());
		BBComponent->SetValueAsInt("PatrolIndex", NextIndex);
		AActor* NextPatrolActor = PPath[NextIndex].Get();
		BBComponent->SetValueAsObject("PatrolActor", NextPatrolActor);
	}
	return EBTNodeResult::Succeeded;
}
