// Copyrighted Celestial Games. All Rights Reserved.


#include "StarlightGameState.h"
#include "Starlight_Island/Manager/TimeManager.h"
#include "Starlight_Island/Manager/UltraDynamicWeatherHandler.h"

AStarlightGameState::AStarlightGameState()
{
	TimeManager = nullptr;
	UltraDynamicWeatherHandler = nullptr;
	
	static ConstructorHelpers::FObjectFinder<UBlueprint> TimeManagerBP(
		TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Manager/BP_TimeManager.BP_TimeManager'"));
	if(TimeManagerBP.Succeeded())
	{
		BP_TimeManager = (UClass*)TimeManagerBP.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> UltraDynamicWeatherHandlerBP(
		TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Manager/BP_UltraDynamicWeatherHandler.BP_UltraDynamicWeatherHandler'"));
	if(UltraDynamicWeatherHandlerBP.Succeeded())
	{
		BP_UltraDynamicWeatherHandler = (UClass*)UltraDynamicWeatherHandlerBP.Object->GeneratedClass;
	}
}

void AStarlightGameState::BeginPlay()
{
	Super::BeginPlay();
	
	if(GetWorld())
	{
		// Spawn in Time Manager
		TimeManager = GetWorld()->SpawnActor<ATimeManager>(BP_TimeManager,
			FVector::ZeroVector, FRotator::ZeroRotator);
		TimeManager->SetFolderPath("Manager");
		TimeManager->SetActorLocation(FVector(0.f, 0.f, 390.0f));

		// Spawn in Ultra Dynamic Weather Handler
		UltraDynamicWeatherHandler = GetWorld()->SpawnActor<AUltraDynamicWeatherHandler>(BP_UltraDynamicWeatherHandler,
			FVector::ZeroVector, FRotator::ZeroRotator);
		UltraDynamicWeatherHandler->SetFolderPath("Manager");
		UltraDynamicWeatherHandler->SetActorLocation(FVector(0.f, 0.f, 300.f));
	}
}