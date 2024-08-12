// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TemperatureComponent.generated.h"


enum class EPlayerGameState : uint8;
class AStarlightGameState;
class APlayerCharacter;
class UStatsComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARLIGHT_ISLAND_API UTemperatureComponent : public UActorComponent
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Temperature", meta=(AllowPrivateAccess="true"))
    double CurrentTemperature;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Temperature", meta=(AllowPrivateAccess="true"))
    EPlayerGameState PlayerGameState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Temperature|Debug", meta=(AllowPrivateAccess="true"))
    AStarlightGameState* GameState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Temperature|Debug", meta=(AllowPrivateAccess="true"))
    APlayerCharacter* PlayerCharacter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Temperature|Debug", meta=(AllowPrivateAccess="true"))
    UStatsComponent* StatsComponent;
    
public:
    // Sets default values for this component's properties
    UTemperatureComponent();

    /**
     * @brief Used to set the temperature of the component through artificial means such as a spell, campfire,
     * standing near lava.
     * @param Temperature The temperature to set.
     */
    void SetArtificialTemperature(const double Temperature);
    
protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    // Called when the component is registered.
    virtual void OnRegister() override;
    
    /**
     * @brief Used to set the temperature of the component through ambient means such as the weather.
     * @param Temperature The temperature to set.
     */
    void SetAmbientTemperature(const double Temperature);
};
