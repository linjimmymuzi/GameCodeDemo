// Copyright 2024 Jimmy Lin


#include "AmmoPickup.h"

#include "MultiBPLibrary.h"
#include "MultiCharacter.h"
#include "Components/TextRenderComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AAmmoPickup::AAmmoPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetSphereRadius(25.0f);
	RootComponent = SphereComponent;
	
	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetText(FText::FromString("Ammo"));
	TextRenderComponent->SetupAttachment(RootComponent);
	TextRenderComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRenderComponent->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	TextRenderComponent->SetTextRenderColor(FColor::Green);

}

// Called when the game starts or when spawned
void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickup::HandleOnBeginOverlap);
	}
	
}

void AAmmoPickup::HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (AMultiCharacter* MyCharacter = Cast<AMultiCharacter>(OtherActor))
	{
		MyCharacter->AddAmmo(AmmoAmount);
		
		Destroy();
	}
}

// Called every frame
void AAmmoPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

