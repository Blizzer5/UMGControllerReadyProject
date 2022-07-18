// Copyright © Pedro Costa, 2021. All rights reserved


#include "UCRBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameViewport/UCRGameViewportClient.h"
#include "GameFramework/Character.h"
#include "Blueprint/WidgetTree.h"

TArray<FPlatformKeyIcon> UUCRBlueprintFunctionLibrary::PlatformKeysIcons = TArray<FPlatformKeyIcon>();

EGamePlatform UUCRBlueprintFunctionLibrary::GetPlatformType(UObject* WorldContext, bool bCheckForController)
{
	/*(Platform names include Windows, Mac, IOS, Android, PS4, XboxOne, Linux)*/
	FString PlatformName = UGameplayStatics::GetPlatformName();
	if (PlatformName.Equals("PS4"))
	{
		return EGamePlatform::PS4;
	}
	else if (PlatformName.Equals("XboxOne"))
	{
		return EGamePlatform::XboxOne;
	}
	return bCheckForController && GetCurrentInputType(WorldContext) == EFInputType::Controller ? EGamePlatform::XboxOne : EGamePlatform::Win64;
}

EFInputType UUCRBlueprintFunctionLibrary::GetCurrentInputType(UObject* WorldContext)
{
	if (WorldContext && WorldContext->GetWorld())
	{
		if (UGameInstance* GameInstance = WorldContext ? WorldContext->GetWorld()->GetGameInstance<UGameInstance>() : nullptr)
		{
			if (UUCRGameViewportClient* GameViewport = Cast<UUCRGameViewportClient>(GameInstance->GetGameViewportClient()))
			{
				return GameViewport->GetCurrentInputType();
			}
		}
	}
	return EFInputType::KeyboardMouse;
}

const FInputActionKeyMapping UUCRBlueprintFunctionLibrary::GetKeyAssignedToAction(AController* Controller, const FName ActionName, bool bMatchInputType)
{
	if (Controller)
	{
		if (APlayerController* PC = Cast<APlayerController>(Controller))
		{
			const TArray<FInputActionKeyMapping> Mappings = PC->PlayerInput->GetKeysForAction(ActionName);
			if (Mappings.Num() > 0)
			{
				if (bMatchInputType)
				{
					bool bIsGamepadInput = GetCurrentInputType(Controller) == EFInputType::Controller;
					return *Mappings.FindByPredicate([&](const FInputActionKeyMapping Mapping)
					{
						return Mapping.Key.IsGamepadKey() == bIsGamepadInput;
					});
				}
				else
				{
					return PC->PlayerInput->GetKeysForAction(ActionName)[0];
				}
			}
		}
	}
	return {};
}

UTexture2D* UUCRBlueprintFunctionLibrary::GetIconTextureFromKey(UObject* WorldContext, FKey key)
{
	if (AreIconsLoaded())
	{
		FPlatformKeyIcon* FoundPlatformKeyIcons = PlatformKeysIcons.FindByPredicate([&](FPlatformKeyIcon KeyIcon) { return KeyIcon.Platform == GetPlatformType(WorldContext); });
		if (FoundPlatformKeyIcons && (*(FoundPlatformKeyIcons)).IsValid())
		{
			return FoundPlatformKeyIcons->PlatformKeyIcon.Contains(key) ? FoundPlatformKeyIcons->PlatformKeyIcon[key] : nullptr;
		}
	}
	return nullptr;
}

bool UUCRBlueprintFunctionLibrary::AreIconsLoaded()
{
	return PlatformKeysIcons.Num() > 0;
}

void UUCRBlueprintFunctionLibrary::SetPlatformKeyIcons(TArray<FPlatformKeyIcon> KeyIcons)
{
	PlatformKeysIcons = KeyIcons;
}

