// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "PlantFactory.generated.h"

class UPlantSlot;
class UDataTable;
class UPlant;

/**
 * Handles the spawning of all Plant objects and PlantSlot actors.
 */
UCLASS()
class STARLIGHT_ISLAND_API UPlantFactory : public UObject
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant Factory", meta=(AllowPrivateAccess="true"))
    UDataTable* PlantDatabase;
    
public:
    UPlantFactory();

    /**
     * @brief Create a new plant object to prevent rewriting the same code.
     * @param SeedItemID Take in the seed ID of the item to look up the data in the database.
     * @return Return a new plant object.
     */
    UPlant* CreatePlant(const FGameplayTag SeedItemID) const;

    /**
     * Use to create a new plant actor to prevent rewriting the same code.
     * @param Owner The owner of the plant actor
     * @param InVector The vector to spawn the actor at.
     * @param InRotator The rotator to spawn the actor with.
     * @param SeedItemID The seed ID of the item to look up the data in the database.
     * @return Return a new plant actor.
     */
    UPlantSlot* CreatePlantSlot(UObject* Owner, const FVector& InVector, const FRotator& InRotator,
        const FGameplayTag SeedItemID) const;
};
