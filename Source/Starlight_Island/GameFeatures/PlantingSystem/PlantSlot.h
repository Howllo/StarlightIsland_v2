// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FarmingSystem.h"
#include "Components/SceneComponent.h"
#include "PlantSlot.generated.h"

class USphereComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARLIGHT_ISLAND_API UPlantSlot : public USceneComponent
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant Actor", meta = (AllowPrivateAccess="true"))
    USphereComponent* PlantCollision;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plant Actor", meta=(AllowPrivateAccess="true"))
    UStaticMeshComponent* PlantMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Plant Slot", meta=(AllowPrivateAccess = "true"))
    UPlant* Plant;

    UPROPERTY()
    float TotalHP = 20.f;

    // Holder for the Delegate.
    FOnHarvestDelegate OnHarvested;
public:
    // Sets default values for this component's properties
    UPlantSlot();

    // Function to set function pointer OnHarvest
    void SetOnHarvested(const FOnHarvestDelegate& InOnHarvested);
    
    /**
     * Used to set Plant data to the plant actor.
     * @param InPlantData Plant data to set the plant actor to.
     */
    UFUNCTION(BlueprintCallable, Category="Plant Actor")
    void SetPlantData(UPlant* InPlantData);
    

    /**
     * Used to get the plant data of the plant actor.
     * @return Return the plant data of the plant actor.
     */
    UFUNCTION(BlueprintCallable, Category="Plant Actor")
    FORCEINLINE UPlant* GetPlantData() const { return Plant; }
    
    /**
     * @brief Used to get the plant actor that is currently in the slot.
     * @return Return the plant actor.
     */
    UFUNCTION(BlueprintCallable, Category="Plant Slot")
    void SetMesh(UStaticMesh* Mesh);

    /**
     * @brief Used to remove the plant actor from the slot.
     */
    UFUNCTION(BlueprintCallable, Category="Plant Slot")
    void RemovePlant();
 
    /**
     * @brief Used to check if the slot is empty.
     * @return Return true if the slot is empty.
     */
    UFUNCTION(BlueprintCallable, Category="Plant Slot")
    FORCEINLINE bool IsSlotEmpty() const { return PlantMesh->GetStaticMesh() == nullptr; }

    /**
     * @brief Used to set the mesh component of the plant actor.
     * @param InMesh The mesh component to set the plant actor to.
     */
    UFUNCTION(BlueprintCallable, Category="Plant Slot")
    FORCEINLINE void SetMeshComponent(UStaticMeshComponent* InMesh) { PlantMesh = InMesh; }

    /**
     * @brief Used to set the collision component of the plant actor.
     * @param InCollision The collision component to set the plant actor to.
     */
    UFUNCTION(BlueprintCallable, Category="Plant Slot")
    void SetCollisionComponent(USphereComponent* InCollision);

    /**
     * @brief Used to get the collision component of the plant actor.
     * @return Return the collision component of the plant actor.
     */
    UFUNCTION(BlueprintCallable, Category="Plant Slot")
    FORCEINLINE USphereComponent* GetCollisionComponent() const { return PlantCollision; }

    /**
     * @brief Used to get the mesh component of the plant actor.
     * @return Return the mesh component of the plant actor.
     */
    UFUNCTION(BlueprintCallable, Category="Plant Slot")
    FORCEINLINE UStaticMeshComponent* GetMeshComponent() const { return PlantMesh; }
 
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);
};
