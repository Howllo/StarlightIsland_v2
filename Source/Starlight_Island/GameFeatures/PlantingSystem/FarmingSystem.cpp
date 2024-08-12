// Copyrighted Celestial Games. All Rights Reserved.


#include "FarmingSystem.h"
#include "Plant.h"
#include "PlantSlot.h"
#include "Components/SphereComponent.h"
#include "Starlight_Island/Enumeration/EWeatherEvents.h"
#include "Starlight_Island/GameFeatures/Inventory/Item.h"
#include "Starlight_Island/GameMode/StarlightGameInstance.h"
#include "Starlight_Island/GameMode/StarlightGameState.h"
#include "Starlight_Island/Manager/TimeManager.h"
#include "Starlight_Island/Manager/UltraDynamicWeatherHandler.h"
#include "Starlight_Island/Misc/ItemFactory.h"
#include "Starlight_Island/Misc/PlantFactory.h"
#include "Starlight_Island/NativeTags/ItemTag.h"
#include "Starlight_Island/SaveSystem/SaveData/FarmingSaveData.h"


// Sets default values
AFarmingSystem::AFarmingSystem()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create Root component
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // Set up the static mesh component for the planting area.
    PlantingArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Planting Area"));
    PlantingArea->SetupAttachment(Root);

    // Set up the total allowed plants.
    TotalAllowedPlants = 9;

    // Set up the current plants.
    CurrentPlants = 0;

    // Create the slots for the plants.
    CreatePlantSlots();

    // Set up the delegate for OnHarvest.
    OnHarvested.BindUFunction(this, "OnHarvest");

    // Water Loss Rate
    NormalTempLoss = 0.1f;
    BaseRateLoss = 0.1f;
    MaxWaterAmount = 75.f;
    CurrentWaterAmount = 0.f;

    // Populate Growth Percentage
    GrowthPercentage.Add(0.25f);
    GrowthPercentage.Add(0.5f);
    GrowthPercentage.Add(0.75f);
    GrowthPercentage.Add(1.f);
}

void AFarmingSystem::BeginPlay()
{
    Super::BeginPlay();
    
    GameInstance = Cast<UStarlightGameInstance>(GetGameInstance());
    const AStarlightGameState* GameState = GetWorld()->GetGameState<AStarlightGameState>();
    TimeManager = GameState->GetTimeManager();
    Weather = GameState->GetUltraDynamicWeatherHandler();

    // Events
    TimeManager->OnHourUpdate.AddUObject(this, &AFarmingSystem::DistributeWaterToPlants);
    TimeManager->OnRealMinuteUpdate.AddUObject(this, &AFarmingSystem::RemoveWaterFromSupply);
    TimeManager->OnHourUpdate.AddUObject(this, &AFarmingSystem::AddWaterOnEventCall);

    // Create Fake Plants for test purpose.
    if(true)
    {
        for(int32 i = 0; i < PlantingSlots.Num(); i++)
        {
            AddPlant(GameInstance->GetItemFactory()->CreateItem(ItemTag::Default, 1));
        }
    }
}

