// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FPlantList.generated.h"

enum class ESeason : uint8;
enum class EPlantState : uint8;

/**
 * DataTable for the database of plants in the game.
 */
USTRUCT(BlueprintType)
struct FPlantList : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System", meta=(ClampMin="1", ToolTip="The unique ID of the plant for plant database."))
    FGameplayTag PlantID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System", meta=( ToolTip="The name of the plant."))
    FText PlantName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plant System", meta=(ToolTip=
        "Different mesh stages of the plant."))
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
        Tooltip = "Used when the plant doesn't get enough water for its growth.", ClampMin="-10.0", ClampMax="0.0"))
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

    FPlantList();
};
