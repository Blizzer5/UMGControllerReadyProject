// Copyright © Pedro Costa, 2021. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "InputCoreTypes.h"
#include "UCRGameViewportClient.generated.h"

UENUM(BlueprintType)
enum class EFInputType : uint8
{
    KeyboardMouse,
    Controller,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputTypeChanged, EFInputType, newInputType);

/**
 * 
 */
UCLASS()
class UMGCONTROLLERREADY_API UUCRGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
    bool InputKey(const FInputKeyEventArgs& EventArgs) override;

    bool InputAxis(FViewport* OurViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples = 1, bool bGamepad = false) override;

    EFInputType GetCurrentInputType();

    UPROPERTY(BlueprintAssignable, Category = "Gameviewport | Input")
    FOnInputTypeChanged OnInputTypeChanged;

    void MouseMove(FViewport* OurViewport, int32 X, int32 Y) override;


    void ReceivedFocus(FViewport* FocusedViewport) override;

private:
    void CheckInput(FKey Key);

    EFInputType CurrentInputType;

    FVector2D MousePosition;
};
