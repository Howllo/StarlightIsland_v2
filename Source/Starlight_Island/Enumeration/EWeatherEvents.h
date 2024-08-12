// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeatherEvents : uint8
{
    LightRain       = 0 UMETA(DisplayName = "Light Rain"),
    Rain            = 1 UMETA(DisplayName = "Rain"),
    HeavyRain       = 2 UMETA(DisplayName = "Heavy Rain"),
    Thunderstorm    = 3 UMETA(DisplayName = "Thunderstorm"),
    LightSnow       = 4 UMETA(DisplayName = "Light Snow"),
    Snow            = 5 UMETA(DisplayName = "Snow"),
    HeavySnow       = 6 UMETA(DisplayName = "Heavy Snow"),
    Blizzard        = 7 UMETA(DisplayName = "Blizzard"),
    DustStorm       = 8 UMETA(DisplayName = "Dust Storm"),
    Dust            = 9 UMETA(DisplayName = "Dust"),
    Foggy           = 10 UMETA(DisplayName = "Foggy"),
    ClearSkies      = 11 UMETA(DisplayName = "Clear Skies"),
    PartlyCloudy    = 12 UMETA(DisplayName = "Partly Cloudy"),
    Cloudy          = 13 UMETA(DisplayName = "Cloudy"),
    Overcast        = 14 UMETA(DisplayName = "Overcast"),
};