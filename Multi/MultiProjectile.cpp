// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiProjectile.h"

#include "MultiBPLibrary.h"
#include "MultiCharacter.h"
#include "MultiGameMode.h"
#include "MultiGameStateBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"

class AGameMode;

AMultiProjectile::AMultiProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMultiProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	bReplicates = true;
	SetReplicateMovement(true);
}

void AMultiProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
	
	if (AMultiCharacter* HitCharacter = Cast<AMultiCharacter>(OtherActor))
	{
		AMultiGameStateBase* GameState = UMultiBPLibrary::GetMultiGameState(this);
		AMultiCharacter* MyCharacter = Cast<AMultiCharacter>(GetInstigator());
		if (MyCharacter && HitCharacter->MultiTeam != MyCharacter->MultiTeam && GameState->GetMatchState() == MatchState::InProgress)
		{
			if (APlayerState* MyPlayerState = MyCharacter->GetPlayerState())
			{
				MyPlayerState->SetScore(MyPlayerState->GetScore()+1);
			}
			if (AMultiGameStateBase* MyGameState = GetWorld()->GetGameState<AMultiGameStateBase>())
			{
				if (MyCharacter->MultiTeam == EMultiTeam::Red)
				{
					MyGameState->RedTeamScore += 1; 
				}
				else if (MyCharacter->MultiTeam == EMultiTeam::Blue)
				{
					MyGameState->BlueTeamScore += 1; 
				}
			}
		}
	}
}