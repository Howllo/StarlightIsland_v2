// Copyrighted Celestial Games. All Rights Reserved.


#include "TimeManager.h"
#include "Starlight_Island/Enumeration/ECurrentDay.h"
#include "Starlight_Island/Enumeration/ETime.h"


// Sets default values
ATimeManager::ATimeManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Dynamic
    UltraTimeOfDay = 1.0f;
    CurrentTime = 1.0f;
    CurrentTwentyFourHour = 0;
    CurrentTwelveHour = 0;
    CurrentMinute = 0;
    TimeFormat = ETime::TwelveHour;
    bIsTimeAM = true;
    bIsItDaytime = true;

    // Calendar
    Year = 1;
    Month = 1;
    Day = 1;
    
    // Day
    CurrentDay = 1;
    CurrentDayEnum = ECurrentDay::Monday;

    // Static
    TotalUltraTime = 2400;
    UltraDawnTime = 1;
    UltraDuskTime = 1;
    DayTime = 1;
    NightTime = 1;
    TotalDayNightTime = 1;
    DaysPerMonth = 28;
    MonthsPerYear = 4;
    
    // Debug
    bIsNewDay = false;
    DayTracking = 1;
    PreviousHour = 0;
    PreviousRealMinute = 0;
}

void ATimeManager::SetTimes(const float InDay, const float Night, const float Dawn, const float Dusk)
{
    DayTime = InDay;
    NightTime = Night;
    TotalDayNightTime = DayTime + NightTime;
    UltraDawnTime = Dawn;
    UltraDuskTime = Dusk;
}

void ATimeManager::CalculateRealTime(const float InTimeOfDay)
{
    UltraTimeOfDay = InTimeOfDay;
    CurrentTime = UltraTimeOfDay * (TotalDayNightTime / TotalUltraTime);

    // Broadcast for each Real Minute increase.
    if(FMath::FloorToInt(CurrentTime) != PreviousRealMinute)
    {
        OnRealMinuteUpdate.Broadcast();
        PreviousRealMinute = StaticCast<int32>(FMath::FloorToFloat(CurrentTime));
    }
    
    OnCurrentTime.Broadcast(CurrentTime);
    ConvertToHourAndMinute();
    UpdateDay();
}

void ATimeManager::ConvertToHourAndMinute()
{
    PreviousHour = CurrentTwentyFourHour;
    const float HoursFloat = (CurrentTime / TotalDayNightTime) * 24;
    CurrentTwentyFourHour = FMath::FloorToFloat(HoursFloat);
    CurrentMinute = FMath::FloorToFloat((HoursFloat - CurrentTwentyFourHour) * 60);

    if(PreviousHour != CurrentTwentyFourHour)
    {
        OnHourUpdate.Broadcast();
        ConvertToTwelveHour();
    }
}

float ATimeManager::CalculatedFutureTime(const float Timer) const
{
    float FutureTime = CurrentTime + Timer;

    if(FutureTime > TotalDayNightTime)
    {
        FutureTime =  FutureTime - TotalDayNightTime;
    }
    
    return FutureTime;
}

void ATimeManager::SetIsItDaytime(const bool bIsDay)
{
    if(bIsItDaytime == bIsDay) { return; }
    
    bIsItDaytime = bIsDay;
    OnDaytimeChange.Broadcast(bIsItDaytime);
}

// Called when the game starts or when spawned
void ATimeManager::BeginPlay()
{
    Super::BeginPlay();
}

void ATimeManager::ConvertToTwelveHour()
{
    if(TimeFormat != ETime::TwelveHour) { return; }
    
    if(CurrentTwentyFourHour >= 12 && CurrentTwentyFourHour < 24)
    {
        bIsTimeAM = false;
        if(CurrentTwentyFourHour > 12)
        {
            CurrentTwelveHour = CurrentTwentyFourHour - 12;
        }
        else
        {
            CurrentTwelveHour = CurrentTwentyFourHour;
        }
    }
    else
    {
        bIsTimeAM = true;
        if(CurrentTwentyFourHour >= 0 && CurrentTwentyFourHour < 12)
        {
            CurrentTwelveHour = 12;
        }
        else
        {
            CurrentTwelveHour = CurrentTwentyFourHour;
        }
    }
}

void ATimeManager::UpdateDay()
{
    if(CurrentTime >= 0.f && CurrentTime <= 0.1f && bIsNewDay)
    {
        bIsNewDay = false;
        CurrentDay++;

        if(Day <= 27)
        {
            Day++;
        }
        else
        {
            Day = 1;
            CalculateTheMonth();
        }
        
        CalculateTheDay();
    }
    else if (CurrentTime > 0.2f)
    {
        bIsNewDay = true;
    }
}

void ATimeManager::CalculateTheDay()
{
    switch (DayTracking)
    {
        case 1:
            CurrentDayEnum = ECurrentDay::Monday;
            OnDayTimeChange.Broadcast(CurrentDayEnum);
            DayTracking++;
            break;
        case 2:
            CurrentDayEnum = ECurrentDay::Tuesday;
            OnDayTimeChange.Broadcast(CurrentDayEnum);
            DayTracking++;
            break;
        case 3:
            CurrentDayEnum = ECurrentDay::Wednesday;
            OnDayTimeChange.Broadcast(CurrentDayEnum);
            DayTracking++;
            break;
        case 4:
            CurrentDayEnum = ECurrentDay::Thursday;
            OnDayTimeChange.Broadcast(CurrentDayEnum);
            DayTracking++;
            break;
        case 5:
            CurrentDayEnum = ECurrentDay::Friday;
            OnDayTimeChange.Broadcast(CurrentDayEnum);
            DayTracking++;
            break;
        case 6:
            CurrentDayEnum = ECurrentDay::Saturday;
            OnDayTimeChange.Broadcast(CurrentDayEnum);
            DayTracking++;
            break;
        case 7:
            CurrentDayEnum = ECurrentDay::Sunday;
            OnDayTimeChange.Broadcast(CurrentDayEnum);
            DayTracking = 1;
            break;
        default:
            CurrentDayEnum = ECurrentDay::Monday;
            OnDayTimeChange.Broadcast(CurrentDayEnum);
            DayTracking = 1;
    }
}

void ATimeManager::CalculateTheMonth()
{
    switch(Month)
    {
        case 1:
            Year++;
            Month++;
            OnMonthChange.Broadcast(Month);
            OnYearChange.Broadcast(Year);
            break;
        case 2:
            Month++;
            OnMonthChange.Broadcast(Month);
            break;
        case 3:
            Month++;
            OnMonthChange.Broadcast(Month);
            break;
        case 4:
            Month++;
            OnMonthChange.Broadcast(Month);
            Month = 1;
            break;
        default:
            Month = 1;
            OnMonthChange.Broadcast(Month);
    }
}
