// Copyrighted Celestial Games. All Rights Reserved.


#include "PlantSaveData.h"
#include "Starlight_Island/Enumeration/EPlantState.h"

UPlantSaveData::UPlantSaveData(): PlantState(EPlantState::Seedling), bIsMultiSeasonal(false), ExpireTime(0), PreserveExpireTime(0),
                                  StoreSeedItem(nullptr),
                                  PlantIndex(0),
                                  DaysWithoutWater(0),
                                  AdditionalDaysToDie(0),
                                  GrowthStage(0)
{
}
