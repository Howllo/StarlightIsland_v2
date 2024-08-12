// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Used to determine the status of the player inventory.
 */
UENUM(BlueprintType)
enum class EPlayerInventoryStatus : uint8
{
 Space  = 0 UMETA(DisplayName = "Space"),
 Full   = 1 UMETA(DisplayName = "Full"),
};