TArray<UBaseButton*> UUCRBlueprintFunctionLibrary::GetWidgetButtons(UUserWidget* WidgetToCheck, bool bSearchInsideChildren)
{
	if (!WidgetToCheck) {
		return {};
	}
	TArray<UBaseButton*> Buttons;
	UBaseUserWidget* WidgetAsBase = Cast<UBaseUserWidget>(WidgetToCheck);

	WidgetToCheck->WidgetTree->ForEachWidget([&](UWidget* Widget) {
		check(Widget);
		if (UBaseButton* Button = Cast<UBaseButton>(Widget))
		{
			if (WidgetAsBase && !Button->GetMainParent()) {
				Button->SetMainParent(WidgetAsBase);
			}

			if (Button->bIsFocusable) {
				Buttons.Add(Button);
			}
		}
		else if (UUserWidget* UserWidget = Cast<UUserWidget>(Widget)) {
			if (bSearchInsideChildren) {
				Buttons.Append(GetWidgetButtons(UserWidget, bSearchInsideChildren));
			}
		}
	});

	return Buttons;
}

UBaseButton* UUCRBlueprintFunctionLibrary::GetFirstWidgetButton(UWidget* WidgetToCheck)
{
	if (!WidgetToCheck) {
		return nullptr;
	}

	UPanelWidget* WidgetAsPanel = Cast<UPanelWidget>(WidgetToCheck);
	for (UWidget* Child : WidgetAsPanel->GetAllChildren())
	{
		if (UBaseButton* Button = Cast<UBaseButton>(Child)) {
			return Button;
		}
		else if (UUserWidget* UserWidget = Cast<UUserWidget>(Child)) {
			if (GetWidgetButtons(UserWidget, true).Num() > 0) {
				return GetWidgetButtons(UserWidget,  true)[0];
			}
		}
		else if (UPanelWidget* ChildAsPanel = Cast<UPanelWidget>(Child)) {
			if (UBaseButton* ChildButton = GetFirstWidgetButton(ChildAsPanel)) {
				if (ChildButton->bIsFocusable) {
					return ChildButton;
				}
			}
		}
	}
	return nullptr;
}

UBaseButton* UUCRBlueprintFunctionLibrary::GetFocusedButton(UWidget* WidgetToCheck)
{
	if (!WidgetToCheck) {
		return nullptr;
	}

	if (UBaseButton* Button = Cast<UBaseButton>(WidgetToCheck)) {
		if (IsButtonSelected(Button)) {
			return Button;
		}
	}

	UPanelWidget* WidgetAsPanel = Cast<UPanelWidget>(WidgetToCheck);
	if (!WidgetAsPanel) {
		if (UUserWidget* WidgetAsUserWidget = Cast<UUserWidget>(WidgetToCheck)) {
			TArray<UBaseButton*> WidgetButtons = GetWidgetButtons(WidgetAsUserWidget , true);
			for (UBaseButton* button : WidgetButtons) {
				if (IsButtonSelected(button)) {
					return button;
				}
			}
		}
	}

	if (WidgetAsPanel) {
		for (UWidget* Child : WidgetAsPanel->GetAllChildren())
		{
			if (UBaseButton* Button = Cast<UBaseButton>(Child)) {
				if (IsButtonSelected(Button)) {
					return Button;
				}
			}
			if (UBaseButton* ChildButton = GetFocusedButton(Child)) {
				return ChildButton;
			}
		}
	}
	return nullptr;
}

void UUCRBlueprintFunctionLibrary::SetCursorState(UUserWidget* WidgetToCheck, bool bShowCursor)
{
	if (!WidgetToCheck) {
		return;
	}

	EMouseCursor::Type CursorType = bShowCursor ? EMouseCursor::Default : EMouseCursor::None;

	GEngine->GameViewport->Viewport->ShowCursor(false);

	WidgetToCheck->GetRootWidget()->SetCursor(CursorType);

	UPanelWidget* WidgetAsPanel = Cast<UPanelWidget>(WidgetToCheck->GetRootWidget());
	for (UWidget* Child : WidgetAsPanel->GetAllChildren())
	{
		if (UUserWidget* UserWidget = Cast<UUserWidget>(Child)) {
			UserWidget->SetCursor(CursorType);
			GetWidgetButtons(UserWidget).Num();
		}
		else {
			Child->SetCursor(CursorType);
		}
	}
}

bool UUCRBlueprintFunctionLibrary::IsButtonSelected(UBaseButton* ButtonToCheck)
{
	return ButtonToCheck && (ButtonToCheck->HasKeyboardFocus() || ButtonToCheck->IsSelected());
}
