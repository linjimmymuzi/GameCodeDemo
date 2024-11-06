// Copyright 2024 Han Lin - linjimmy@usc.edu


#include "TopDownMenu.h"
#include "Blueprint/WidgetTree.h"
#include "TopDownButton.h"

UTopDownMenu::UTopDownMenu(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	SetIsFocusable(true);
}

void UTopDownMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// Clear in case there already are buttons
	Buttons.Empty();
	// Use the widget tree to iterate over all the children and find buttons
	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
	if (auto Button = Cast<UTopDownButton>(Widget))
	{
	Buttons.Add(Button);
	}
	});
	// Select index 0
	SetSelectedIndex(0);
}

FReply UTopDownMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// See which key it is
	FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::Up || Key == EKeys::W)
	{
		SetSelectedIndex(SelectedIndex - 1);
	}
	else if (Key == EKeys::Down || Key == EKeys::S)
	{
		SetSelectedIndex(SelectedIndex + 1);
	}
	else if (Key == EKeys::Enter && Buttons.IsValidIndex(SelectedIndex))
	{
		Buttons[SelectedIndex].Get()->NativeActivated();
	}
	// Bypass the super and just always say we handled it
	return FReply::Handled();
}

void UTopDownMenu::SetSelectedIndex(int InSelectedIndex)
{
	// Tell the old button it's not selected
	if (Buttons.IsValidIndex(SelectedIndex))
	{
		Buttons[SelectedIndex].Get()->NativeSetSelection(false);
	}

	// Handle wrapping around selected index to new value
	SelectedIndex = FMath::Wrap(InSelectedIndex, 0, Buttons.Num() - 1);

	// Tell the new button it's selected
	if (Buttons.IsValidIndex(SelectedIndex))
	{
		Buttons[SelectedIndex].Get()->NativeSetSelection(true);
	}
}

FReply UTopDownMenu::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
	for(int Index = 0; Index < Buttons.Num(); Index++)
	{
		FGeometry ButtonPosition = Buttons[Index]->GetCachedGeometry();
		if (ButtonPosition.IsUnderLocation(MousePosition) && Index != LastSelectedIndex)
		{
			Buttons[LastSelectedIndex].Get()->NativeSetSelection(false);
			LastSelectedIndex = Index;
			Buttons[Index].Get()->NativeSetSelection(true);
		}
	}
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UTopDownMenu::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
	for(int Index = 0; Index < Buttons.Num(); Index++)
	{
		FGeometry ButtonPosition = Buttons[Index]->GetCachedGeometry();
		if (ButtonPosition.IsUnderLocation(MousePosition))
		{
			Buttons[Index].Get()->NativeActivated();
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
