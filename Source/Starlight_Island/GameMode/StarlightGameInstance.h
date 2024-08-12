// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "StarlightGameInstance.generated.h"

class UItemFactory;
class UPlantFactory;

/**
 * 
 */
UCLASS()
class STARLIGHT_ISLAND_API UStarlightGameInstance : public UGameInstance
{
	GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=ItemFactory, meta=(AllowPrivateAccess = "true"))
    UItemFactory* ItemFactory;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=PlantFactory, meta=(AllowPrivateAccess = "true"))
    UPlantFactory* PlantFactory;
    
public:
    UStarlightGameInstance();

    virtual void Init() override;

    /**
     * Used to get the Item Factory.
     * @return Returns the Item Factory.
     */
    UFUNCTION(BlueprintCallable, Category="GameInstance")
    UItemFactory* GetItemFactory() const { return ItemFactory; }

    UFUNCTION(BlueprintCallable, Category="GameInstance")
    UPlantFactory* GetPlantFactory() const { return PlantFactory; }
};
