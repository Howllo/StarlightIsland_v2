// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeManager.generated.h"

enum class ECurrentDay : uint8;
enum class ETime : uint8;

// Event Dispatcher
DECLARE_EVENT_OneParam(ATimeManager, FOnCurrentTime, float)
DECLARE_EVENT_OneParam(ATimeManager, FOnDaytimeChange, bool);
DECLARE_EVENT_OneParam(ATimeManager, FOnDayChange, ECurrentDay);
DECLARE_EVENT_OneParam(ATimeManager, FOnMonthChange, int32);
DECLARE_EVENT_OneParam(ATimeManager, FOnYearChange, int32);
DECLARE_EVENT(ATimeManager, FOnHourUpdate);
DECLARE_EVENT(ATimeManager, FOnRealMinuteUpdate);

UCLASS()
class STARLIGHT_ISLAND_API ATimeManager : public AActor
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time", meta=(AllowPrivateAccess="true"))
    float UltraTimeOfDay;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time", meta=(AllowPrivateAccess="true"))
    float CurrentTime;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time", meta=(AllowPrivateAccess="true"))
    int32 CurrentTwentyFourHour;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time", meta=(AllowPrivateAccess="true"))
    int32 CurrentTwelveHour;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time", meta=(AllowPrivateAccess="true"))
    int32 CurrentMinute;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time", meta=(AllowPrivateAccess="true"))
    ETime TimeFormat;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time", meta=(AllowPrivateAccess="true"))
    bool bIsTimeAM;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time", meta=(AllowPrivateAccess="true"))
    bool bIsItDaytime;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time|Calendar", meta=(AllowPrivateAccess="true",
        ToolTip="The amount of days per month."))
    uint8 Year;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time|Calendar", meta=(AllowPrivateAccess="true"))
    int32 Month;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time|Calendar", meta=(AllowPrivateAccess="true"))
    int32 Day;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time|Day", meta=(AllowPrivateAccess="true"))
    int32 CurrentDay;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time|Day", meta=(AllowPrivateAccess="true"))
    ECurrentDay CurrentDayEnum;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time|Static", meta=(AllowPrivateAccess="true"))
    float TotalUltraTime;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time|Static", meta=(AllowPrivateAccess="true"))
    float UltraDawnTime;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time|Static", meta=(AllowPrivateAccess="true"))
    float UltraDuskTime;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time|Static", meta=(AllowPrivateAccess="true"))
    float DayTime;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time|Static", meta=(AllowPrivateAccess="true"))
    float NightTime;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time|Static", meta=(AllowPrivateAccess="true"))
    float TotalDayNightTime;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time|Static", meta=(AllowPrivateAccess="true",
    ToolTip="The amount of days per month."))
    int32 DaysPerMonth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Time|Static", meta=(AllowPrivateAccess="true",
        ToolTip="The amount of months per year."))
    int32 MonthsPerYear;
    
    // Used to prevent the day from updating multiple times.
    UPROPERTY(VisibleAnywhere, Category="Time|Debug")
    bool bIsNewDay;

    // Used to track the day using the KISS principle.
    UPROPERTY(VisibleAnywhere, Category="Time|Debug")
    uint8 DayTracking;
    
    // Used to track the previous hour.
    UPROPERTY(VisibleAnywhere, Category="Time|Debug")
    int32 PreviousHour;

    // Used to track the previous minute
    UPROPERTY(VisibleAnywhere, Category="Time|Debug")
    int32 PreviousRealMinute;
    
public:
    // Sets default values for this actor's properties
    ATimeManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /**
     * @brief Convert the current time to 12-hour format.
     */
    void ConvertToTwelveHour();

    /**
     * @brief Update the current day if it is midnight.
     */
    void UpdateDay();

    /**
     * @brief Calculate the day.
     */
    void CalculateTheDay();

    /**
     * @brief Calculate the month.
     */
    void CalculateTheMonth();
