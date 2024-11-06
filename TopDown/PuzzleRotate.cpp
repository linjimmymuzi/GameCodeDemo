// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "PuzzleRotate.h"
#include "Components/BillboardComponent.h"
#include "PuzzleComponent.h"

// Sets default values
APuzzleRotate::APuzzleRotate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<UBillboardComponent>("Root");
	PuzzleComponent = CreateDefaultSubobject<UPuzzleComponent>("Puzzle");

	PuzzleComponent->OnReceiveInput.AddDynamic(this, &APuzzleRotate::OnReceiveInput);
}

void APuzzleRotate::OnReceiveInput(class AActor* InputActor, bool bValue)
{
	PuzzleComponent->SetOutputActive(bValue);
}

void APuzzleRotate::OnRotateTimer()
{
	if (InputActors.Num() == 0)
	{
		return;
	}
	
	if (PuzzleComponent)
	{
		PuzzleComponent->PuzzleReset();
	}
	
	for (TSoftObjectPtr<AActor> InputActor : InputActors)
	{
		if (AActor* MyInputActor = InputActor.Get())
		{
			if (UPuzzleComponent* PuzzleComp = MyInputActor->FindComponentByClass<UPuzzleComponent>())
			{
				PuzzleComp->ChangeOutputActor(nullptr);
			}
		}
	}
	
	if (AActor* CurrentActor = InputActors[CurrentIndex].Get())
	{
		if (UPuzzleComponent* PuzzleComp = CurrentActor->FindComponentByClass<UPuzzleComponent>())
		{
			PuzzleComp->ChangeOutputActor(this);
		}
	}
	
	CurrentIndex = (CurrentIndex + 1) % InputActors.Num();
}

// Called when the game starts or when spawned
void APuzzleRotate::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &APuzzleRotate::OnRotateTimer, TimerDuration, true);

	for (TSoftObjectPtr<AActor> InputActor : InputActors)
	{
		if (AActor* MyInputActor = InputActor.Get())
		{
			if (UPuzzleComponent* PuzzleComp = MyInputActor->FindComponentByClass<UPuzzleComponent>())
			{
				PuzzleComp->ChangeOutputActor(nullptr);
			}
		}
	}
}

// Called every frame
void APuzzleRotate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

