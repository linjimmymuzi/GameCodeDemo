// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "InteractSubsystem.h"
#include "InteractComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UInteractSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UInteractSubsystem::Deinitialize()
{
	Super::Deinitialize();
	InteractComponents.Empty();
}

void UInteractSubsystem::AddComponent(UInteractComponent* Component)
{
	InteractComponents.Add(Component);
}

void UInteractSubsystem::RemoveComponent(UInteractComponent* Component)
{
	InteractComponents.Remove(Component);
}

TStatId UInteractSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UInteractSubsystem, STATGROUP_Tickables);
}

void UInteractSubsystem::PerformInteract()
{
	if (UInteractComponent* Component = BestCandidate.Get())
	{
		Component->NativeInteract();
	}
}

void UInteractSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BestCandidate = nullptr;

	if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		FVector PlayerPos = Character->GetActorLocation();
		FVector PlayerForward = Character->GetActorForwardVector();
		float BestAngleRadians = PI/3.0f;
		float BestDistance = TNumericLimits<float>::Max();

		for (const auto& WeakPtr : InteractComponents)
		{
			if (UInteractComponent* Component = WeakPtr.Get())
			{
				Component->SetInteractGlow(FVector(0.0f, 0.0f, 0.0f));
			}
		}
		
		for (const auto& WeakPtr : InteractComponents)
		{
			UInteractComponent* Component = WeakPtr.Get();
			if (Component != nullptr && Component->InteractEnabled)
			{
				FVector CandidateLocation = Component->GetOwner()->GetActorLocation();
				float Distance = FVector::Distance(PlayerPos, CandidateLocation);
				
				if (Distance < 200.0f)
				{
					FVector ToCandidate = CandidateLocation - PlayerPos;
					ToCandidate.Normalize();
					float AngleRadians = FMath::Acos(FVector::DotProduct(PlayerForward, ToCandidate) / (PlayerForward.Size() * ToCandidate.Size()));
					if (AngleRadians < BestAngleRadians)
					{
						BestAngleRadians = AngleRadians;
                        Component->SetInteractGlow(FVector(1.0f, 1.0f, 1.0f));
                        if (BestCandidate != nullptr)
                        {
                        	BestCandidate->SetInteractGlow(FVector(0.0f, 0.0f, 0.0f));
                        }
                        BestCandidate = Component;
					}
				}
			}
		}
	}
}

bool UInteractSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}
