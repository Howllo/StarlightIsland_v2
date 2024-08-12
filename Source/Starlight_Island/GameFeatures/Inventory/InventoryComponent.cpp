// Copyrighted Celestial Games. All Rights Reserved.


#include "InventoryComponent.h"
#include "Item.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set default values
	TotalInventorySize = 20;
	CurrentInventoryUsage = 0;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

bool UInventoryComponent::Add(UItem* InItem)
{
	if(CurrentInventoryUsage >= TotalInventorySize)
	{
		return false;
	}

	for(UItem* Item : Inventory)
	{
		if(Item->ItemID == InItem->ItemID && Item->Quantity + InItem->Quantity <= Item->TotalQuantity)
		{
			Item->Quantity += InItem->Quantity;
			InItem->Quantity = 0;
			return true;
		}
		
		if (Item->ItemID == InItem->ItemID && Item->Quantity + InItem->Quantity > Item->TotalQuantity)
		{
			int32 Remaining = Item->TotalQuantity - Item->Quantity;
			Item->Quantity = Item->TotalQuantity;
			InItem->Quantity -= Remaining;
		}
	}
	
	if(CurrentInventoryUsage < TotalInventorySize)
	{
		Inventory.Add(InItem);
		CurrentInventoryUsage++;
		return true;
	}
	
	return false;
}

bool UInventoryComponent::DeleteItem(UItem* InItem)
{
	for(const UItem* Item : Inventory)
	{
		if(Item == InItem)
		{
			Inventory.Remove(InItem);
			CurrentInventoryUsage--;
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::Remove(int32 index, int32 Quantity)
{
	for(UItem* Item : Inventory)
	{
		if(Item->ItemID == Inventory[index]->ItemID && Item->Quantity - Quantity >= 0)
		{
			Item->Quantity -= Quantity;
			if(Item->Quantity == 0)
			{
				Inventory.RemoveAt(index);
				CurrentInventoryUsage--;
			}
			return true;
		}
	}
	return false;
}
