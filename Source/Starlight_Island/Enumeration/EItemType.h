// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EItemType : uint8
{
    None,
    Seed,
    Plant,
    WateringCan,
    Fertilizer,
    Tool,
    Decoration,
    Furniture,
    Food,
    Drink,
    Medicine,
    Crafting,
    Material,
    Junk,
    Quest,
    Key,
    Blueprint,
    Recipe,
    Currency,
    Gift,
    Event,
    Special,
    Max
};