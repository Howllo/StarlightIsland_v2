// Copyrighted Celestial Games. All Rights Reserved.


#include "PlayerCharState.h"
#include "Starlight_Island/GameFeatures/Inventory/InventoryComponent.h"

APlayerCharState::APlayerCharState()
{
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
}