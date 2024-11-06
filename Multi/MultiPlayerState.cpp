// Copyright 2024 Jimmy Lin


#include "MultiPlayerState.h"

#include "Net/UnrealNetwork.h"

AMultiPlayerState::AMultiPlayerState()
{
	NetUpdateFrequency = 4.0f;
}

void AMultiPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiPlayerState, MultiTeam);
}
