// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerCharState.generated.h"

class UInventoryComponent;
class UStatsComponent;

/**
 * Player state hold information that will be replicated to other players.
 */
UCLASS()
class STARLIGHT_ISLAND_API APlayerCharState : public APlayerState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Player, meta = (AllowPrivateAccess = "true"))
	FName PlayerName;
	
	/* Inventory System */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Inventory", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player|Stats", meta=(AllowPrivateAccess = "true"))
	UStatsComponent* Stats;
public:
	APlayerCharState();
	
	/** Returns the inventory of the player character. **/
	FORCEINLINE UInventoryComponent* GetInventory() const { return Inventory; }

	/** Get the player in-game name. **/
	FORCEINLINE FName GetPlayerGameName() const { return PlayerName; };

	/* Get the stat component. */
	FORCEINLINE UStatsComponent* GetStatsComponent() const { return Stats; }
};
