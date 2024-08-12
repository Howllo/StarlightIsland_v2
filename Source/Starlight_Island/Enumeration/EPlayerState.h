// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerGameState : uint8
{
    Idle       = 0 UMETA(DisplayName = "Idle"),
    Walking    = 1 UMETA(DisplayName = "Walking"),
    Running    = 2 UMETA(DisplayName = "Running"),
    Crouching  = 3 UMETA(DisplayName = "Crouching"),
    Swimming   = 4 UMETA(DisplayName = "Swimming"),
    Climbing   = 5 UMETA(DisplayName = "Climbing"),
    Jumping    = 6 UMETA(DisplayName = "Jumping"),
    Falling    = 7 UMETA(DisplayName = "Falling"),
    Stunned    = 8 UMETA(DisplayName = "Stunned"),
    Staggered  = 9 UMETA(DisplayName = "Staggered"),
    Dead       = 10 UMETA(DisplayName = "Dead"),
    Temperature= 11 UMETA(DisplayName = "Temperature"),
    Default    = 12 UMETA(DisplayName = "Default Disable Movement")
};