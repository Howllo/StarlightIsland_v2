
// Copyrighted Celestial Games. All Rights Reserved.


#include "UltraDynamicWeatherHandler.h"
#include "TimeManager.h"
#include "Starlight_Island/Enumeration/ESeason.h"
#include "Starlight_Island/Enumeration/EWeatherEvents.h"
#include "Starlight_Island/GameMode/StarlightGameState.h"


// Sets default values
AUltraDynamicWeatherHandler::AUltraDynamicWeatherHandler()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    CurrentTemperature = 0.f;
    SeasonValue = 0.f;
    Season = ESeason::Spring;
    CurrentWeather = EWeatherEvents::LightRain;
    PreviousWeather = EWeatherEvents::LightRain;
    RainfallScaling = 10.f;
    Humidity = 0.f;
    AvgHumidity = 0.f;
    TimeManager = nullptr;
    AverageRainfall.Init(0.f, 4);
    BaseLevelHumidity.Init(FVector2D(0.f, 0.f), 4);
    DeviationHumidity.Empty();
    PhaseShift = 0.f;
    NightDifference = FVector2D(-5.f, 5.f);
    LastRainfall = 0.f;
    PreviousTemperature = 0.f;
    TotalHourPerYear = 0;
}

void AUltraDynamicWeatherHandler::BeginPlay()
{
    Super::BeginPlay();
    TimeManager = Cast<AStarlightGameState>(GetWorld()->GetGameState())->GetTimeManager();
    TimeManager->OnHourUpdate.AddUObject(this, &AUltraDynamicWeatherHandler::CalculateTheHumidity);
    TimeManager->OnHourUpdate.AddUObject(this, &AUltraDynamicWeatherHandler::CalculateSeason);
    CalculateTheHumidity();
    TotalHourPerYear = TimeManager->GetDaysPerMonth() * TimeManager->GetMonthsPerYear() * 24;
}

void AUltraDynamicWeatherHandler::GetSwitchSeason()
{
    if(SeasonValue >= 0.0f && SeasonValue <= 0.99f)
    {
        Season = ESeason::Spring;
    }
    else if(SeasonValue >= 1.0f && SeasonValue <= 1.99f)
    {
        Season = ESeason::Summer;
    }
    else if(SeasonValue >= 2.0f && SeasonValue <= 2.99f)
    {
        Season = ESeason::Autumn;
    }
    else if (SeasonValue >= 3.0f && SeasonValue <= 3.99f)
    {
        Season = ESeason::Winter;
    }
    else
    {
        Season = ESeason::Spring;
    }
}

void AUltraDynamicWeatherHandler::CalculateTheHumidity()
{
    const FVector2D BaseLevel = GetSwitchBaseLevelHumidity();
    float BaseHumidity;
    
    if(AvgHumidity > 0.0f )
    {
        BaseHumidity = (BaseLevel.Y - BaseLevel.X) * FMath::Sin( ((2 * PI)/3600) * TimeManager->GetTimeInMinutes()
            + PhaseShift) + AvgHumidity;
    }
    else
    {
        BaseHumidity = (BaseLevel.Y - BaseLevel.X) * FMath::Sin( ((2 * PI)/3600) * TimeManager->GetTimeInMinutes()
            + PhaseShift) + BaseLevel.X;
    }

    // Calculate the deviation.
    if(DeviationHumidity.Contains(CurrentWeather))
    {
        Humidity = BaseHumidity + FMath::RandRange(DeviationHumidity[CurrentWeather].X,
        DeviationHumidity[CurrentWeather].Y);
    }
    else
    {
        Humidity = BaseHumidity + 0.f;
    }

    if(!TimeManager->GetIsItDaytime())
    {
        Humidity += FMath::RandRange(NightDifference.X, NightDifference.Y);
    }

    // Set min and max to 0 to 100.
    Humidity = FMath::Max(0, FMath::Min(100, Humidity));

    // Pseudo queue for the humidity values.
    if(HumidityValues.Num() >= 10)
    {
        HumidityValues.RemoveAt(0);
    }
    HumidityValues.Add(Humidity);
    
    // Get the Humidity Average
    RollingAvgHumidity();
}

void AUltraDynamicWeatherHandler::RollingAvgHumidity()
{
    if(HumidityValues.Num() == 0) return;
    
    float Holder = 0.f;
    
    for(const float Value : HumidityValues)
    {
        Holder += Value;
    }

    AvgHumidity = Holder / HumidityValues.Num();
}

float AUltraDynamicWeatherHandler::GetSwitchAverageRainfall()
{
    switch(CurrentWeather)
    {
        case EWeatherEvents::LightRain:
            return AverageRainfall[0];
        case EWeatherEvents::Rain:
            return AverageRainfall[1];
        case EWeatherEvents::HeavyRain:
            return AverageRainfall[2];
        case EWeatherEvents::Thunderstorm:
            return AverageRainfall[3];
        default:
            return 0.0f;
    }
}

FVector2D AUltraDynamicWeatherHandler::GetSwitchBaseLevelHumidity()
{
    switch(Season)
    {
        case ESeason::Spring:
            return BaseLevelHumidity[0];
        case ESeason::Summer:
            return BaseLevelHumidity[1];
        case ESeason::Autumn:
            return BaseLevelHumidity[2];
        case ESeason::Winter:
            return BaseLevelHumidity[3];
        default:
            return BaseLevelHumidity[0];
    }
}

void AUltraDynamicWeatherHandler::CalculateSeason()
{
    if(!TimeManager) return;
    
    const uint16 CurrentHourOfYear = (TimeManager->GetCurrentMonth() - 1) * TimeManager->GetDaysPerMonth() +
        (TimeManager->GetCurrentDay() - 1) * 24 + TimeManager->GetCurrentTwentyFourHour();

    const float NormalizedDay = static_cast<float>(CurrentHourOfYear) / static_cast<float>(TotalHourPerYear);
    
    SeasonValue = NormalizedDay * 4.f;
}

void AUltraDynamicWeatherHandler::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AUltraDynamicWeatherHandler::SetWeather(const EWeatherEvents WeatherEnum)
{
    PreviousWeather = CurrentWeather;
    CurrentWeather = WeatherEnum;
    OnWeatherChanged.Broadcast(WeatherEnum);

    if(CurrentWeather > EWeatherEvents::Thunderstorm)
    {
        OnNoLongerRaining.Broadcast();
    }
}

bool AUltraDynamicWeatherHandler::CheckIfWeatherEventIsSame(const EWeatherEvents WeatherEnum) const
{
    return CurrentWeather != WeatherEnum;
}

float AUltraDynamicWeatherHandler::GetRainfall()
{
    if(CurrentWeather >= EWeatherEvents::LightRain && CurrentWeather <= EWeatherEvents::Thunderstorm)
    {
        const float Rainfall = GetSwitchAverageRainfall() * RainfallScaling;
        LastRainfall += Rainfall;
        return Rainfall;
    }

    LastRainfall = 0.f;
    return 0.f;
}

double AUltraDynamicWeatherHandler::GetSeasonValue()
{
    GetSwitchSeason();
    return SeasonValue;
}

void AUltraDynamicWeatherHandler::SetTemperature(const double InTemperature)
{
    CurrentTemperature = InTemperature;
    if(FMath::FloorToDouble(CurrentTemperature) != PreviousTemperature)
    {
        OnTemperatureChanged.Broadcast(CurrentTemperature);
        PreviousTemperature = FMath::FloorToDouble(CurrentTemperature);
    }
}
