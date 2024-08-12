// Copyrighted Celestial Games. All Rights Reserved.


#include "ItemFactory.h"
#include "GameplayTagContainer.h"
#include "Starlight_Island/DataTable//FItemList.h"
#include "Starlight_Island/GameFeatures/Inventory/Item.h"
#include "Starlight_Island/GameFeatures/Inventory/ItemActor.h"

UItemFactory::UItemFactory()
{
	ItemDatabase = ConstructorHelpers::FObjectFinder<UDataTable>(
		TEXT("/Script/Engine.DataTable'/Game/DataTables/Database/DT_ItemDatabase.DT_ItemDatabase'")).Object;
}

UItem* UItemFactory::CreateItem(const FGameplayTag ItemID, const int32 StartingItemStack) const
{
	UItem* Item = NewObject<UItem>();
    
	if(ItemDatabase)
	{
        const FString ItemIDString = ItemID.GetTagName().ToString();
        const FName ItemIDFName = *ItemIDString.Replace(*FString("Item."), *FString("")); 
	    
	    if(const FItemList* ItemRow = ItemDatabase->FindRow<FItemList>(ItemIDFName, FString("")))
		{
			Item->ItemID = ItemRow->ItemID;
			Item->ItemName = ItemRow->ItemName;
			Item->ItemDescription = ItemRow->ItemDescription;
	        Item->ItemType = ItemRow->ItemType;
			Item->ItemIcon = ItemRow->ItemIcon;
			Item->ItemMesh = ItemRow->ItemMesh;
			Item->bAllowStacking = ItemRow->bAllowStacking;
			Item->TotalQuantity = ItemRow->TotalQuantity;
			Item->InterpSpeed = ItemRow->InterpSpeed;
			Item->TimerToStartFollow = ItemRow->TimerToStartFollow;
			Item->TimerToIncreaseInterpSpeed = ItemRow->TimerToIncreaseInterpSpeed;
			Item->InterpSpeedIncrease = ItemRow->InterpSpeedIncrease;
			
			if(StartingItemStack > 0)
			{
				Item->Quantity = StartingItemStack;
			}
			else
			{
				Item->Quantity = ItemRow->Quantity;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Item ID: %s does not exist in the database."), *ItemIDString);
		}
	}
	return Item;
}

UItem* UItemFactory::CreateItem(const FGameplayTag ItemID, const int32 StartingItemStack, const UDataTable* InDatabase,
	UObject* InOuter)
{
	UItem* Item = NewObject<UItem>(InOuter);

	if(InDatabase)
	{
	    const FString ItemIDString = ItemID.GetTagName().ToString();
	    const FName ItemIDFName = *ItemIDString.Replace(*FString("Item."), *FString("")); 
	    
	    if(const FItemList* ItemRow = InDatabase->FindRow<FItemList>(ItemIDFName, FString("")))
		{
			Item->ItemID = ItemRow->ItemID;
			Item->ItemName = ItemRow->ItemName;
			Item->ItemDescription = ItemRow->ItemDescription;
	        Item->ItemType = ItemRow->ItemType;
			Item->ItemIcon = ItemRow->ItemIcon;
			Item->ItemMesh = ItemRow->ItemMesh;
			Item->bAllowStacking = ItemRow->bAllowStacking;
			Item->TotalQuantity = ItemRow->TotalQuantity;
			Item->InterpSpeed = ItemRow->InterpSpeed;
			Item->TimerToStartFollow = ItemRow->TimerToStartFollow;
			Item->TimerToIncreaseInterpSpeed = ItemRow->TimerToIncreaseInterpSpeed;
			Item->InterpSpeedIncrease = ItemRow->InterpSpeedIncrease;
			
			if(StartingItemStack > 0)
			{
				Item->Quantity = StartingItemStack;
			}
			else
			{
				Item->Quantity = ItemRow->Quantity;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Item ID: %s does not exist in the database."), *ItemIDString);
		}
	}
	return Item;
}

AItemActor* UItemFactory::CreateItemActor(const FGameplayTag ItemID, const FVector& InVector, const FRotator& InRotator,
                                          const int32 StartingItemStack, UWorld* World) const
{
	AItemActor* ItemActor = nullptr;
	if(World)
	{
		ItemActor = World->SpawnActor<AItemActor>(AItemActor::StaticClass(), InVector, InRotator);
		if(UItem* Item = CreateItem(ItemID, StartingItemStack); Item && ItemActor)
		{
			ItemActor->SetItemHolder(Item);
			ItemActor->SetInterpSpeed(Item->InterpSpeed);
			ItemActor->SetTimerToStartFollow(Item->TimerToStartFollow);
			ItemActor->SetTimerToIncreaseInterpSpeed(Item->TimerToIncreaseInterpSpeed);
			ItemActor->SetInterpSpeedIncrease(Item->InterpSpeedIncrease);
		} else
		{
			if(!Item)
			{
				UE_LOG(LogTemp, Error, TEXT("Item is nullptr in UItemFactory."));
			}
			
			if (!ItemActor)
			{
				UE_LOG(LogTemp, Error, TEXT("ITemActor is nullptr in UItemFactory."))
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("World is nullptr."));
	}
	return ItemActor;
}
