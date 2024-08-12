// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Starlight_Island/GameFeatures/PlantingSystem/Plant.h"
#include "FarmingSaveData.generated.h"

/**
 * 
 */
UCLASS()
class STARLIGHT_ISLAND_API UFarmingSaveData : public USaveGame
{
	GENERATED_BODY()
public:
    UFarmingSaveData();
    
    UPROPERTY(VisibleAnywhere, Category="Farming System")
    FVector PlanterLocation;

    UPROPERTY(VisibleAnywhere, Category="Farming System")
    FRotator PlanterRotation;

    UPROPERTY(VisibleAnywhere, Category="Farming System")
    TArray<UPlant*> Plants;
};
