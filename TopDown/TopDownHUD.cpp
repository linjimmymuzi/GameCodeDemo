// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "TopDownHUD.h"
#include "DrawDebugHelpers.h"
#include "PuzzleComponent.h"
#include "InteractSubsystem.h"
#include "InteractComponent.h"
#include "TopDownBPLibrary.h"
#include "TopDownCharacter.h"

static TAutoConsoleVariable<int32> CVarDebugPuzzles(
	TEXT("DebugPuzzles"), // Console command
	0, // Default value
	TEXT("Turns puzzle debug view on/off (off by default)"), // Help text
	ECVF_Default); // Flags

void ATopDownHUD::DrawHUD()
{
	Super::DrawHUD();
	
	float XCord = 20.0f;
	float YCord = 0.0f;

	FString WaterMark = TEXT("Han Lin - linjimmy@usc.edu");
	FLinearColor MarkColor = FLinearColor::Blue;
	DrawText(WaterMark, MarkColor, XCord, YCord, GEngine->GetSubtitleFont(), 1.0f);
	
	if (ATopDownCharacter* Charactor = UTopDownBPLibrary::GetTopDownCharacter(this, 0))
	{
		DrawText(Charactor->GetName(), FLinearColor::Green, 20, 70, nullptr, 1.5f);

		// Num of stealth box
		int StealthCount = Charactor->GetStealthCount();
		FString StealthText = FString::Printf(TEXT("Stealth Boxes: %i"), StealthCount);
		DrawText(StealthText, FLinearColor::Red, 20, 800, nullptr, 4.0f); 
	}
	if (ATopDownPlayerController* Controller = UTopDownBPLibrary::GetTopDownController(this, 0))
	{
		DrawText(Controller->GetName(), FLinearColor::Green, 20, 85, nullptr, 1.5f);
	}
	if (ATopDownGameMode* GameMode = UTopDownBPLibrary::GetTopDownGameMode(this))
	{
		DrawText(GameMode->GetName(), FLinearColor::Green, 20, 100, nullptr, 1.5f);
	}
	
	if (UInteractSubsystem* InteractSubsystem = GetWorld()->GetSubsystem<UInteractSubsystem>())
	{
		TWeakObjectPtr<class UInteractComponent> BestCandidate = InteractSubsystem->GetBestCandidate();
		if (BestCandidate != nullptr)
		{
			FVector OwnerWorldLocation = BestCandidate->GetOwner()->GetActorLocation();
			FVector ProjectPosition = Project(OwnerWorldLocation);
			
			FVector2D ScreenPosition;
			ScreenPosition.X = ProjectPosition.X;
			ScreenPosition.Y = ProjectPosition.Y;
			
			float TextWidth, TextHeight;
			GetTextSize(TEXT("[E] - Interact"), TextWidth, TextHeight, nullptr, 3.0f);
			DrawText(TEXT("[E] - Interact"), FLinearColor::Red, ScreenPosition.X - TextWidth / 2, ScreenPosition.Y - TextHeight / 2, nullptr, 3.0f);
		}
	}

#if ENABLE_DRAW_DEBUG
	if (CVarDebugPuzzles.GetValueOnGameThread() != 0)
	{
		for (TObjectIterator<UPuzzleComponent> Itr; Itr; ++Itr)
        	{
        		Itr->DebugDrawOutput();
        	}
	}
#endif
}
