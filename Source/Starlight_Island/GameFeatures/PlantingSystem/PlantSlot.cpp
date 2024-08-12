// Copyrighted Celestial Games. All Rights Reserved.

#include "PlantSlot.h"
#include "Plant.h"
#include "Components/SphereComponent.h"
#include "Starlight_Island/NativeTags/CollisionTag.h"


UPlantSlot::UPlantSlot()
{
    // Create the collision that the plant actor will overlap with.
    PlantCollision = nullptr;

    // Create Mesh
    PlantMesh = nullptr;

    // Set the total HP of the plant actor.
    TotalHP = 20.f;

    // Set Plant to Nullptr
    Plant = nullptr;
}

void UPlantSlot::SetOnHarvested(const FOnHarvestDelegate& InOnHarvested)
{
    OnHarvested = InOnHarvested;
}

void UPlantSlot::SetPlantData(UPlant* InPlantData)
{
    if(InPlantData)
    {
        Plant = InPlantData;
        if(Plant->PlantStageMeshes.Num() > 0)
        {
            SetMesh(Plant->PlantStageMeshes[0]);
        }
    }
}

void UPlantSlot::SetMesh(UStaticMesh* Mesh)
{
    PlantMesh->SetStaticMesh(Mesh);
}

void UPlantSlot::RemovePlant()
{
    PlantMesh->SetStaticMesh(nullptr);
    Plant = nullptr;
}

void UPlantSlot::SetCollisionComponent(USphereComponent* InCollision)
{
    PlantCollision = InCollision;
    PlantCollision->OnComponentBeginOverlap.AddDynamic(this, &UPlantSlot::OnOverlapBegin);
    PlantCollision->OnComponentEndOverlap.AddDynamic(this, &UPlantSlot::OnOverlapEnd);
}

void UPlantSlot::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    // TODO: Update this with the correct weapon actor.
    if(OtherActor->ActorHasTag(CollisionTag::Scythe.GetTag().GetTagName()))
    {
        if(OnHarvested.IsBound())
        {
            OnHarvested.Execute(Plant);
            RemovePlant();
        }
    }
}

void UPlantSlot::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
}
