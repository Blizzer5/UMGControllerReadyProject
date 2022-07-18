// Copyright © Pedro Costa, 2021. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "InputHelpersWidget.h"
#include "BaseUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UMGCONTROLLERREADY_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Base User Widget")
	void OnWidgetConstructedBP();

    void RemoveFromParent() override;

    UFUNCTION(BlueprintCallable, Category = "Base User Widget")
    void AddWidgetToViewport(UUserWidget* WidgetToAdd);

    void EnableDisableButtons(bool bEnable);

    UFUNCTION(BlueprintImplementableEvent, Category = "Base User Widget")
    void OnUMGVisibilityChanged(bool bOpened);

    UFUNCTION()
    void OnInputTypeChanged(EFInputType newInputType);

    UFUNCTION(BlueprintImplementableEvent, Category = "Base User Widget")
    void OnInputTypeChangedBP(EFInputType newInputType);

    UFUNCTION(BlueprintNativeEvent, Category = "Base User Widget")
    void OnWidgetWantToClose();

    UFUNCTION(BlueprintImplementableEvent, Category = "Base User Widget")
    void OnFocusReceivedFromChild();

	UFUNCTION(BlueprintCallable, Category = "Base User Widget")
	void PrepareUMGForVisibility(bool bOpened, UUserWidget* WidgetToFocus);

    UFUNCTION(BlueprintCallable, Category = "Base User Widget")
    void GiveParentFocus();

    UFUNCTION(BlueprintCallable, Category = "Base User Widget")
    void SetCursorState(bool bShowCursor);

    UFUNCTION(BlueprintCallable, Category = "Base User Widget")
    void AddButton(UBaseButton* newButton);

    UFUNCTION(BlueprintPure, Category = "Base User Widget")
    UBaseButton* GetFirstButton();

    UFUNCTION(BlueprintPure, Category = "Base User Widget")
    UBaseButton* GetFocusedButton();

    UFUNCTION(BlueprintCallable, Category = "Base User Widget")
    virtual void HandleInputs(bool bListen);

    UFUNCTION(BlueprintCallable, Category = "Base User Widget")
    void AddListenInputAction(FName ActionName, TEnumAsByte< EInputEvent > EventType, bool bConsume, FOnInputAction Callback, bool bCallWhenPaused);

    void SetVisibility(ESlateVisibility InVisibility) override;

    void HandleDelegates(bool bOpened);

    UFUNCTION(BlueprintPure, Category = "Base User Widget")
    TArray<UBaseButton*> GetWidgetButtons(UUserWidget* Parent, UWidget* Root = nullptr, bool bCheckIfAlreadySet = true);

    bool HasButtonsToFocus();

    void Refocus();

protected:
    void AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base User Widget")
        bool bCanBeBacked = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base User Widget", Meta = (ExposeOnSpawn = true))
        UBaseButton* ButtonParent;

    UPROPERTY(BlueprintReadWrite, Category = "Base User Widget")
        TArray<UBaseButton*> Buttons;

    UPROPERTY(BlueprintReadWrite, Category = "Base User Widget")
        ACharacter* CharacterOwner;

    FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

    UBaseButton* GetFirstButtonFromWidget(UWidget* WidgetToCheck);


    void NativeConstruct() override;

private:
    UFUNCTION(BlueprintPure, Category = "Base User Widget | Visibility")
    ESlateVisibility GetWidgetVisibility();

    // When we use "SetKeyboardFocus" the gameviewport will receive a "FocusReceived" event
    // This helps us to prevent a loop
    bool bHasRefocused;
};
