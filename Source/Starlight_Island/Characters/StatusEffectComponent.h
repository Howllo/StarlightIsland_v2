// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusEffectComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARLIGHT_ISLAND_API UStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

	// Types of Status Effects:
	// Blood Lost - Full: Lose 10% HP, reset the blood lost.
	// Poison - Full: Higher trick damage and more rapid damn.
	// Darkness - Full: Instant Death.
	// Frostbite - Full: Slowness for movement, attacks and lower FP.
	// Fiery - 	Full: Lower defense and increases tick damage.
	// Rot - Full: Reduces the amount of HP character has  slowly regains it overtime.
	// Sleep - Full: Put player to sleep for duration. 

	
public:
	// Sets default values for this component's properties
	UStatusEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
