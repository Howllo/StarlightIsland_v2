// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlantState : uint8
{
    Seedling    = 0 UMETA(DisplayName = "Seedling"),
    Sapling     = 1 UMETA(DisplayName = "Sapling"),
    Mature      = 2 UMETA(DisplayName = "Mature"),
    Dead        = 3 UMETA(DisplayName = "Dead"),
    Harvested   = 4 UMETA(DisplayName = "Harvested"),
};