public:
    /**
     * @brief Convert the current time to 24-hour format.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    void ConvertToHourAndMinute();

    /**
     * @brief Set the times for the day, night, dawn, and dusk.
     * @param InDay The time of day for the day.
     * @param Night The time of day for the night.
     * @param Dawn The time of day for the dawn.
     * @param Dusk The time of day for the dusk.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    void SetTimes(const float InDay, const float Night, const float Dawn, const float Dusk);

    /**
     * @brief Calculate the real-time.
     * @param InTimeOfDay The time of day to calculate.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    void CalculateRealTime(const float InTimeOfDay);

    /**
     * @brief Used to get the current ultra time for Ultra Dynamic Sky.
     * @return The current ultra sky time based on the 2400 clock cycle.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    FORCEINLINE float GetUltraTime() const { return UltraTimeOfDay; };
    
    /**
     * @brief Calculate the future time.
     * @param Timer The time to calculate.
     * @return The future time.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    float CalculatedFutureTime(const float Timer) const;

    /**
     * @brief Get the total real time minutes that have gone by (For real time day + night).
     * @return The current float amount of real minutes that has gone by.
     */                       
    UFUNCTION(BlueprintCallable, Category="Time")
    FORCEINLINE float GetCurrentTime() const { return CurrentTime; }

    /**
     * @brief Used to set the is it daytime through the blueprint.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    FORCEINLINE void SetIsItDaytime(const bool bIsDay);

    /**
     * @brief Get the is it daytime.
     * @return If it is daytime.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    FORCEINLINE bool GetIsItDaytime() const { return bIsItDaytime; }
    
    /**
     * @brief Get the current total amount of days that have gone by.
     * @return Returns the total amount of days that have gone by.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    FORCEINLINE int32 GetTotalDay() const { return CurrentDay; }

    /**
     * @brief Get the current time in 24-hour format.
     * @return The current time in 24-hour format.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    ECurrentDay GetCurrentDayEnum() const { return CurrentDayEnum; }

    /**
     * @brief Get the minutes of the in-game time.
     * @return The current time in 24-hour format.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    FORCEINLINE int32 GetTimeInMinutes() const { return CurrentMinute; }

    /**
     * @brief Get the total days per month.
     * @return Returns the total days per month.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    FORCEINLINE int32 GetDaysPerMonth() const { return DaysPerMonth; }

    /**
     * @brief Get the total months per year.
     * @return Returns the total months per year.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    FORCEINLINE int32 GetMonthsPerYear() const { return MonthsPerYear; }

    /**
     * @brief Get the current year.
     * @return The current year.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    FORCEINLINE int32 GetCurrentYear() const { return Year; }

    /**
     * @brief Get the current month.
     * @return The current month.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    FORCEINLINE int32 GetCurrentMonth() const { return Month; }

    /**
     * @brief Get the current day.
     * @return The current day.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    FORCEINLINE int32 GetCurrentDay() const { return Day; }

    /**
     * @brief Get the current time in 12-hour format.
     * @return The current time in 12-hour format.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    FORCEINLINE int32 GetCurrentTwelveHour() const { return CurrentTwelveHour; }

    /**
     * @brief Get the current time in 24-hour format.
     * @return The current time in 24-hour format.
     */
    UFUNCTION(BlueprintCallable, Category="Time")
    FORCEINLINE int32 GetCurrentTwentyFourHour() const { return CurrentTwentyFourHour; }
    
    // Event caller for the current real minute time.
    FOnCurrentTime OnCurrentTime;

    // Event caller for the change of daytime to nighttime, or vice-versa.
    FOnDaytimeChange OnDaytimeChange;

    // Event caller for the day changing.
    FOnDayChange OnDayTimeChange;

    // Event caller for in-game hourly update.
    FOnHourUpdate OnHourUpdate;

    // Event caller for each real life minute.
    FOnRealMinuteUpdate OnRealMinuteUpdate;

    // Event caller for the month changing.
    FOnMonthChange OnMonthChange;

    // Event caller for the year changing.
    FOnYearChange OnYearChange;
};
