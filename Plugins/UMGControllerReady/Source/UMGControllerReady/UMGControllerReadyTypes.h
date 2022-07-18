#pragma once

#include "UObject/ObjectMacros.h"
#include "Engine/DataTable.h"
#include "UMGControllerReadyTypes.generated.h"

UENUM(BlueprintType)
enum class EGamePlatform : uint8
{
    None,
    Win64,
    PS4,
    XboxOne,
};

/* Used to create a database with the platforms respective buttons */
USTRUCT(BlueprintType)
struct UMGCONTROLLERREADY_API FPlatformKeyIcon : public FTableRowBase
{
    GENERATED_BODY()
        
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGamePlatform Platform;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FKey, UTexture2D*> PlatformKeyIcon;

    bool IsValid()
    {
        return Platform != EGamePlatform::None;
    }
};