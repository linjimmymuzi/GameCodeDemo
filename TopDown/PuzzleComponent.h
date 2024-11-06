// Copyright 2024 Han Lin - linjimmy@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PuzzleComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPuzzleComponentOnReceiveInput, class AActor*, InputActor, bool, bValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPuzzleComponentOnPuzzleReset);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOPDOWN_API UPuzzleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPuzzleComponent();

	/**
	* Set activeness of the new output
	* @param bNewOutputActive - If the new output is active
	*/
	UFUNCTION(BlueprintCallable)
	virtual void  SetOutputActive(bool bNewOutputActive);

	// receive input
	virtual void NativeReceiveInput(TObjectPtr<AActor> Sender, bool NewValue);
    
    /**
     * Reset the puzzle component
     */
	UFUNCTION(BlueprintCallable)
	virtual void PuzzleReset();

	UPROPERTY (Transient, BlueprintReadOnly)
	bool bIsOutputActive = false;

	UPROPERTY (EditInstanceOnly)
	TSoftObjectPtr<AActor> OutputActor;

	UPROPERTY(BlueprintAssignable)
	FPuzzleComponentOnReceiveInput OnReceiveInput;

	UPROPERTY(BlueprintAssignable)
	FPuzzleComponentOnPuzzleReset OnPuzzleReset;

	void DebugDrawOutput();
    
    /**
     * Temporarily disable the puzzle component's output
     */
	UFUNCTION(BlueprintCallable)
	virtual void PuzzleTempDisable();
    
    /**
    * Change the output actor
    * @param NewOutputActor - The new actor to be set as the output
*/
	UFUNCTION(BlueprintCallable)
	void ChangeOutputActor(AActor* NewOutputActor);

protected:
	// Called when the game startss
	virtual void BeginPlay() override;
};
