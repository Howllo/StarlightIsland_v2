// Copyrighted Celestial Games. All Rights Reserved.


#include "StatsComponent.h"


// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set default health
	StartingHealth = 300.f;
	CalculateAndSetPlayerHealth(1);
	CurrentHealth = TotalHealth;
}


// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UStatsComponent::CalculateAndSetPlayerHealth(int32 Level)
{
	if(Level >= 1 && Level <= 25)
	{	// 25 = 700
		TotalHealth = FMath::Pow(StartingHealth + ((Level - 1.f) * 10.f) / 4.42f, 1.5f);
	} else if(Level >= 26 && Level <= 50)
	{	// 50 = 1600
		TotalHealth = FMath::Pow(StartingHealth + ((Level - 1.f) * 10.f) / 4.113f, 1.5f);
	} else if(Level >= 51 && Level <= 70)
	{	// 70 = 1800
		TotalHealth = FMath::Pow(1450 + (1-(1-(Level - 25)*16)/3.5), 1.1f);
	} else if(Level >= 71 && Level <= 99)
	{
		TotalHealth = FMath::Pow(1800 + (1-(1-(Level - 50)*11)/6.5), 1.2f);
	}
}