// Copyrighted Celestial Games. All Rights Reserved.


#include "FPlantList.h"
#include "Starlight_Island/Enumeration/EPlantState.h"
#include "Starlight_Island/Enumeration/ESeason.h"
#include "Starlight_Island/NativeTags/FPlantTag.h"

FPlantList::FPlantList()
{
    PlantName = FText::FromString("None");
    SeedItemID = PlantTag::Default;
    MatureItemID = PlantTag::Default;
    PlantFiberItemID = PlantTag::Default;
    PlantState = EPlantState::Seedling;
    InSeasonPlant = ESeason::Spring;
    bIsMultiSeasonal = false;
    ExpireTime = 0.f;
    PreserveExpireTime = 0.f;
    PlantDecayRate = 0.f;
    PlantGrowthRate = 1.f;
    PlantWaterRate = 1.f;
    PlantMaxGrowth = 1.f;
}
