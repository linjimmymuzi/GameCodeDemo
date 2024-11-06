// Copyright 2024 Han Lin - linjimmy@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_Invulnerable.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API UAnimNotifyState_Invulnerable : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_Invulnerable();
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
