// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StarlightGameState.generated.h"

class AUltraDynamicWeatherHandler;
class UItemFactory;
class ATimeManager;

/**
 * 
 */
UCLASS()
class STARLIGHT_ISLAND_API AStarlightGameState : public AGameStateBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ItemFactorySubsystem", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ATimeManager> BP_TimeManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ItemFactorySubsystem", meta=(AllowPrivateAccess = "true"))
	ATimeManager* TimeManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ItemFactorySubsystem", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<AUltraDynamicWeatherHandler> BP_UltraDynamicWeatherHandler;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ItemFactorySubsystem", meta=(AllowPrivateAccess = "true"))
	AUltraDynamicWeatherHandler* UltraDynamicWeatherHandler;
protected:
	virtual void BeginPlay() override;
public:
	AStarlightGameState();

	/* Get the TimeManager of the current level. */
	FORCEINLINE ATimeManager* GetTimeManager() const { return TimeManager; }

	/* Get the UltraDynamicWeatherHandler of the current level. */
	FORCEINLINE AUltraDynamicWeatherHandler* GetUltraDynamicWeatherHandler() const { return UltraDynamicWeatherHandler; }
};
