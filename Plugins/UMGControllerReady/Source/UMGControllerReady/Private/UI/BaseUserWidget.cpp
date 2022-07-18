// Copyright © Pedro Costa, 2021. All rights reserved

#include "UI/BaseUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameViewport/UCRGameViewportClient.h"
#include "Engine/GameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/BaseButton.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include <UI/InputHelpersWidget.h>
#include "Kismet/GameplayStatics.h"
#include "UObject/NoExportTypes.h"
#include "UCRBlueprintFunctionLibrary.h"

void UBaseUserWidget::GiveParentFocus()
{
    if (ButtonParent && ButtonParent->GetMainParent())
    {
        ButtonParent->GetMainParent()->OnFocusReceivedFromChild();
        ButtonParent->GetMainParent()->PrepareUMGForVisibility(true, ButtonParent);
    }
}

void UBaseUserWidget::AddButton(UBaseButton* newButton)
{
    Buttons.Add(newButton);
}

void UBaseUserWidget::RemoveFromParent()
{
    if (IsVisible())
    {
        OnUMGVisibilityChanged(false);

        if (ButtonParent)
        {
            GiveParentFocus();
        }
        else
        {
            PrepareUMGForVisibility(false, nullptr);
        }
    }
    Super::RemoveFromParent();
}

void UBaseUserWidget::AddWidgetToViewport(UUserWidget* WidgetToAdd)
{
    WidgetToAdd->AddToViewport();
}

void UBaseUserWidget::OnInputTypeChanged(EFInputType newInputType)
{
    bool bShowCursor = newInputType == EFInputType::KeyboardMouse;
    SetCursorState(bShowCursor);


    if (UBaseButton* FocusedButton = GetFocusedButton())
    {
        FocusedButton->GetMainButton()->SetKeyboardFocus();
        FocusedButton->SetCursor(bShowCursor ? EMouseCursor::Default : EMouseCursor::None);
    }

    OnInputTypeChangedBP(newInputType);
}

void UBaseUserWidget::SetCursorState(bool bShowCursor)
{
    UUCRBlueprintFunctionLibrary::SetCursorState(this, bShowCursor);    
    FInputModeGameAndUI newInputMode;
    newInputMode.SetHideCursorDuringCapture(false);
    newInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
    APlayerController* FirstPlayerController = UGameplayStatics::GetPlayerController(this, 0);
    FirstPlayerController->SetInputMode(newInputMode);
    FirstPlayerController->bShowMouseCursor = bShowCursor;
}

void UBaseUserWidget::OnWidgetWantToClose_Implementation()
{
    RemoveFromParent();
}

void UBaseUserWidget::PrepareUMGForVisibility(bool bOpened, UUserWidget* WidgetToFocus)
{
    if (!CharacterOwner)
    {
        CharacterOwner = Cast<ACharacter>(GetOwningPlayerPawn());
    }

    HandleInputs(bOpened);
    GetWidgetButtons(this);
    if (bOpened)
    {
        if (!WidgetToFocus)
        {
            WidgetToFocus = GetFirstButton();
        }


        if (WidgetToFocus)
        {
            WidgetToFocus->SetKeyboardFocus();
        }
    }

    HandleDelegates(bOpened);
}

UBaseButton* UBaseUserWidget::GetFirstButton()
{
    return Buttons.Num() > 0 ? Buttons[0] : UUCRBlueprintFunctionLibrary::GetFirstWidgetButton(GetRootWidget());
}

UBaseButton* UBaseUserWidget::GetFocusedButton()
{
    return UUCRBlueprintFunctionLibrary::GetFocusedButton(this);
}

void UBaseUserWidget::SetVisibility(ESlateVisibility InVisibility)
{
    Super::SetVisibility(InVisibility);
    bool bOpened = !(InVisibility == ESlateVisibility::Collapsed || InVisibility == ESlateVisibility::Hidden);
    PrepareUMGForVisibility(bOpened, nullptr);
    if (!bOpened)
    {
        GiveParentFocus();
    }
    OnUMGVisibilityChanged(bOpened);
}

