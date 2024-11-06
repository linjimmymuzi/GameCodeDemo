// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "BTService_FindPlayer.h"

#include "AIController.h"
#include "HUDWidget.h"
#include "TopDownAICharacter.h"
#include "TopDownCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TopDownBPLibrary.h"

UBTService_FindPlayer::UBTService_FindPlayer()
{
	NodeName = "FindPlayer";

	PlayerKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_FindPlayer, PlayerKey), AActor::StaticClass());
	
	ThreatLevelSettings.Add(EThreatLevel::Green, FVector2D(100.0f, 0.52f));
    ThreatLevelSettings.Add(EThreatLevel::Blue, FVector2D(250.0f, 0.70f));
    ThreatLevelSettings.Add(EThreatLevel::Yellow, FVector2D(500.0f, 0.87f));
    ThreatLevelSettings.Add(EThreatLevel::Red, FVector2D(2000.0f, 1.22f));
    ThreatLevelSettings.Add(EThreatLevel::Purple, FVector2D(4000.0f, 1.57f));
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (UBlackboardComponent* MyBlackBoard = OwnerComp.GetBlackboardComponent())
	{
		ATopDownCharacter* PlayerChar = UTopDownBPLibrary::GetTopDownCharacter(&OwnerComp,0);
		if (PlayerChar && !PlayerChar->IsDead())
		{
			AAIController* AIOwner = OwnerComp.GetAIOwner();
			if (ATopDownAICharacter* AIChar = Cast<ATopDownAICharacter>(AIOwner->GetCharacter()))
			{
				FVector AILocation = AIChar->GetActorLocation();
				FVector PlayerLocation = PlayerChar->GetActorLocation();
				
				float Distance = FVector::Dist(AILocation, PlayerLocation);
				
				FVector DirectionToPlayer = (PlayerLocation - AILocation).GetSafeNormal();
				FVector AIFacingDirection = AIChar->GetActorForwardVector();
				
				float Angle = FMath::Abs(FMath::Acos(FVector::DotProduct(DirectionToPlayer, AIFacingDirection)));

				EThreatLevel PlayerThreatLevel = PlayerChar->GetThreatLevel();
				FVector2D* ThreatDetectRange = ThreatLevelSettings.Find(PlayerThreatLevel);
				
				float MaxDistance = 0.0f;
				float MaxAngle = 0.0f;

				if (ThreatDetectRange && Distance <= ThreatDetectRange->X && Angle <= ThreatDetectRange->Y)
				{
					FHitResult Hit;
					GetWorld()->LineTraceSingleByChannel(Hit, AILocation, PlayerLocation, ECC_Camera);
					if (Hit.GetActor() == PlayerChar)
					{
						MyBlackBoard->SetValueAsObject(GetSelectedPlayerKey(), PlayerChar);
						return;
					}
				}
			}
		}
		
		MyBlackBoard->SetValueAsObject(GetSelectedPlayerKey(), nullptr);
		
	}
}

void UBTService_FindPlayer::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		PlayerKey.ResolveSelectedKey(*BBAsset);
	}
}
