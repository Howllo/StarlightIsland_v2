// Copyrighted Celestial Games. All Rights Reserved.


#include "TemperatureComponent.h"
#include "PlayerCharacter.h"
#include "Starlight_Island/Enumeration/EPlayerState.h"
#include "Starlight_Island/GameMode/PlayerCharState.h"
#include "Starlight_Island/GameMode/StarlightGameState.h"
#include "Starlight_Island/Manager/UltraDynamicWeatherHandler.h"


// Sets default values for this component's properties
UTemperatureComponent::UTemperatureComponent()
{
    CurrentTemperature = 0.f;
    GameState = nullptr;
    PlayerGameState = EPlayerGameState::Idle;
    PlayerCharacter = nullptr;
    StatsComponent = nullptr;
}

void UTemperatureComponent::SetArtificialTemperature(const double Temperature)
{
    if(PlayerGameState == EPlayerGameState::Temperature)
    {
        CurrentTemperature = Temperature;
    }
}

// Called when the game starts
void UTemperatureComponent::BeginPlay()
{
    Super::BeginPlay();

    // Get the Game Instance
    if(GameState = Cast<AStarlightGameState>(GetWorld()->GetGameState()); GameState != nullptr)
    {
        // Set the temperature of the game instance.
        GameState->GetUltraDynamicWeatherHandler()->OnTemperatureChanged.AddUObject(this,
            &UTemperatureComponent::SetAmbientTemperature);
    }

    if(GetOwner()->IsA(APlayerCharacter::StaticClass()))
    {
        PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
        StatsComponent = Cast<APlayerCharState>(PlayerCharacter->GetPlayerState())->GetStatsComponent();
    }
}

void UTemperatureComponent::OnRegister()
{
    Super::OnRegister();
}

void UTemperatureComponent::SetAmbientTemperature(const double Temperature)
{
    if(PlayerGameState != EPlayerGameState::Temperature)
    {
        CurrentTemperature = Temperature;
    }
}
