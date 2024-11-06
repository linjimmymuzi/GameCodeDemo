// Copyright 2024 Han Lin - linjimmy@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleRotate.generated.h"

UCLASS()
class TOPDOWN_API APuzzleRotate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleRotate();

    /**
     * Called when input is received from another puzzle.
     * @param InputActor - The actor sending the input
     * @param bValue - The value of the input received
     */
	UFUNCTION()
	void OnReceiveInput(class AActor* InputActor, bool bValue);

	void OnRotateTimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Our PuzzleComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPuzzleComponent> PuzzleComponent;

	// Duration of the timer
	UPROPERTY(EditAnywhere)
	float TimerDuration = 1.0f;

	// Handle to manage the timer
	FTimerHandle TimerHandle;

	int CurrentIndex = 0;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// The input actors with puzzle components which we want to rotate between
	UPROPERTY(EditInstanceOnly)
	TArray<TSoftObjectPtr<class AActor>> InputActors;

};
