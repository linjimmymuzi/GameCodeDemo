// Copyright 2024 Jimmy Lin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPickup.generated.h"

UCLASS()
class MULTI_API AAmmoPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	class USphereComponent* SphereComponent;
	
	UPROPERTY(BlueprintReadWrite)
	class UTextRenderComponent* TextRenderComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoAmount = 5;

	UFUNCTION()
	void HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
