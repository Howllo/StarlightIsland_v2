// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Starlight_Island/SaveSystem/GameplaySaveSystem.h"
#include "FarmingSystem.generated.h"

class UFarmingSaveData;
class AUltraDynamicWeatherHandler;
class ATimeManager;
class UPlantSlot;
class UStarlightGameInstance;
class UPlant;
class UItem;
enum class EPlantState : uint8;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnHarvestDelegate, UPlant*, Plant);

/**
 * @brief The farming system for the game that allows the player to plant and harvest plants.
 *
 * The farming system allows the player to plant and harvest plants.
 * The player can plant a plant in the planting area and water the plant to help it grow.
 * The player can also harvest the plant when it is ready to be harvested.
 * This is hard dependency on the PlantSlot and Plant class since this class handle the growth of the plant.
 */
UCLASS()
class STARLIGHT_ISLAND_API AFarmingSystem : public AActor, public IGameplaySaveSystem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Plant System", meta=(AllowPrivateAccess="true"))
    USceneComponent* Root;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System", meta=(AllowPrivateAccess="true"))
    UStaticMeshComponent* PlantingArea;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant System", meta=(AllowPrivateAccess="true"))
    TArray<UPlantSlot*> PlantingSlots;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Plant System", meta=(AllowPrivateAccess="true",
        ToolTip = "The growth percentage of the plant before it switches mesh. This is a percentage. (0.0 to 1.0"))
    TArray<float> GrowthPercentage;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant System", meta=(AllowPrivateAccess="true"))
    int32 TotalAllowedPlants;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant System", meta=(AllowPrivateAccess="true"))
    int32 CurrentPlants;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System", meta=(AllowPrivateAccess="true",
        ToolTip="The maximum amount of water that the water supply can hold."))
    float MaxWaterAmount;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant System", meta=(AllowPrivateAccess="true",
        ToolTip = "The current water amount in the water supply."))
    float CurrentWaterAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System|Water Supply", meta=(AllowPrivateAccess="true",
        Tooltip="A reference temperature where water loss is normal (e.g., 20°C). This is celsius."))
    float NormalTempLoss;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System|Water Supply", meta=(AllowPrivateAccess="true",
        Tooltip="A constant representing the base water loss per minute."))
    float BaseRateLoss;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System|Water Supply", meta=(AllowPrivateAccess="true",
          Tooltip="A scaling factor to adjust the sensitivity of temperature changes on water loss (e.g., 10°C). This is celsius."))
    float LossScalingRate;
    
    UPROPERTY()
    UStarlightGameInstance* GameInstance;

    UPROPERTY()
    ATimeManager* TimeManager;
    
    UPROPERTY()
    AUltraDynamicWeatherHandler* Weather;

    UPROPERTY()
    UFarmingSaveData* FarmingSaveData;

public:
    // Sets default values for this actor's properties
    AFarmingSystem();

    /**
     * @brief Add a plant to the planting system.
     * @param InItem The item to add to the planting system.
     */
    UFUNCTION(BlueprintCallable, Category="Plant System")
    void AddPlant(UItem* InItem);

    /**
     * @brief Remove a plant from the planting system.
     * @param PlantToRemove The plant to remove from the planting system.
     */
    UFUNCTION(BlueprintCallable, Category="Plant System")
    void OnHarvest(UPlant* PlantToRemove);

    /**
     * @brief Water the plant.
     * @param WaterAmount The amount of water to give the plant.
     */
    UFUNCTION(BlueprintCallable, Category="Plant System")
    void WaterPlant(float WaterAmount);
    
    /**
     * @brief Add water to the plant on an event call.
     */
    UFUNCTION()
    void AddWaterOnEventCall();

    /*
     * @brief Save the game data from the farming system.
     */
    virtual void SaveGame(FString& Version, FString& Slot) override;

    /*
     * @brief Load the game data from the farming system.
     */
    virtual void LoadGame(FString& Version, FString& Slot) override;

protected:
    /**
     * @brief Called when the game data is saved.
     */
    virtual void SaveGameComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess) override;

    /**
     * @brief Called when the game data is loaded.
     */
    virtual void LoadGameComplete(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData) override;
    
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /**
     * @brief Create the plant slots.
     */
    void CreatePlantSlots();
    
    /**
     * @brief Remove water from the water supply per real life minute.
     */
    void RemoveWaterFromSupply();
    
    /**
     * @brief Distribute water to the plants per in-game hour.
     */
    void DistributeWaterToPlants();

    /**
     * @brief Get the total number of non-mature plants.
     * @return An int32 representing the total number of non-mature plants.
     */
    int32 GetTotalNonMaturePlants() const;

    /**
     * @brief Update the growth of the plant.
     */
    void UpdatePlantGrowth(UPlant& Plant, float Percentage = 1.f);

    /**
     * @brief Used to check the growth of a plant and change it mesh based on the growth.
     * @param Plant The plant to check for growth.
     */
    void CheckPlantForGrowth(UPlant& Plant);
    
    // Call to harvest the plant.
    FOnHarvestDelegate OnHarvested;
};
