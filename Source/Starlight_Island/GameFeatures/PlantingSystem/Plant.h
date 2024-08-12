// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Starlight_Island/Enumeration/EPlantState.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Plant.generated.h"

class UItem;
enum class ESeason : uint8;
/**
 * Class that hold data information about a plant and its status.
 */
UCLASS(Blueprintable)
class STARLIGHT_ISLAND_API UPlant : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System", meta=(ClampMin="1",
        ToolTip="The unique ID of the plant for plant database."))
    FGameplayTag PlantID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System", meta = (ToolTip = "The name of the plant."))
    FText PlantName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System", meta = (
        ToolTip = "Plant Mesh for stage. 1 = Dead, 2 = Seed, 3 = Stapling, 4 = Mature, etc."))
    TArray<UStaticMesh*> PlantStageMeshes;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System", meta=(ClampMin="1",
        ToolTip="The ID of the item that will be used to plant the seed if broken before maturing."))
    FGameplayTag SeedItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System", meta=(ClampMin="1",
        ToolTip="The ID of the item that will be used to harvest the plant."))
    FGameplayTag MatureItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System", meta=(ClampMin="1",
        ToolTip="Used to if the plant is in a specific state (dead)."))
    FGameplayTag PlantFiberItemID;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Plant System", meta=(ClampMin="1", EditCondition="false",
        ToolTip="It will use the item description instead."))
    EPlantState PlantState;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System|Seasonal", meta=(AllowPrivateAccess="true",
        ToolTip="Season in which the plant grows the best."))
    ESeason InSeasonPlant;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System|Seasonal", meta=(AllowPrivateAccess="true",
        ToolTip="Is the crop multi-seasonal?"))
    bool bIsMultiSeasonal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="plant System", meta=(ClampMin="1",
        ToolTip="The amount of time that it take for a plant to expire (minutes). 64 minutes is a day in the game."))
    float ExpireTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="plant System", meta=(ClampMin="1",
        ToolTip="The amount of time that it take for a plant to expire (minutes). 64 minutes is a day in the game."))
    float PreserveExpireTime;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Plant System|Growth", meta=(AllowPrivateAccess="true",
        Tooltip = "Used when the plant doesn't get enough water for its growth.", ClampMin="-1.0", ClampMax="0.0"))
    float PlantDecayRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Plant System|Growth", meta=(AllowPrivateAccess="true",
        Tooltip = "Growth rate of the plant under normal circumstances per in-game hour.", ClampMin="1.0"))
    float PlantGrowthRate;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Plant System|Growth", meta=(AllowPrivateAccess="true",
        ToolTip = "How much the of water the plant will use per hour.", ClampMin="1.0"))
    float PlantWaterRate;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Plant System|Growth", meta=(AllowPrivateAccess="true",
        Tooltip = "The maximum growth the plant can reach before it is considered mature.", ClampMin="1.0"))
    float PlantMaxGrowth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant System|Growth", meta=(AllowPrivateAccess="true",
    Tooltip = "The current amount of growth the plant has."))
    float PlantCurrentGrowth = 0.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant System|Static Set", meta=(AllowPrivateAccess="true"))
    bool bIsMature = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant System|Static Set", meta=(AllowPrivateAccess="true"))
    UItem* StoreSeedItem;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant System|Static Set", meta=(AllowPrivateAccess="true"))
    int32 PlantIndex;

    // The amount of days the plant has to die and grow.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant System|Debug", meta=(AllowPrivateAccess="true"))
    int32 DaysToDieAndGrow = 4;

    // The amount of days the plant has not been watered.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant System|Debug", meta=(AllowPrivateAccess="true"))
    int32 DaysWithoutWater = 0;

    // If the plant reach 4/4 half water then it will add a day to die.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant System|Debug", meta=(AllowPrivateAccess="true"))
    int32 AdditionalDaysToDie = 0;

    // The current growth stage of the plant used for indexing stage meshes.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant System|Debug", meta=(AllowPrivateAccess="true"))
    int32 GrowthStage = 1;
};
