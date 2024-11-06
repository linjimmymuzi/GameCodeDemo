// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "FireComponent.h"

#include "Kismet/GameplayStatics.h"

UFireComponent::UFireComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bIsOnFire = false;
	bStartOnFire = false;

	FireSound = nullptr;
	FireParticle = nullptr;
}

void UFireComponent::HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UFireComponent* OtherFireComponent = OtherActor->FindComponentByClass<UFireComponent>();
	
	if (OtherFireComponent && !OtherFireComponent->bIsOnFire && bIsOnFire)
	{
		OtherFireComponent->CatchFireNative();
	}
}

void UFireComponent::CatchFireNative()
{
	if (!bIsOnFire)
	{
		bIsOnFire = true;

		if (FireSound)
		{
			UGameplayStatics::SpawnSoundAttached(FireSound, this);
		}
		if (FireParticle)
		{
			UGameplayStatics::SpawnEmitterAttached(FireParticle, this);
		}
		
		OnCatchFire.Broadcast();
	}
}


void UFireComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UFireComponent::HandleOnBeginOverlap);

	if (bStartOnFire)
	{
		CatchFireNative();
	}
}

void UFireComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OnComponentBeginOverlap.RemoveAll(this);
}
