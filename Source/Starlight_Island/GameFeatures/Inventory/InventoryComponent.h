// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARLIGHT_ISLAND_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Inventory, meta=(AllowPrivateAccess = "true"))
	TArray<UItem*> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Inventory, meta=(AllowPrivateAccess = "true"))
	int32 TotalInventorySize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Inventory, meta=(AllowPrivateAccess = "true"))
	int32 CurrentInventoryUsage;
	
public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/**
	 * Used to add items to inventory system.
	 * @param InItem Take in an item object to be placed in the inventory.
	 * @return Return a boolean to tell whether it was successful or not.
	 */
	UFUNCTION()
	bool Add(UItem* InItem);

	/**
	 * Used to remove item object from the inventory.
	 * @param InItem Take in an item object to be placed in the inventory.
	 * @return Return a boolean to tell whether it was successful or not.
	 */
	UFUNCTION()
	bool DeleteItem(UItem* InItem);

	/**
	 * Used to remove a specific quantity of item from the inventory.
	 * @param index Take in an index of array to be removed from the inventory.
	 * @param Quantity Take in a quantity to be removed from the item stack.
	 * @return Return a boolean to tell whether it was successful or not.
	 */
	UFUNCTION()
	bool Remove(int32 index, int32 Quantity);

	/**
	 * Get the total slot in the inventory.
	 * @return Returns int32 total slot in the inventory.
	 */
	UFUNCTION()
	FORCEINLINE int32 Max() const { return TotalInventorySize; }
	
	/**
	 * Get the current used inventory size.
	 * @return Returns int32 slot taken in the inventory.
	 */
	UFUNCTION()
	FORCEINLINE int32 Size() const { return CurrentInventoryUsage; }

	/**
	 * Get the inventory array.
	 * @return Returns TArray of UItem*.
	 */
	UFUNCTION()
	FORCEINLINE TArray<UItem*> GetInventory() const { return Inventory; }
	
	/**
	 * Used to check if an inventory is full.
	 * @return Return boolean whether if inventory is full.
	 */
	UFUNCTION()
	FORCEINLINE bool CheckInventoryFull() const { return CurrentInventoryUsage >= TotalInventorySize; }
};
