// Copyright Epic Games, Inc. All Rights Reserved.

#include "UMGCRProjectGameMode.h"
#include "UMGCRProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUMGCRProjectGameMode::AUMGCRProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
