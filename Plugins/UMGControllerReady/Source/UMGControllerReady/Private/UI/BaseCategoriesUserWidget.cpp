
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseCategoriesUserWidget.h"
#include "UI/BaseButton.h"
#include "UCRBlueprintFunctionLibrary.h"

void UBaseCategoriesUserWidget::OpenCurrentCategory()
{
	CategoriesContentWidgetSwitcher->SetActiveWidgetIndex(CurrentCategoryIndex);
	GiveFocusToCurrentCategoryContentButton();
}

void UBaseCategoriesUserWidget::NextCategory()
{
	AddAndGoToCategory(1);
}

void UBaseCategoriesUserWidget::PreviousCategory()
{
	AddAndGoToCategory(-1);
}

void UBaseCategoriesUserWidget::SetActiveCategory(int newCategoryIndex)
{
	CurrentCategoryIndex = newCategoryIndex;

	if (newCategoryIndex >= Categories.Num()) {
		CurrentCategoryIndex = 0;
	}
	else if (newCategoryIndex < 0) {
		CurrentCategoryIndex = Categories.Num() - 1;
	}

	SetSelectedCategory();
}

void UBaseCategoriesUserWidget::SetSelectedCategory()
{
	for (int i = 0; i < Categories.Num(); ++i) {
		Categories[i]->SetForceSelected(i == CurrentCategoryIndex);
	}
}

void UBaseCategoriesUserWidget::OpenCategoryIndex(int CategoryIndexToOpen)
{
	SetActiveCategory(CategoryIndexToOpen);
	OpenCurrentCategory();
}

void UBaseCategoriesUserWidget::SetCategoriesAndContentSelector(TArray<UBaseButton*> WidgetCategories, UWidgetSwitcher* WidgetSwitcher)
{
	Categories = WidgetCategories;
	CategoriesContentWidgetSwitcher = WidgetSwitcher;
}

void UBaseCategoriesUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (UBaseButton* button : Categories) {
		if (button->GetMainButton()) {
			button->GetMainButton()->OnHovered.AddUniqueDynamic(this, &UBaseCategoriesUserWidget::OnButtonHovered);
		}
	}
}

void UBaseCategoriesUserWidget::AddAndGoToCategory(int ToAdd)
{
	SetActiveCategory(CurrentCategoryIndex + ToAdd);
	OpenCurrentCategory();
}

void UBaseCategoriesUserWidget::CheckIfCategoriesAndContentMatch()
{
	ensureMsgf(Categories.Num() == CategoriesContentWidgetSwitcher->GetChildrenCount(), TEXT("%s categories and content children count doesn't match"), *GetName());
}

void UBaseCategoriesUserWidget::GiveFocusToCurrentCategoryContentButton()
{
	UUserWidget* CurrentActiveWidgetUserWidget = Cast<UUserWidget>(CategoriesContentWidgetSwitcher->GetActiveWidget());
	UBaseButton* FirstWidgetButton = UUCRBlueprintFunctionLibrary::GetFirstWidgetButton(CurrentActiveWidgetUserWidget ? CurrentActiveWidgetUserWidget->GetRootWidget() : CategoriesContentWidgetSwitcher->GetActiveWidget());
	if (FirstWidgetButton) {
		FirstWidgetButton->SetKeyboardFocus();
	}
}

void UBaseCategoriesUserWidget::OnButtonHovered()
{
	for (int i = 0; i < Categories.Num(); ++i) {
		if (Categories[i]->GetMainButton()) {
			Categories[i]->SetForceSelected(Categories[i]->GetMainButton()->IsHovered() || i == CurrentCategoryIndex);
		}
	}
}

void UBaseCategoriesUserWidget::HandleInputs(bool bListen)
{
	Super::HandleInputs(bListen);

	if (bListen)
	{
		FScriptDelegate NextCategoryDelegate;
		NextCategoryDelegate.BindUFunction(this, TEXT("NextCategory"));
		FOnInputAction NextCategoryInputAction(NextCategoryDelegate);
		AddListenInputAction("Next", EInputEvent::IE_Pressed, true, NextCategoryInputAction, true);

		FScriptDelegate PreviousCategoryDelegate;
		PreviousCategoryDelegate.BindUFunction(this, TEXT("PreviousCategory"));
		FOnInputAction PreviousCategoryInputAction(PreviousCategoryDelegate);
		AddListenInputAction("Previous", EInputEvent::IE_Pressed, true, PreviousCategoryInputAction, true);
	}
	else
	{
		StopListeningForInputAction("Next", EInputEvent::IE_Pressed);
		StopListeningForInputAction("Previous", EInputEvent::IE_Pressed);
	}
}

void UBaseCategoriesUserWidget::AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder)
{
	Super::AddToScreen(LocalPlayer, ZOrder);
	OpenCurrentCategory();
}

void UBaseCategoriesUserWidget::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	OpenCurrentCategory();
}
