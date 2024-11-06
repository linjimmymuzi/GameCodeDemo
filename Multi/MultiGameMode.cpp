// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiGameMode.h"
#include "MultiCharacter.h"
#include "MultiGameStateBase.h"
#include "UObject/ConstructorHelpers.h"
#include "MultiPlayerState.h"
#include "Engine/PlayerStartPIE.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "MultiBPLibrary.h"
#include "MultiPlayerController.h"
#include "Kismet/GameplayStatics.h"

AMultiGameMode::AMultiGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	PlayerStateClass = AMultiPlayerState::StaticClass();

	GameStateClass = AMultiGameStateBase::StaticClass();

	bDelayedStart = true;

	PrimaryActorTick.bCanEverTick = true;
}

void AMultiGameMode::GenericPlayerInitialization(AController* C)
{
	Super::GenericPlayerInitialization(C);

	AMultiPlayerState* PlayerState = C->GetPlayerState<AMultiPlayerState>();
	if (PlayerState && PlayerState->MultiTeam == EMultiTeam::None)
	{
		if (RedTeamCount <= BlueTeamCount)
		{
			PlayerState->MultiTeam = EMultiTeam::Red;
			RedTeamCount++;
		}
		else
		{
			PlayerState->MultiTeam = EMultiTeam::Blue;
			BlueTeamCount++;
		}
	}
}

AActor* AMultiGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// Choose a player start
	APlayerStart* FoundPlayerStart = nullptr;
	UClass* PawnClass = GetDefaultPawnClassForController(Player);
	APawn* PawnToFit = PawnClass ? PawnClass->GetDefaultObject<APawn>() : nullptr;
	TArray<APlayerStart*> UnOccupiedStartPoints;
	TArray<APlayerStart*> OccupiedStartPoints;
	UWorld* World = GetWorld();

	EMultiTeam Team = Player->GetPlayerState<AMultiPlayerState>()->GetMultiTeam();
	FName PlayerStartTag = NAME_None;

	switch(Team)
	{
		case EMultiTeam::Red:
			PlayerStartTag = "Red";
			break;
		case EMultiTeam::Blue:
			PlayerStartTag = "Blue";
			break;
		default:
			break;
	}
	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		APlayerStart* PlayerStart = *It;

		if (PlayerStart->IsA<APlayerStartPIE>())
		{
			// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			FoundPlayerStart = PlayerStart;
			break;
		}
		else if (PlayerStartTag == NAME_None || PlayerStartTag == PlayerStart->PlayerStartTag)
		{
			FVector ActorLocation = PlayerStart->GetActorLocation();
			const FRotator ActorRotation = PlayerStart->GetActorRotation();
			if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
			{
				UnOccupiedStartPoints.Add(PlayerStart);
			}
			else if (World->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
			{
				OccupiedStartPoints.Add(PlayerStart);
			}
		}
	}
	if (FoundPlayerStart == nullptr)
	{
		if (UnOccupiedStartPoints.Num() > 0)
		{
			FoundPlayerStart = UnOccupiedStartPoints[FMath::RandRange(0, UnOccupiedStartPoints.Num() - 1)];
		}
		else if (OccupiedStartPoints.Num() > 0)
		{
			FoundPlayerStart = OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
		}
	}
	return FoundPlayerStart;
	
}

bool AMultiGameMode::ReadyToEndMatch_Implementation()
{
	AMultiGameStateBase* MyGameState = UMultiBPLibrary::GetMultiGameState(this);
	if (MyGameState->RedTeamScore >= 10 || MyGameState->BlueTeamScore >= 10)
	{
		return true;
	}
	return false;
}

void AMultiGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	
	AMultiGameStateBase* MyGameState = UMultiBPLibrary::GetMultiGameState(this);

	FString WinTeam;
	FLinearColor TeamColor;

	if (MyGameState->RedTeamScore >= 10)
	{
		WinTeam = "RED TEAM WINS";
		TeamColor = FLinearColor::Red;
	}
	else if (MyGameState->BlueTeamScore >= 10)
	{
		WinTeam = "BLUE TEAM WINS";
		TeamColor = FLinearColor::Blue;
	}
	
	if (GetLocalRole() == ROLE_Authority)
	{
		MyGameState->ShowEndMatch(WinTeam, TeamColor, 5.0f);
	}

	GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AMultiGameMode::RestartGame, 5.0f, false);
}

