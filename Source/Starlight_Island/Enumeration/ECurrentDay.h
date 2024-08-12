// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECurrentDay : uint8
{
    None        = 0 UMETA(DisplayName = "None"),
    Monday      = 1 UMETA(DisplayName = "Monday"),
    Tuesday     = 2 UMETA(DisplayName = "Tuesday"),
    Wednesday   = 3 UMETA(DisplayName = "Wednesday"),
    Thursday    = 4 UMETA(DisplayName = "Thursday"),
    Friday      = 5 UMETA(DisplayName = "Friday"),
    Saturday    = 6 UMETA(DisplayName = "Saturday"),
    Sunday      = 7 UMETA(DisplayName = "Sunday"),
};
