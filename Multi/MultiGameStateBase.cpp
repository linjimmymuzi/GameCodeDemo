// Copyright 2024 Jimmy Lin


#include "MultiGameStateBase.h"

#include "MultiBPLibrary.h"
#include "MultiGameMode.h"
#include "MultiPlayerController.h"
#include "MultiPlayerState.h"
#include "Net/UnrealNetwork.h"

AMultiGameStateBase::AMultiGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMultiGameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetMatchState() == MatchState::WaitingToStart)
	{
		WaitingToStartTime -= DeltaSeconds;
		if (WaitingToStartTime <= 0.0f)
		{
			WaitingToStartTime = 0.0f;
			if (GetLocalRole() == ROLE_Authority)
			{
				if (AMultiGameMode* GameMode = UMultiBPLibrary::GetMultiGameMode(this))
				{
					GameMode->StartMatch();
					ShowStartMatch("STARTING MATCH", FLinearColor::Green, 3.0f);
				}
			}
		}
	}
}

void AMultiGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiGameStateBase, RedTeamScore);
	DOREPLIFETIME(AMultiGameStateBase, BlueTeamScore);
	DOREPLIFETIME_CONDITION(AMultiGameStateBase, WaitingToStartTime, COND_InitialOnly);
}

void AMultiGameStateBase::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();

	if (GetLocalRole() == ROLE_Authority)
	{
		WaitingToStartTime = GetDefaultGameMode<AMultiGameMode>()->WaitingToStartDuration;
	}
}

void AMultiGameStateBase::ShowEndMatch_Implementation(const FString& WinningTeam, FLinearColor TeamColor, float Duration)
{
	if (AMultiPlayerController* PlayerController = UMultiBPLibrary::GetMultiPlayerController(this, 0))
	{
		if (PlayerController->IsLocalController())
		{
			PlayerController->SetEndMatchText(WinningTeam, TeamColor, Duration);
		}
	}
}

void AMultiGameStateBase::ShowStartMatch_Implementation(const FString& StartText, FLinearColor Color, float Duration)
{
	AMultiPlayerController* PlayerController = UMultiBPLibrary::GetMultiPlayerController(this, 0);
	if (PlayerController->IsLocalController())
	{
		PlayerController->SetStartMatchText(StartText, Color, Duration);
	}
}
