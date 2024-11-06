// Copyright 2024 Jimmy Lin


#include "HUDWidget.h"

#include "MultiBPLibrary.h"
#include "MultiGameStateBase.h"
#include "Components/TextBlock.h"
#include "MultiCharacter.h"
#include "MultiPlayerController.h"
#include "GameFramework/GameMode.h"

enum class EMultiTeam : uint8;
class AMultiPlayerState;

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	AMultiGameStateBase* MyGameState = UMultiBPLibrary::GetMultiGameState(this);
	
	if (MyGameState && MyGameState->GetMatchState() == MatchState::WaitingToStart)
	{
		if (Timer->GetVisibility() != ESlateVisibility::HitTestInvisible)
		{
				Timer->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		FString TimerText = FString::Printf(TEXT("%f"), MyGameState->WaitingToStartTime);
		Timer->SetText(FText::FromString(TimerText));
	}
	else if (Timer->GetVisibility() != ESlateVisibility::Hidden)
	{
		Timer->SetVisibility(ESlateVisibility::Hidden);
	}

	if (MyGameState && MyGameState->IsMatchInProgress())
	{
		
		if (StartMatchText->GetVisibility() == ESlateVisibility::HitTestInvisible)
		{
			StartMatchDuration -= InDeltaTime;
			if (StartMatchDuration <= 0.0f)
			{
				StartMatchText->SetVisibility(ESlateVisibility::Hidden);
				StartMatchDuration = 0.0f;
			}
		}
	}
	if(MyGameState)
	{
		if (EndMatchText->GetVisibility() == ESlateVisibility::HitTestInvisible)
        {
        	EndMatchDuration -= InDeltaTime;
        	if (EndMatchDuration <= 0.0f)
        	{
        		EndMatchText->SetVisibility(ESlateVisibility::Hidden);
        		EndMatchDuration = 0.0f;
        	}
        }
	}
	
	
	if (AMultiCharacter* MyCharacter = UMultiBPLibrary::GetMultiCharacter(GetWorld(), 0))
	{
		FString AmmoTextString = FString::Printf(TEXT("%i AMMO"), MyCharacter->GetAmmo());
		AmmoTextBlock->SetText(FText::FromString(AmmoTextString));
	}
	if (MyGameState)
	{
		int RedTeamScore = MyGameState->RedTeamScore;
		int BlueTeamScore = MyGameState->BlueTeamScore;
		
		if (APlayerController* PlayerController = UMultiBPLibrary::GetMultiPlayerController(GetWorld(), 0))
		{
			if (AMultiPlayerState* PlayerState = PlayerController->GetPlayerState<AMultiPlayerState>())
			{
				// Determine the player’s team and individual score
				EMultiTeam PlayerTeam = PlayerState->MultiTeam;
				int PlayerScore = PlayerState->GetScore();

				// Format the score text
				FString RedScoreText = FString::Printf(TEXT("RED %d %s"), RedTeamScore, 
					(PlayerTeam == EMultiTeam::Red) ? *FString::Printf(TEXT("(%d)"), PlayerScore) : TEXT(""));

				FString BlueScoreText = FString::Printf(TEXT("%d %s BLUE"),
					BlueTeamScore, (PlayerTeam == EMultiTeam::Blue) ? *FString::Printf(TEXT("(%d)"), PlayerScore) : TEXT(""));

				// Set the text to the TextBlocks
				RedTeamScoreTextBlock->SetText(FText::FromString(RedScoreText));
				BlueTeamScoreTextBlock->SetText(FText::FromString(BlueScoreText));

				RedTeamScoreTextBlock->SetColorAndOpacity(FLinearColor::Red);
				BlueTeamScoreTextBlock->SetColorAndOpacity(FLinearColor::Blue);
			}
		}
	}
}

void UHUDWidget::SetStartMatchText(const FString& StartText, FLinearColor Color, float Duration)
{
	StartMatchDuration = Duration;
	StartMatchText->SetVisibility(ESlateVisibility::HitTestInvisible);
	StartMatchText->SetText(FText::FromString(StartText));
	StartMatchText->SetColorAndOpacity(Color);
}

void UHUDWidget::SetEndMatchText(const FString& EndText, FLinearColor Color, float Duration)
{
	EndMatchDuration = Duration;
	EndMatchText->SetVisibility(ESlateVisibility::HitTestInvisible);
	EndMatchText->SetText(FText::FromString(EndText));
	EndMatchText->SetColorAndOpacity(Color);
}


