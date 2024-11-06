// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "PowerComponent.h"
#include "GameFramework/Actor.h"

UPowerComponent::UPowerComponent()
{
	bIsPowered = false;
	
	OnComponentBeginOverlap.AddDynamic(this, &UPowerComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UPowerComponent::OnEndOverlap);
}

void UPowerComponent::SetPower(bool IsOnPower)
{
	if (bIsPowered != IsOnPower)
	{
		bIsPowered = IsOnPower;
		OnChangePowerState.Broadcast(bIsPowered);
		
		if (PowerType == EPowerType::Receiver || PowerType == EPowerType::Output)
		{
			PropagatePower();
		}
	}
}

void UPowerComponent::PropagatePower()
{
	if (PowerType == EPowerType::Output)
	{
		TArray<UPrimitiveComponent*> Components;
		GetOverlappingComponents(Components);

		for (UPrimitiveComponent* OverlappingComp : Components)
		{
			if (UPowerComponent* OtherPowerComp = Cast<UPowerComponent>(OverlappingComp))
			{
				if (OtherPowerComp->PowerType == EPowerType::Receiver)
				{
					OtherPowerComp->SetPower(bIsPowered);
				}
			}
		}
	}
	else if (PowerType == EPowerType::Receiver)
	{
		TArray<UPowerComponent*> PowerComponents;
		GetOwner()->GetComponents<UPowerComponent>(PowerComponents);
		for (UPowerComponent* Component : PowerComponents)
		{
			if (UPowerComponent* PowerComp = Cast<UPowerComponent>(Component))
			{
				if (PowerComp->PowerType == EPowerType::Output)
				{
					PowerComp->SetPower(bIsPowered);
				}
			}
		}
	}
}

void UPowerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPowerComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PowerType == EPowerType::Generator)
	{
		if (UPowerComponent* OtherPowerComp = Cast<UPowerComponent>(OtherComp))
		{
			if (OtherPowerComp->PowerType == EPowerType::Receiver)
			{
				OtherPowerComp->SetPower(true);
			}
		}
	}
}

void UPowerComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PowerType == EPowerType::Generator)
	{
		if (UPowerComponent* OtherPowerComp = Cast<UPowerComponent>(OtherComp))
		{
			if (OtherPowerComp->PowerType == EPowerType::Receiver)
			{
				OtherPowerComp->SetPower(false);
			}
		}
	}
}
