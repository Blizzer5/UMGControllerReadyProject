// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputHelperWidget.h"
#include "InputHelpersWidget.generated.h"

/**
 *
 */
UCLASS()
class UMGCONTROLLERREADY_API UInputHelpersWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void AddInputHelper(const FText& Action, FName InputActionName, bool bMatchInputType, bool bJustIcon);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void RemoveInputHelper(FName InputActionName);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void UpdateInputHelpers();

    UFUNCTION(BlueprintPure)
    UInputHelperWidget* GetHelperFromInputActionName(FName InputActionName);

protected:
    UPROPERTY(BlueprintReadWrite)
    TArray<UInputHelperWidget*> InputHelpers;
};
