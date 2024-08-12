// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UltraDynamicWeatherHandler.generated.h"

class ATimeManager;
enum class ESeason : uint8;
enum class EWeatherEvents : uint8;

DECLARE_EVENT_OneParam(AUltraDynamicWeatherHandler, FOnWeatherChanged, EWeatherEvents);
DECLARE_EVENT_OneParam(AUltraDynamicWeatherHandler, FOnTempatureChanged, double);
DECLARE_EVENT(AUltraDynamicWeatherHandler, FNoLongerRaining);

UCLASS(BlueprintType)
class STARLIGHT_ISLAND_API AUltraDynamicWeatherHandler : public AActor
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weather", meta = (AllowPrivateAccess = "true"))
    double CurrentTemperature;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weather", meta=(AllowPrivateAccess="true"))
    EWeatherEvents CurrentWeather;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weather", meta=(AllowPrivateAccess="true"))
    EWeatherEvents PreviousWeather;

    UPROPERTY(VisibleAnywhere, Category="Weather|Season", meta=(
        ToolTip="The season value that will be used to calculate the current season."))
    double SeasonValue;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weather|Season", meta=(AllowPrivateAccess="true"))
    ESeason Season;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather|Rainfall", meta=(AllowPrivateAccess="true",
        ToolTip="The average rainfall for each type of weather event. 1 = Light Rain, 2 = Rain, 3 = Heavy Rain, 4 = Thunderstorm."))
    TArray<float> AverageRainfall;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather|Rainfall", meta=(AllowPrivateAccess="true",
    ToolTip="Scaling amount of rainfall based."))
    float RainfallScaling;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weather|Humidity", meta=(AllowPrivateAccess="true",
        ToolTip="The Humidity value from the weather event."))
    float Humidity;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weather|Humidity", meta=(AllowPrivateAccess="true",
        ToolTip="Rolling Humidity Average."))
    float AvgHumidity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather|Humidity", meta=(AllowPrivateAccess="true",
        ToolTip="The base level of Humidity for each season. 1 = Spring, 2 = Summer, 3 = Autumn, 4 = Winter."))
    TArray<FVector2D> BaseLevelHumidity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather|Humidity", meta=(AllowPrivateAccess="true",
        ToolTip="The deviation of humaidity."))
    TMap<EWeatherEvents, FVector2D> DeviationHumidity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather|Humidity", meta=(AllowPrivateAccess="true",
        ToolTip="Phase shift  the wave left or right."))
    float PhaseShift;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather|Humidity", meta=(AllowPrivateAccess="true",
        ToolTip="Humaidity increases during the night, but how much should it increase?"))
    FVector2D NightDifference;
    
    // Used for the humidity averaging.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weather|Debug", meta=(AllowPrivateAccess="true"))
    TArray<float> HumidityValues;

    // Cache Time Manager
    UPROPERTY(VisibleAnywhere, Category="Weather|Debug", meta=(AllowPrivateAccess="true"))
    ATimeManager* TimeManager;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weather|Debug", meta=(AllowPrivateAccess="true"))
    float LastRainfall;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weather|Debug", meta=(AllowPrivateAccess="true"))
    float PreviousTemperature;

    // Cached Total Days Per Year
    uint16 TotalHourPerYear;

public:
    // Sets default values for this actor's properties
    AUltraDynamicWeatherHandler();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /**
     * @brief Calculate the season based on the season double value.
     */
    void GetSwitchSeason();

    /**
     * @brief Calculate the Humidity based on the current temperature and previous event.
     */
    void CalculateTheHumidity();

    /**
     * @brief Calculate the deviation of Humidity based on the current weather event.
     */
    void RollingAvgHumidity();

    /**
     * @brief Calculate the average rainfall based on the current weather event.
     * @return The average rainfall for the current weather event.
     */
    float GetSwitchAverageRainfall();

    /**
     * @brief Calculate the base level of Humidity based on the current season.
     * @return The base level of Humidity for the current season.
     */
    FVector2D GetSwitchBaseLevelHumidity();

    void CalculateSeason();
public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /**
     * @brief Get the current temperature.
     * @return The current temperature.
     */
    UFUNCTION(BlueprintCallable, Category="Weather")
    FORCEINLINE double GetTemperature() const { return CurrentTemperature; }

    /**
     * @brief Get the current season.
     * @return The current season.
     */
    UFUNCTION(BlueprintCallable, Category="Weather")
    FORCEINLINE ESeason GetSeason() const { return Season; }

    /**
     * @brief Get the current weather.
     * @return The current weather event.
     */
    UFUNCTION(BlueprintCallable, Category="Weather")
    FORCEINLINE EWeatherEvents GetWeather() const { return CurrentWeather; }

    /**
     * @brief Set the current weather.
     * @param WeatherEnum The weather event to set.
     */
    UFUNCTION(BlueprintCallable, Category="Weather")
    FORCEINLINE void SetWeather(EWeatherEvents WeatherEnum);

    /**
     * @brief Check if the weather event is the same.
     * @param WeatherEnum The weather event to check.
     * @return True if the weather event is the same.
     */
    UFUNCTION(BlueprintCallable, Category="Weather")
    bool CheckIfWeatherEventIsSame(const EWeatherEvents WeatherEnum) const;

    /**
     * @brief Used to get the rainfall value.
     * 
     * @return The rainfall value.
     */
    UFUNCTION(BlueprintCallable, Category="Weather")
    float GetRainfall();

    /**
     * @brief Get the humidity value.
     * @return The humidity value.
     */
    UFUNCTION(BlueprintCallable, Category="Weather")
    FORCEINLINE float GetHumidity() const { return Humidity; }

    /**
     * @brief  Used to get the current seasonal value on a scale of 0.0 to 4.0.
     * @return The season value.
     */
    UFUNCTION(BlueprintCallable, Category="Weather")
    FORCEINLINE double GetSeasonValue();

    UFUNCTION(BlueprintCallable, Category="Weather")
    void SetTemperature(const double InTemperature);

    // Weather change event
    FOnWeatherChanged OnWeatherChanged;

    // Event handler for no longer raining.
    FNoLongerRaining OnNoLongerRaining;

    // Event for when the temperature changes.
    FOnTempatureChanged OnTemperatureChanged;
};
