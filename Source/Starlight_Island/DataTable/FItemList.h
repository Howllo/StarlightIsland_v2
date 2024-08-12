// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FItemList.generated.h"


enum class EItemType : uint8;

/**
 * DataTable for the database of item in the game.
 */
USTRUCT(BlueprintType)
struct FItemList : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item, meta=(ClampMin="1"))
	FGameplayTag ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item, meta=(Tooltip="Item name to display in the inventory."))
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item, meta=(
		Tooltip="Item description to display in the inventory."))
	FText ItemDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item, meta=(ToolTip="The type of item."))
    EItemType ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item, meta=(ToolTip="If the item is unique or not."))
	bool bIsUnique = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item, meta=(ToolTip="The icon to display in the inventory."))
	UTexture2D* ItemIcon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item, meta=(ToolTip="The mesh to display in the world."))
	UStaticMesh* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item", meta =(EditCondition = "!bIsUnique",
		Tooltip="If the item can stack."))
	bool bAllowStacking;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Stack", meta=(ClampMin=1, ClampMax=1000,
		EditCondition = "bAllowStacking", Tooltip="The total quantity of the item stack that is allowed."))
	int32 TotalQuantity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item|Stack", meta=(ClampMin=1, ClampMax=1000,
		Tooltip="The current quantity of the item stack."))
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item", meta=(
		ToolTip = "Change the actors speed to the player, and speed up if detected to be following the player."))
	bool bChangeActorSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Item Actor", meta=(
		ToolTip = "The speed in which the item will follow the player.", EditCondition = "bChangeActorSpeed"))
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Item Actor", meta=(
		ToolTip = "The timer to start following the player.", EditCondition = "bChangeActorSpeed"))
	float TimerToStartFollow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Item Actor", meta=(
		ToolTip = "The timer that increase the speed of the follow.", EditCondition = "bChangeActorSpeed"))
	float TimerToIncreaseInterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Item Actor", meta=(
		ToolTip = "How much the speed increase based on the timer to increase interp speed.",
		EditCondition = "bChangeActorSpeed"))
	float InterpSpeedIncrease;;

	FItemList();
};