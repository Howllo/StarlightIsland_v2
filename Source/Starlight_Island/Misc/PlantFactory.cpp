// Copyrighted Celestial Games. All Rights Reserved.


#include "PlantFactory.h"
#include "Starlight_Island/DataTable/FPlantList.h"
#include "Starlight_Island/GameFeatures/PlantingSystem/Plant.h"
#include "Starlight_Island/GameFeatures/PlantingSystem/PlantSlot.h"

UPlantFactory::UPlantFactory()
{
    static ConstructorHelpers::FObjectFinder<UDataTable>
        PlantDatabaseObject(TEXT("/Script/Engine.DataTable'/Game/DataTables/Database/DT_PlantDatabase.DT_PlantDatabase'"));
    if(PlantDatabaseObject.Succeeded())
    {
        PlantDatabase = PlantDatabaseObject.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Plant Database does not exist."));
    }
}

UPlant* UPlantFactory::CreatePlant(const FGameplayTag SeedItemID) const
{
    UPlant* Plant = NewObject<UPlant>();
    if(PlantDatabase)
    {
        const FString PlantIDString = SeedItemID.GetTagName().ToString();
        const FName PlantIDFName = *PlantIDString.Replace(*FString("Item."), *FString("")); 
        
        if(const FPlantList* PlantRow = PlantDatabase->FindRow<FPlantList>(PlantIDFName, FString("")))
        {
            Plant->PlantID = PlantRow->PlantID;
            Plant->PlantName = PlantRow->PlantName;

            for(int i = 0; i < PlantRow->PlantStageMeshes.Num(); i++)
            {
                Plant->PlantStageMeshes.Add(PlantRow->PlantStageMeshes[i]);
            }

            Plant->SeedItemID = PlantRow->SeedItemID;
            Plant->MatureItemID = PlantRow->MatureItemID;
            Plant->PlantState = PlantRow->PlantState;
            Plant->InSeasonPlant = PlantRow->InSeasonPlant;
            Plant->bIsMultiSeasonal = PlantRow->bIsMultiSeasonal;
            Plant->ExpireTime = PlantRow->ExpireTime;
            Plant->PreserveExpireTime = PlantRow->PreserveExpireTime;
            Plant->PlantGrowthRate = PlantRow->PlantGrowthRate;
            Plant->PlantWaterRate = PlantRow->PlantWaterRate;
            Plant->PlantMaxGrowth = PlantRow->PlantMaxGrowth;
            Plant->PlantDecayRate = PlantRow->PlantDecayRate;
        }
        else
        {
            const FString SeedItemIDString = SeedItemID.GetTagName().ToString();
            UE_LOG(LogTemp, Error, TEXT("Item ID: %s does not exist in the database."), *SeedItemIDString);
        }
    } else
    {
        UE_LOG(LogTemp, Error, TEXT("Plant Database does not exist or the row is less than 5."));
    }
    return Plant;
}

UPlantSlot* UPlantFactory::CreatePlantSlot(UObject* Owner, const FVector& InVector, const FRotator& InRotator,
                                             const FGameplayTag SeedItemID) const
{
    UPlantSlot* PlantSlot = nullptr;
    if(Owner)
    {
        PlantSlot = NewObject<UPlantSlot>(Owner);
        if(UPlant* Plant = CreatePlant(SeedItemID); Plant && PlantSlot)
        {
            PlantSlot->SetPlantData(Plant);
        }
        else
        {
            if(!Plant)
            {
                UE_LOG(LogTemp, Error, TEXT("Item is nullptr in UItemFactory."));
            }
			
            if (!PlantSlot)
            {
                UE_LOG(LogTemp, Error, TEXT("ITemActor is nullptr in UItemFactory."))
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("World is nullptr."));
    }
    return PlantSlot;
}