void UBaseUserWidget::HandleDelegates(bool bOpened)
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (bOpened)
        {
            if (UUCRGameViewportClient* GameViewport = Cast<UUCRGameViewportClient>(GameInstance->GetGameViewportClient()))
            {
                GameViewport->OnInputTypeChanged.AddUniqueDynamic(this, &UBaseUserWidget::OnInputTypeChanged);
            }
        }
        else
        {
            if (UUCRGameViewportClient* GameViewport = Cast<UUCRGameViewportClient>(GameInstance->GetGameViewportClient()))
            {
                GameViewport->OnInputTypeChanged.RemoveDynamic(this, &UBaseUserWidget::OnInputTypeChanged);
            }
        }
    }
}

TArray<UBaseButton*> UBaseUserWidget::GetWidgetButtons(UUserWidget* Parent, UWidget* Root, bool bCheckIfAlreadySet)
{
    if(Buttons.Num() == 0) {
        Buttons = UUCRBlueprintFunctionLibrary::GetWidgetButtons(this);
    }
    return Buttons;
}

void UBaseUserWidget::AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder)
{
    Super::AddToScreen(LocalPlayer, ZOrder);
    OnUMGVisibilityChanged(true);
    GetWidgetButtons(this);
    PrepareUMGForVisibility(true, GetFirstButton());
}

FReply UBaseUserWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
    Super::NativeOnFocusReceived(InGeometry, InFocusEvent);

    GetFirstButton()->SetKeyboardFocus();
    return FReply::Handled();
}

UBaseButton* UBaseUserWidget::GetFirstButtonFromWidget(UWidget* WidgetToCheck)
{
    UUserWidget* UserWidget = Cast<UUserWidget>(WidgetToCheck);

    TArray<UBaseButton*> BaseButtons = GetWidgetButtons(UserWidget, UserWidget, false);
    return BaseButtons.Num() > 0 ? BaseButtons[0] : nullptr;

    return nullptr;
}

void UBaseUserWidget::NativeConstruct()
{
    GetWidgetButtons(this);

    OnWidgetConstructedBP();
}

bool UBaseUserWidget::HasButtonsToFocus()
{
    return GetFirstButton() != nullptr;
}

void UBaseUserWidget::Refocus()
{
    if (!bHasRefocused)
    {
        UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%s refocus"), *GetName()));
        bHasRefocused = true;
        GetWorld()->GetTimerManager().SetTimerForNextTick([=]
        {
            bHasRefocused = false;
            if (UBaseButton* FocusedButon = GetFocusedButton())
            {
                FocusedButon->SetKeyboardFocus();
            }
            SetCursorState(true);
        });
    }
}

ESlateVisibility UBaseUserWidget::GetWidgetVisibility()
{
    return GetVisibility();
}

void UBaseUserWidget::HandleInputs(bool bListen)
{
    if (bListen)
    {
        if (bCanBeBacked)
        {
            FScriptDelegate Delegate;
            Delegate.BindUFunction(this, TEXT("OnWidgetWantToClose"));
            FOnInputAction InputAction(Delegate);
            AddListenInputAction("Cancel", EInputEvent::IE_Pressed, true, InputAction, true);
        }
    }
    else
    {
        StopListeningForInputAction("Cancel", EInputEvent::IE_Pressed);
    }
}

void UBaseUserWidget::AddListenInputAction(FName ActionName, TEnumAsByte< EInputEvent > EventType, bool bConsume, FOnInputAction Callback, bool bCallWhenPaused)
{
    if (!InputComponent)
    {
        InitializeInputComponent();
    }

    if (InputComponent)
    {
        FInputActionBinding NewBinding(ActionName, EventType.GetValue());
        NewBinding.bConsumeInput = bConsume;
        NewBinding.bExecuteWhenPaused = bCallWhenPaused;
        NewBinding.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &ThisClass::OnInputAction, Callback);

        InputComponent->AddActionBinding(NewBinding);
    }
}
