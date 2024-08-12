// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemFactory.generated.h"

struct FGameplayTag;
class UItem;
class AItemActor;

/**
 * Used to create items in the game.
 */
UCLASS()
class STARLIGHT_ISLAND_API UItemFactory : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess = "true"))
	UDataTable* ItemDatabase;
	
public:
	UItemFactory();
	
	/**
	 * Use to centralize the creation of Item objects.
	 * @param ItemID Takes in the Item ID to look up in the Item Database.
	 * @param StartingItemStack Takes in the amount of items in the stack.
	 * @return Returns a new item with the information from the database.
	 */
	UItem* CreateItem(FGameplayTag ItemID, int32 StartingItemStack) const;

	/**
	 * Use to centralize the creation of Item objects.
	 * @param ItemID Takes in the Item ID to look up in the Item Database.
	 * @param StartingItemStack Takes in the amount of items in the stack.
	 * @param InDatabase Takes in the database to look up the item.
	 * @param InOuter Takes in the outer object to create the item.
	 * @return Returns a new item with the information from the database.
	 */
	static UItem* CreateItem(const FGameplayTag ItemID, const int32 StartingItemStack, const UDataTable* InDatabase,
		UObject* InOuter);
	
	/**
	 * Used to centralize the creation of AActor item objects.
	 * @param ItemID Takes in the Item ID to look up in the Item Database.
	 * @param InVector Takes in the location of the actor.
	 * @param InRotator Takes in the rotation of the actor.
	 * @param StartingItemStack Takes in the amount of items in the stack.
	 * @param World Takes in the world to spawn the actor in.
	 * @return Returns a new actor with the information from the database.
	 */
	UFUNCTION()
	AItemActor* CreateItemActor(FGameplayTag ItemID, const FVector& InVector, const FRotator& InRotator,
		int32 StartingItemStack, UWorld* World) const;
	
	/**
	 * Used to set the database for the Item Factory.
	 * @param InDatabase Takes in the database to set.
	 */
	FORCEINLINE void SetDatabase(UDataTable* InDatabase) { ItemDatabase = InDatabase; }
};