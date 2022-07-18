// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InputHelperWidget.h"
#include "GameViewport/UCRGameViewportClient.h"

FName UInputHelperWidget::GetInputActionName()
{
	return InputActionName;
}

void UInputHelperWidget::OnInputChanged(EFInputType newInputType)
{
	if(Visibility != ESlateVisibility::Collapsed && Visibility != ESlateVisibility::Hidden) {
		UpdateInputHelper();
	}
}

void UInputHelperWidget::NativeConstruct()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UUCRGameViewportClient* GameViewport = Cast<UUCRGameViewportClient>(GameInstance->GetGameViewportClient()))
		{
			GameViewport->OnInputTypeChanged.AddUniqueDynamic(this, &UInputHelperWidget::OnInputChanged);
		}
	}

	UpdateInputHelper();
}
