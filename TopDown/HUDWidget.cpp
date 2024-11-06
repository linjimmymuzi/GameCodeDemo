// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "HUDWidget.h"
#include "TopDownBPLibrary.h"
#include "TopDownCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/CharacterMovementComponent.h"

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ATopDownCharacter* Char = UTopDownBPLibrary::GetTopDownCharacter(this, 0))
	{
		FString HealthStr = FString::FromInt(Char->GetCurrentHealth());
		HealthAmount->SetText(FText::FromString(HealthStr));

		float CharHealth = Char->GetHealthPct();
		HealthBar->SetPercent(CharHealth);
		if (CharHealth > 0.7f)
		{
			HealthBar->SetFillColorAndOpacity(DefaultHealthColor);
		}
		else if (CharHealth > 0.3f)
		{
			HealthBar->SetFillColorAndOpacity(DangerHealthColor);
		}
		else
		{
			HealthBar->SetFillColorAndOpacity(CriticalHealthColor);
		}

		int NewThreatLevel = 0;
		//Check Movement
		if (UCharacterMovementComponent* MovementComponent = Char->GetCharacterMovement())
		{
			
			bool bIsMoving = MovementComponent->Velocity.Size() > 0.0f;
			float CurrentTime = GetWorld()->GetTimeSeconds(); 

			if (bIsMoving)
			{
				LastMovementTime = CurrentTime;
			}
			
			if (bIsMoving || (CurrentTime - LastMovementTime <= 0.5f))
			{
				NewThreatLevel++;
			}
		}

		//Check Crouching
		if (Char->bIsCrouched)
		{
			NewThreatLevel--;
		}

		//Check Prone
		if (Char->GetIsProne())
		{
			NewThreatLevel -= 2;
		}
		
		//Check Trigger
		int CurrentStealthCount = Char->GetStealthCount();
		if (CurrentStealthCount >= 2)
		{
			NewThreatLevel -= 2;
		}
		else if (CurrentStealthCount == 1)
		{
			NewThreatLevel -= 1;
		}

		NewThreatLevel = FMath::Clamp(NewThreatLevel, static_cast<int>(EThreatLevel::Green), static_cast<int>(EThreatLevel::Purple));
		UpdateThreatLevel(NewThreatLevel);
		Char->SetThreatLevel(ThreatLevel);
		
		//Switch Threat Indicator Color
		UpdateThreatLevelColor();
	}
}

void UHUDWidget::UpdateThreatLevel(int NewThreatLevel)
{
	switch (NewThreatLevel)
	{
	case 1:
		ThreatLevel = EThreatLevel::Purple;
		break;
	case 0:
		ThreatLevel = EThreatLevel::Red;
		break;
	case -1:
		ThreatLevel = EThreatLevel::Yellow;
		break;
	case -2:
		ThreatLevel = EThreatLevel::Blue;
		break;
	case -3:
		ThreatLevel = EThreatLevel::Green;
		break;
	default:
		ThreatLevel = EThreatLevel::Red; 
		break;
	}
}

void UHUDWidget::UpdateThreatLevelColor()
{
	switch(ThreatLevel)
	{
	case EThreatLevel::Red:
		ThreatLevelIndicator->SetColorAndOpacity(FColor::Red);
		break;
	case EThreatLevel::Purple:
		ThreatLevelIndicator->SetColorAndOpacity(FColor::Purple);
		break;
	case EThreatLevel::Yellow:
		ThreatLevelIndicator->SetColorAndOpacity(FColor::Yellow);
		break;
	case EThreatLevel::Blue:
		ThreatLevelIndicator->SetColorAndOpacity(FColor::Blue);
		break;
	case EThreatLevel::Green:
		ThreatLevelIndicator->SetColorAndOpacity(FColor::Green);
		break;
	default:
		ThreatLevelIndicator->SetColorAndOpacity(FColor::Red);
		break;
	}
}
