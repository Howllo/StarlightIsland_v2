// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplaySaveSystem.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGameplaySaveSystem : public UInterface
{
    GENERATED_BODY()
};

/**
 * Save system that will be called when the game data needs to be saved or loaded.
 */
class STARLIGHT_ISLAND_API IGameplaySaveSystem
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * @brief Save the game data.
     */
    UFUNCTION()
    virtual void SaveGame(FString& Version, FString& Slot) = 0;

    /**
     * @brief Called when the save game is complete.
     */
    UFUNCTION()
    virtual void SaveGameComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess) = 0;
    
    /**
     * @brief Load the game data.
     */
    UFUNCTION()
    virtual void LoadGame(FString& Version, FString& Slot) = 0;

    /**
     * @brief Called when the load game is complete.
     */
    UFUNCTION()
    virtual void LoadGameComplete(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData) = 0;
};
