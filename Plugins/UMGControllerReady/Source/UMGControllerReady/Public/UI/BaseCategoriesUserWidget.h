// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "BaseCategoriesUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UMGCONTROLLERREADY_API UBaseCategoriesUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = "BaseCategoriesUserWidget")
	void OpenCurrentCategory();

	UFUNCTION(BlueprintCallable, Category = "BaseCategoriesUserWidget")
	void NextCategory();

	UFUNCTION(BlueprintCallable, Category = "BaseCategoriesUserWidget")
	void PreviousCategory();

	UFUNCTION(BlueprintCallable, Category = "BaseCategoriesUserWidget")
	void SetActiveCategory(int newCategoryIndex);

	UFUNCTION(BlueprintCallable, Category = "BaseCategoriesUserWidget")
	void SetSelectedCategory();

	UFUNCTION(BlueprintCallable, Category = "BaseCategoriesUserWidget")
	void OpenCategoryIndex(int CategoryIndexToOpen);

	UFUNCTION(BlueprintCallable, Category = "BaseCategoriesUserWidget")
	void SetCategoriesAndContentSelector(TArray<UBaseButton*> WidgetCategories, UWidgetSwitcher* WidgetSwitcher);


	void NativeConstruct() override;
	void HandleInputs(bool bListen) override;


	void AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder) override;

private:
	UFUNCTION(BlueprintCallable, Category = "BaseCategoriesUserWidget")
	void AddAndGoToCategory(int ToAdd);

	UFUNCTION(BlueprintCallable, Category = "BaseCategoriesUserWidget")
	void CheckIfCategoriesAndContentMatch();

	UFUNCTION()
	void GiveFocusToCurrentCategoryContentButton();

	UFUNCTION()
	void OnButtonHovered();
protected:
	int CurrentCategoryIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UBaseButton*> Categories;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetSwitcher* CategoriesContentWidgetSwitcher;

public:
	void SetVisibility(ESlateVisibility InVisibility) override;

};
