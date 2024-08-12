// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "Item.generated.h"

enum class EItemType : uint8;

/**
 * Item object that take in information to create an instance version of it in the game world.
 */
UCLASS(BlueprintType)
class STARLIGHT_ISLAND_API UItem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item, meta=(ClampMin=1))
	FGameplayTag ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item)
	FText ItemName = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item)
	FText ItemDescription = FText::GetEmpty();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item)
    EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item)
	bool bIsUnique = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item)
	UTexture2D* ItemIcon = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item)
	UStaticMesh* ItemMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Stack")
	bool bAllowStacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Stack", meta=(EditCondition="bAllowStacking", ClampMin=1, ClampMax=1000))
	int32 TotalQuantity = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item|Stack", meta=(ClampMin = 0))
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemActor, meta=(
	ToolTip = "The speed in which the item will follow the player."))
	float InterpSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemActor, meta=(
		ToolTip = "The timer to start following the player."))
	float TimerToStartFollow = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemActor, meta=(
		ToolTip = "The timer that increase the speed of the follow."))
	float TimerToIncreaseInterpSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemActor, meta=(
		ToolTip = "How much the speed increase based on the timer to increase interp speed."))
	float InterpSpeedIncrease = 0.f;

	FORCEINLINE bool operator==(const UItem* Other) const
	{
		return ItemID == Other->ItemID;
	}

	/**
	 * Used to copy an item for the purpose of creating a new item and not modifying the original item.
	 * @param InItem Item to copy.
	 * @param InOwner The owner of the item.
	 * @return New item that is a copy of the input item.
	 */
	static UItem* Copy(const UItem* InItem, UObject* InOwner)
	{
		UItem* NewItem = NewObject<UItem>(InOwner, UItem::StaticClass());
		NewItem->ItemID = InItem->ItemID;
		NewItem->ItemName = InItem->ItemName;
		NewItem->ItemDescription = InItem->ItemDescription;
		NewItem->bIsUnique = InItem->bIsUnique;
		NewItem->ItemIcon = InItem->ItemIcon;
		NewItem->ItemMesh = InItem->ItemMesh;
		NewItem->bAllowStacking = InItem->bAllowStacking;
		NewItem->TotalQuantity = InItem->TotalQuantity;
		NewItem->Quantity = InItem->Quantity;
		NewItem->InterpSpeed = InItem->InterpSpeed;
		NewItem->TimerToStartFollow = InItem->TimerToStartFollow;
		NewItem->TimerToIncreaseInterpSpeed = InItem->TimerToIncreaseInterpSpeed;
		NewItem->InterpSpeedIncrease = InItem->InterpSpeedIncrease;
		return NewItem;
	}
};