void AFarmingSystem::AddPlant(UItem* InItem)
{
    if(CurrentPlants < TotalAllowedPlants)
    {
        for(int32 i = 0; i < PlantingSlots.Num(); i++)
        {
            if(PlantingSlots[i]->IsSlotEmpty())
            {
                UPlant* Plant = GameInstance->GetPlantFactory()->CreatePlant(InItem->ItemID);
                PlantingSlots[i]->SetPlantData( Plant );
                PlantingSlots[i]->SetOnHarvested(OnHarvested);

                if(PlantingSlots[i]->GetPlantData()->PlantStageMeshes.Num() > 0)
                {
                    PlantingSlots[i]->GetMeshComponent()->SetStaticMesh(Plant->PlantStageMeshes[0]);
                }
                
                Plant->PlantIndex = i;
                Plant->PlantState = EPlantState::Seedling;
                CurrentPlants++;
                break;
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("You have reached the maximum amount of plants allowed."));
    }
}

void AFarmingSystem::OnHarvest(UPlant* PlantToRemove)
{
    if(CurrentPlants > 0 && PlantToRemove)
    {
        PlantingSlots[PlantToRemove->PlantIndex]->RemovePlant();
        CurrentPlants--;
    }
}

void AFarmingSystem::WaterPlant(const float WaterAmount)
{
    CurrentWaterAmount += WaterAmount;
}

void AFarmingSystem::AddWaterOnEventCall()
{
    if(Weather)
    {
        CurrentWaterAmount += Weather->GetRainfall();
    }
}

void AFarmingSystem::SaveGame(FString& Version, FString& Slot)
{
    FarmingSaveData = Cast<UFarmingSaveData>(UGameplayStatics::CreateSaveGameObject(UFarmingSaveData::StaticClass()));
    
    if(FarmingSaveData)
    {
        FarmingSaveData->PlanterLocation = GetActorLocation();
        FarmingSaveData->PlanterRotation = GetActorRotation();

        for(const auto Slots : PlantingSlots)
        {
            if(Slots->GetPlantData())
            {
                FarmingSaveData->Plants.Add(Slots->GetPlantData());
            }
        }

        // Delegate for saving the game.
        FAsyncSaveGameToSlotDelegate SaveGameDelegate;
        
        SaveGameDelegate.BindUObject(this, &AFarmingSystem::SaveGameComplete);
        UGameplayStatics::AsyncSaveGameToSlot(FarmingSaveData, Slot, 0, SaveGameDelegate);
    }
}

void AFarmingSystem::LoadGame(FString& Version, FString& Slot)
{
    // Delegate for loading the game.
    FAsyncLoadGameFromSlotDelegate LoadedDelegate;
    
    LoadedDelegate.BindUObject(this, &AFarmingSystem::LoadGameComplete);
    UGameplayStatics::AsyncLoadGameFromSlot(Slot, 0, LoadedDelegate);
}

void AFarmingSystem::SaveGameComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
    if(bSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Successfully saved the game data."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save the game data."));
    }
}

void AFarmingSystem::LoadGameComplete(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData)
{
    if(LoadedGameData)
    {
        if(UFarmingSaveData* LoadedData = Cast<UFarmingSaveData>(LoadedGameData))
        {
            for(auto Plant : LoadedData->Plants)
            {
                AddPlant(GameInstance->GetItemFactory()->CreateItem(Plant->PlantID, 1));
            }
            
            SetActorLocation(LoadedData->PlanterLocation);
            SetActorRotation(LoadedData->PlanterRotation);
        }
    }
}

void AFarmingSystem::CreatePlantSlots()
{
    for(int32 i = 0; i < TotalAllowedPlants; i++)
    {
        UPlantSlot* PlantSlot = CreateDefaultSubobject<UPlantSlot>(*FString::Printf(TEXT("Plant Slot %d"), i));

        // Setup Plant Slot Mesh Component
        if(UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(
            *FString::Printf(TEXT("Plant Mesh %d"), i)))
        {
            MeshComponent->CastShadow = false;
            PlantSlot->SetMeshComponent(MeshComponent);
            PlantSlot->GetMeshComponent()->AttachToComponent(PlantSlot, FAttachmentTransformRules::KeepRelativeTransform);
        }

        // Setup Plant Slot Collision Component
        if(USphereComponent* CollisionComponent = CreateDefaultSubobject<USphereComponent>(
            *FString::Printf(TEXT("Plant Collision %d"), i)))
        {
            PlantSlot->SetCollisionComponent(CollisionComponent);
            PlantSlot->GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            PlantSlot->GetCollisionComponent()->AttachToComponent(PlantSlot, FAttachmentTransformRules::KeepRelativeTransform);
            CollisionComponent->SetSphereRadius(10.5f);
        }

        // Attach the Plant Slot to the Root Component.
        PlantSlot->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        PlantingSlots.Add(PlantSlot);
    }
}

void AFarmingSystem::RemoveWaterFromSupply()
{
    if(!Weather)
    {
        UE_LOG(LogTemp, Error, TEXT("Weather is not set."));
        return;
    }
    
    if(LossScalingRate == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("LossScalingRate cannot be zero."));
        return;
    }

    if(Weather->GetWeather() >= EWeatherEvents::LightRain && Weather->GetWeather() <= EWeatherEvents::Thunderstorm)
    {
        return;
    }
    
    const float Temperature = (Weather->GetTemperature() - 32) * 5 / 9;
    const float WaterLoss = BaseRateLoss * (1 + (Temperature - NormalTempLoss)/LossScalingRate) *
        (1 - Weather->GetHumidity()/100);
    
    CurrentWaterAmount = FMath::Max(CurrentWaterAmount - WaterLoss, 0);
}

