// Copyright 2024 Jimmy Lin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MultiPlayerState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMultiTeam : uint8
{
	/* Team not assigned yet*/
	None,
	/* On red team */
	Red,
	/* On blue team */
	Blue
};

UCLASS()
class MULTI_API AMultiPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMultiPlayerState();
	
	UPROPERTY(BlueprintReadWrite, Replicated)
	EMultiTeam MultiTeam = EMultiTeam::None;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	EMultiTeam GetMultiTeam() const {return MultiTeam;}
};
