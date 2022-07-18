// Copyright © Pedro Costa, 2021. All rights reserved

#include "GameViewport/UCRGameViewportClient.h"
#include "UnrealClient.h"
#include "Engine/Engine.h"
#include "InputCoreTypes.h"
#include "UI/BaseUserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


bool UUCRGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
    CheckInput(EventArgs.Key);
    return Super::InputKey(EventArgs);
}

bool UUCRGameViewportClient::InputAxis(FViewport* OurViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples /*= 1*/, bool bGamepad /*= false*/)
{
    CheckInput(Key);
    return Super::InputAxis(OurViewport, ControllerId, Key, Delta, DeltaTime, NumSamples = 1, bGamepad = false);
}

EFInputType UUCRGameViewportClient::GetCurrentInputType()
{
    return CurrentInputType;
}

void UUCRGameViewportClient::MouseMove(FViewport* OurViewport, int32 X, int32 Y)
{
    Super::MouseMove(OurViewport, X, Y);
    if (!(FVector2D(X, Y) - MousePosition).IsNearlyZero())
    {
        CheckInput(EKeys::MouseX);
        MousePosition = FVector2D(X, Y);
    }
}

void UUCRGameViewportClient::ReceivedFocus(FViewport* FocusedViewport)
{
    Super::ReceivedFocus(FocusedViewport);

    TArray<UUserWidget*> Widgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, Widgets, UBaseUserWidget::StaticClass(), true);
    if (Widgets.Num() > 0)
    {
        Cast<UBaseUserWidget>(Widgets[0])->Refocus();
    }
}

void UUCRGameViewportClient::CheckInput(FKey Key)
{
    if (Key.IsValid())
    {
        if ((CurrentInputType != EFInputType::Controller && Key.IsGamepadKey()) || (CurrentInputType != EFInputType::KeyboardMouse && !Key.IsGamepadKey()))
        {
            CurrentInputType = Key.IsGamepadKey() ? EFInputType::Controller : EFInputType::KeyboardMouse;
            OnInputTypeChanged.Broadcast(CurrentInputType);
        }
    }
}
