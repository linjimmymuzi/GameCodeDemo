// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "StealthBoxComponent.h"

#include "TopDownCharacter.h"

UStealthBoxComponent::UStealthBoxComponent()
{
	OnComponentBeginOverlap.AddDynamic(this, &UStealthBoxComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UStealthBoxComponent::OnEndOverlap);
}

void UStealthBoxComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStealthBoxComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ATopDownCharacter* Character = Cast<ATopDownCharacter>(OtherActor))
	{
		Character->IncreaseStealthCount();
	}
}

void UStealthBoxComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ATopDownCharacter* Character = Cast<ATopDownCharacter>(OtherActor))
	{
		Character->DecreaseStealthCount();
	}
}
