// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputHelperWidget.generated.h"

/**
 *
 */
UCLASS()
class UMGCONTROLLERREADY_API UInputHelperWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Input Helper Widget")
        FName GetInputActionName();
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Input Helper Widget")
        void UpdateInputHelper();

    UFUNCTION()
        void OnInputChanged(EFInputType newInputType);
protected:
    UPROPERTY(BlueprintReadWrite, Category = "Input Helper Widget")
    FKey InputHelperKey;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Helper Widget", meta = (ExposeOnSpawn = "true"))
    FText ActionText;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Helper Widget", meta = (ExposeOnSpawn = "true"))
    FName InputActionName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Helper Widget", meta = (ExposeOnSpawn = "true"))
    bool bMatchInputType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Helper Widget", meta = (ExposeOnSpawn = "true"))
    bool bJustIcon;

    void NativeConstruct() override;

};
