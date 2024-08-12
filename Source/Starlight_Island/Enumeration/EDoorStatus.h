// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Used for status of the door if it is opened or closed.
 */
UENUM(BlueprintType)
enum class EDoorStatus : uint8
{
    InteractOff     = 0 UMETA(DisplayName = "Interact Off"),
    OpenAwaitClose  = 1 UMETA(DisplayName = "Open Await Close"),
    OpenCannotClose = 2 UMETA(DisplayName = "Open Cannot Close"),
    CloseAwaitOpen  = 33 UMETA(DisplayName = "Close Await Open"),
};