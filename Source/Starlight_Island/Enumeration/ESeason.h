// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESeason : uint8
{
    Winter = 0 UMETA(DisplayName = "Winter"),
    Spring = 1 UMETA(DisplayName = "Spring"),
    Summer = 2 UMETA(DisplayName = "Summer"),
    Autumn = 3 UMETA(DisplayName = "Autumn"),
};