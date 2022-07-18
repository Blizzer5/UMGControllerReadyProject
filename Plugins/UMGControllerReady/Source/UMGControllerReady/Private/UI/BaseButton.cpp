// Copyright © Pedro Costa, 2021. All rights reserved

#include "UI/BaseButton.h"
#include "Components/PanelWidget.h"
#include "Blueprint/WidgetTree.h"
#include "../UMGControllerReadyTypes.h"
#include <UCRBlueprintFunctionLibrary.h>
#include "UI/BaseUserWidget.h"

UBaseUserWidget* UBaseButton::GetMainParent()
{
    return ButtonParent;
}

void UBaseButton::SetMainParent(UBaseUserWidget* newMainParent)
{
    if (!ButtonParent)
    {
        ButtonParent = newMainParent;
    }
}

UButton* UBaseButton::GetMainButton()
{
    return BaseButton;
}

bool UBaseButton::IsSelected()
{
    return bSelected;
}

void UBaseButton::SetForceSelected(bool bShouldForceSelect)
{
    bForceSelected = bShouldForceSelect;
}

void UBaseButton::PostLoad()
{
    Super::PostLoad();

    if (UUCRBlueprintFunctionLibrary::GetPlatformType(this, false) != EGamePlatform::Win64)
    {
        SetVisibility(ESlateVisibility::Collapsed);
    }
}