void AFarmingSystem::DistributeWaterToPlants()
{
    const float WaterPerPlant = CurrentWaterAmount / static_cast<float>(CurrentPlants);
    for(const UPlantSlot* Slot : PlantingSlots)
    {
        UPlant* Plant = Slot->GetPlantData();
        
        if(Slot->IsSlotEmpty() || Plant->bIsMature || Plant->PlantState == EPlantState::Dead)
        {
            continue;
        }

        if(Plant && WaterPerPlant > Plant->PlantWaterRate)
        {
            CurrentWaterAmount -= Plant->PlantWaterRate;
            Plant->DaysWithoutWater--;
            UpdatePlantGrowth(*Plant);
        }
        else if(Plant->AdditionalDaysToDie < 4 && Plant->PlantWaterRate * 0.51f > WaterPerPlant)
        {
            if(Plant->AdditionalDaysToDie == 4)
            {
                Plant->DaysWithoutWater++;
            }
            else if(Plant->AdditionalDaysToDie < 4)
            {
                Plant->AdditionalDaysToDie++;
            }

            // Get the amount of water that will be used for the plant growth. 2.3 / 2.5 = 0.92 (92%)
            UpdatePlantGrowth(*Plant, WaterPerPlant / Plant->PlantWaterRate);

            // Remove the water from the supply.
            CurrentWaterAmount = FMath::Max(CurrentWaterAmount - Plant->PlantWaterRate, 0.f);
        }
        else if(WaterPerPlant < Plant->PlantWaterRate * 0.51f)
        {
            Plant->DaysWithoutWater++;
            UpdatePlantGrowth(*Plant, WaterPerPlant / Plant->PlantWaterRate);
            CurrentWaterAmount = FMath::Max(CurrentWaterAmount - Plant->PlantWaterRate, 0.f);
        }
    }
}

int32 AFarmingSystem::GetTotalNonMaturePlants() const
{
    int32 TotalNonMaturePlants = 0;
    for(const UPlantSlot* Slot : PlantingSlots)
    {
        if(!Slot->GetPlantData()->bIsMature)
        {
            TotalNonMaturePlants++;
        }
    }
    return TotalNonMaturePlants;
}

void AFarmingSystem::UpdatePlantGrowth(UPlant& Plant, const float Percentage)
{
    if(Plant.PlantCurrentGrowth < Plant.PlantMaxGrowth)
    {
        Plant.PlantCurrentGrowth +=
            // Growth
            Plant.PlantGrowthRate * Percentage +
                // Decay
            ( (1.f - Percentage) * Plant.PlantDecayRate );
    }
    else
    {
        Plant.bIsMature = true;
    }

    CheckPlantForGrowth(Plant);
}

void AFarmingSystem::CheckPlantForGrowth(UPlant& Plant)
{
    if(Plant.bIsMature)
    {
        return;
    }

    const UPlantSlot* Slot = PlantingSlots[Plant.PlantIndex];

    if(Plant.DaysWithoutWater >= Plant.DaysToDieAndGrow)
    {
        Plant.PlantState = EPlantState::Dead;
        Plant.bIsMature = false;
        Plant.GrowthStage = 0;
        Slot->GetMeshComponent()->SetStaticMesh(Plant.PlantStageMeshes[Plant.GrowthStage]);
        return;
    }
    
    if(Plant.PlantCurrentGrowth >= Plant.PlantMaxGrowth * GrowthPercentage[Plant.GrowthStage])
    {
        Plant.GrowthStage++;

        // TODO: Add animation, sound effect or VFX for plant growth stage change.
        
        Slot->GetMeshComponent()->SetStaticMesh(Plant.PlantStageMeshes[Plant.GrowthStage]);
        
        if(Plant.GrowthStage == Plant.PlantStageMeshes.Num() - 1)
        {
            Plant.bIsMature = true;
        }
    }
}
