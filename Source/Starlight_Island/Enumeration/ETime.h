// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


/**
 * Used to determine the status of the player inventory.
 */
UENUM(BlueprintType)
enum class ETime : uint8
{
    TwelveHour          = 0 UMETA(DisplayName = "12-Hour"),
    TwentyFourHour      = 1 UMETA(DisplayName = "24-Hour"),
};