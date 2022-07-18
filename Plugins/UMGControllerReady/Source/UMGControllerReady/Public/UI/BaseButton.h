// Copyright © Pedro Costa, 2021. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "BaseButton.generated.h"

class UBaseUserWidget;

/**
 * 
 */
UCLASS()
class UMGCONTROLLERREADY_API UBaseButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintPure, Category = "Base Button")
    UBaseUserWidget* GetMainParent();

    void SetMainParent(UBaseUserWidget* newMainParent);

    UFUNCTION(BlueprintPure, Category = "Base Button")
    UButton* GetMainButton();

    UFUNCTION(BlueprintPure, Category = "Base Button")
    bool IsSelected();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    bool bPressWhenSelected;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    bool bDontShowOnConsoles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	bool bCanReceiveFocus;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* BaseButton;

    UFUNCTION(BlueprintCallable, Category = "Base Button")
    void SetForceSelected(bool bShouldForceSelect);
protected:
    virtual void PostLoad() override;
    UPROPERTY(BlueprintReadWrite, Category = "Base Button")
    bool bSelected;
    UPROPERTY(BlueprintReadWrite, Category = "Base Button")
    bool bForceSelected;

    UBaseUserWidget* ButtonParent;
};
