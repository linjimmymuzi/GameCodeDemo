// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "AnimNotifyState_Invulnerable.h"

#include "TopDownCharacter.h"

UAnimNotifyState_Invulnerable::UAnimNotifyState_Invulnerable()
{
}

void UAnimNotifyState_Invulnerable::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	
	if (Owner != nullptr && Cast<ATopDownCharacter>(Owner))
	{
		ATopDownCharacter* Character = Cast<ATopDownCharacter>(Owner);
		Character->AddRequestors(this);
		Character->OnEnterBlueTrigger();
	}
}

void UAnimNotifyState_Invulnerable::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	
	if (Owner != nullptr && Cast<ATopDownCharacter>(Owner))
	{
		ATopDownCharacter* character = Cast<ATopDownCharacter>(Owner);
		character->RemoveRequestors(this);
		character->OnExitBlueTrigger();
	}
}
