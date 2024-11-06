// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "PuzzleComponent.h"
#include "DrawDebugHelpers.h"
#include "Util/ColorConstants.h"

// Sets default values for this component's properties
UPuzzleComponent::UPuzzleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UPuzzleComponent::SetOutputActive(bool bNewOutputActive)
{
	if (bNewOutputActive == bIsOutputActive)
	{
		return;
	}
	
	bIsOutputActive = bNewOutputActive;
	if (AActor* OutActor = OutputActor.Get())
	{
		if (UPuzzleComponent* PuzzleComp = OutActor->FindComponentByClass<UPuzzleComponent>())
		{
			PuzzleComp->NativeReceiveInput(GetOwner(), bIsOutputActive);
		}
	}
}

void UPuzzleComponent::NativeReceiveInput(TObjectPtr<AActor> Sender, bool bIncomingValue)
{
	OnReceiveInput.Broadcast(Sender, bIncomingValue);
}

void UPuzzleComponent::PuzzleReset()
{
	SetOutputActive(false);
	OnPuzzleReset.Broadcast();
}

void UPuzzleComponent::DebugDrawOutput()
{
#if ENABLE_DRAW_DEBUG
	if (IsRegistered())
	{
		DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetName(), nullptr, FColor::White, 0.1f);
	}
	if (AActor* OutActor = OutputActor.Get())
	{
		const FColor DrawColor = bIsOutputActive ? FColor::Green : FColor::Black;
		DrawDebugDirectionalArrow(GetWorld(), GetOwner()->GetActorLocation(), OutActor->GetActorLocation(), 150.0f, DrawColor);
	}
#endif
}

void UPuzzleComponent::PuzzleTempDisable()
{
	SetOutputActive(false);
	OnPuzzleReset.Broadcast();
}

void UPuzzleComponent::ChangeOutputActor(AActor* NewOutputActor)
{
	OutputActor = NewOutputActor;
	
	if (OutputActor && bIsOutputActive)
	{
		if (AActor* OutActor = OutputActor.Get())
		{
			if (UPuzzleComponent* PuzzleComp = OutActor->FindComponentByClass<UPuzzleComponent>())
			{
				PuzzleComp->NativeReceiveInput(GetOwner(), bIsOutputActive);
			}
		}
	}
}

// Called when the game starts
void UPuzzleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



