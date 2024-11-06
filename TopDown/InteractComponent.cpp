// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "InteractComponent.h"

#include "InteractSubsystem.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInteractComponent::SetInteractGlow(FVector GlowValue, FName ParameterName)
{
	if (UStaticMeshComponent* StaticMeshComp = GetOwner()->FindComponentByClass<UStaticMeshComponent>())
	{
		StaticMeshComp->SetVectorParameterValueOnMaterials(ParameterName, GlowValue);
	}
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UInteractSubsystem* Subsystem = GetWorld()->GetSubsystem<UInteractSubsystem>())
	{
		Subsystem->AddComponent(this);
	}
	
	SetInteractGlow(FVector(0.0f, 0.0f, 0.0f));
	
}

void UInteractComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UInteractSubsystem* Subsystem = GetWorld()->GetSubsystem<UInteractSubsystem>())
	{
		Subsystem->RemoveComponent(this);
	}
}


// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractComponent::NativeInteract()
{
	if (!InteractEnabled)
	{
		return;
	}
	
	OnInteract.Broadcast();
}