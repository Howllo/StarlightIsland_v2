// Copyrighted Celestial Games. All Rights Reserved.


#include "StarlightGameInstance.h"
#include "Starlight_Island/Misc/ItemFactory.h"
#include "Starlight_Island/Misc/PlantFactory.h"

UStarlightGameInstance::UStarlightGameInstance()
{
    ItemFactory = CreateDefaultSubobject<UItemFactory>(TEXT("Item Factory"));
    PlantFactory = CreateDefaultSubobject<UPlantFactory>(TEXT("Plant Factory"));
}

void UStarlightGameInstance::Init()
{
    Super::Init();
}
