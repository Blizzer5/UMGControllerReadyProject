// Copyright © Pedro Costa, 2021. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../UMGControllerReadyTypes.h"
#include "GameViewport/UCRGameViewportClient.h"
#include "GameFramework/PlayerInput.h"
#include <UI/BaseButton.h>
#include "UCRBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UMGCONTROLLERREADY_API UUCRBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    
    UFUNCTION(BlueprintCallable, Category = "UCRBlueprintFunctionLibrary")
    static EGamePlatform GetPlatformType(UObject* WorldContext, bool bCheckForController = true);

    UFUNCTION(BlueprintCallable, Category = "UCRBlueprintFunctionLibrary")
    static EFInputType GetCurrentInputType(UObject* WorldContext);

    UFUNCTION(BlueprintCallable, Category = "UCRBlueprintFunctionLibrary")
    static const FInputActionKeyMapping GetKeyAssignedToAction(AController* Controller, const FName ActionName, bool bMatchInputType);
    
    UFUNCTION(BlueprintCallable, Category = "UCRBlueprintFunctionLibrary")
    static UTexture2D* GetIconTextureFromKey(UObject* WorldContext, FKey key);

    UFUNCTION(BlueprintCallable, Category = "UCRBlueprintFunctionLibrary")
    static bool AreIconsLoaded();

    UFUNCTION(BlueprintCallable, Category = "UCRBlueprintFunctionLibrary")
    static void SetPlatformKeyIcons(TArray<FPlatformKeyIcon> KeyIcons);

    UFUNCTION(BlueprintPure, BlueprintCallable, Category = "UCRBlueprintFunctionLibrary")
    static TArray<UBaseButton*> GetWidgetButtons(UUserWidget* WidgetToCheck, bool bSearchInsideChildren = false);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "UCRBlueprintFunctionLibrary")
	static UBaseButton* GetFirstWidgetButton(UWidget* WidgetToCheck);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "UCRBlueprintFunctionLibrary")
	static UBaseButton* GetFocusedButton(UWidget* WidgetToCheck);

	UFUNCTION(BlueprintCallable, Category = "UCRBlueprintFunctionLibrary")
	static void SetCursorState(UUserWidget* WidgetToCheck, bool bShowCursor);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "UCRBlueprintFunctionLibrary")
	static bool IsButtonSelected(UBaseButton* ButtonToCheck);
private:
    static TArray<FPlatformKeyIcon> PlatformKeysIcons;
};
