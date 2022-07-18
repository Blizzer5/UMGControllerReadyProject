// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InputHelpersWidget.h"
#include "UI/InputHelperWidget.h"

void UInputHelpersWidget::UpdateInputHelpers_Implementation()
{
    for (UInputHelperWidget* InputHelper : InputHelpers)
    {
        InputHelper->UpdateInputHelper();
    }
}

UInputHelperWidget* UInputHelpersWidget::GetHelperFromInputActionName(FName InputActionName)
{
    UInputHelperWidget** InputHelper = InputHelpers.FindByPredicate([&](UInputHelperWidget* InputHelper)
    {
        return InputHelper->GetInputActionName() == InputActionName;
    });

    return InputHelper ? *InputHelper : nullptr;
}
