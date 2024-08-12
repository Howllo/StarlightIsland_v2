// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARLIGHT_ISLAND_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()
	
#pragma region Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health", meta = (AllowPrivateAccess = "true"))
	float StartingHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health", meta = (AllowPrivateAccess = "true"))
	float TotalHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats|Health", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health", meta = (AllowPrivateAccess = "true",
		ToolTip = "The rate at which health regenerates at (in seconds)."))
	float HealthRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health", meta = (AllowPrivateAccess = "true",
		ToolTip = "The delay before health regeneration starts (in seconds)."))
	float HealthRegenDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Health", meta = (AllowPrivateAccess = "true"))
	float HealthRegenAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats|Health", meta = (AllowPrivateAccess = "true"))
	FTimerHandle HealthRegenTimer;
#pragma endregion
	
#pragma region Focus
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Focus", meta = (AllowPrivateAccess = "true"))
	float TotalFocus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats|Health", meta = (AllowPrivateAccess = "true"))
	float CurrentFocus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Focus", meta = (AllowPrivateAccess = "true"))
	float FocusRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Focus", meta = (AllowPrivateAccess = "true"))
	float FocusRegenDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Focus", meta = (AllowPrivateAccess = "true"))
	float FocusRegenAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats|Focus", meta = (AllowPrivateAccess = "true"))
	FTimerHandle FocusRegenTimer;
#pragma endregion

#pragma region Mana
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Focus", meta = (AllowPrivateAccess = "true"))
	float TotalMana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats|Focus", meta = (AllowPrivateAccess = "true"))
	float CurrentMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Focus", meta = (AllowPrivateAccess = "true"))
	float ManaRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Focus", meta = (AllowPrivateAccess = "true"))
	float ManaRegenDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Focus", meta = (AllowPrivateAccess = "true"))
	float ManaRegenAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats|Focus", meta = (AllowPrivateAccess = "true"))
	FTimerHandle ManaRegenTimer;
#pragma endregion 
	
public:
	// Sets default values for this component's properties
	UStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void CalculateAndSetPlayerHealth(int32 Level);
};
