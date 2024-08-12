// Copyrighted Celestial Games. All Rights Reserved.

#include "FItemList.h"
#include "Starlight_Island/Enumeration/EItemType.h"
#include "Starlight_Island/NativeTags/ItemTag.h"

FItemList::FItemList()
{
    ItemID = ItemTag::Default;
	ItemName = FText::FromString("None");
	ItemDescription = FText::FromString("None");
    ItemType = EItemType::None;
	bIsUnique = false;
	ItemIcon = nullptr;
	ItemMesh = nullptr;
	bAllowStacking = false;
	TotalQuantity = 1;
	Quantity = 1;
	bChangeActorSpeed = false;
	InterpSpeed = 5.f;
	TimerToStartFollow = 1.f;
	TimerToIncreaseInterpSpeed = 2.5f;
	InterpSpeedIncrease = 2.f;
}
