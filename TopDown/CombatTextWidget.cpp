// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "CombatTextWidget.h"

void UCombatTextWidget::ShowDamage(int Damage)
{
	FString DamageText = FString::FromInt(FMath::Abs(Damage));
	DamageTextBlock->SetColorAndOpacity(Damage < 0 ? FLinearColor::Green : FLinearColor::Red);
	DamageTextBlock->SetText(FText::FromString(DamageText));
	DamageTextBlock->SetRenderOpacity(1.0f);
	PlayAnimation(TextFly);
}
