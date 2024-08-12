// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractParent.h"
#include "GameFramework/Actor.h"
#include "InteractGather.generated.h"

class UStarlightGameInstance;
class AStarlightGameState;
class UGatherItemComponent;
class USphereComponent;
class UItem;

/**
 * Generic class for open world gathering interactable objects to derive from.
 *
 * Used for gathering interactable objects in the world that may or may not respawn.
 * This is not used for player created plants or objects.
 */
UCLASS(Blueprintable)
class STARLIGHT_ISLAND_API AInteractGather : public AInteractParent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact|Gather", meta=(AllowPrivateAccess = "true"))
    bool bDebug;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact|Gather", meta=(AllowPrivateAccess = "true"))
    USphereComponent* DebugSphere;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interact|Gather", meta=(AllowPrivateAccess = "Interact|Gather"))
    UDataTable* ItemDatabase;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interact|Gather", meta=(AllowPrivateAccess = "true"))
    TArray<UItem*> Items;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Gather", meta = (AllowPrivateAccess = "true",
        ClampMin = "0", ToolTip = "The ID of the item that will be gathered. This will destroy all ItemActors."))
    FGameplayTag ItemID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact|Gather", meta=(AllowPrivateAccess = "true",
        ToolTip = "The total amount of item actors that will be spawned", ClampMin = "1"))
    int32 TotalItemToSpawn;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Gather", meta = (AllowPrivateAccess = "true"))
    int32 StackItemAmount;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact|Gather", meta=(AllowPrivateAccess = "true"))
    float CurrentTime;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact|Gather", meta=(AllowPrivateAccess = "true"))
    bool bIsGathered;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact|Gather", meta=(AllowPrivateAccess = "true",
        ToolTip = "If the object get hidden after being picked and taken from the world."))
    bool bObjectHidden;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact|Gather", meta=(AllowPrivateAccess = "true"))
    bool bIsRespawnable;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact|Gather|Respawn", meta=(AllowPrivateAccess = "true",
        EditCondition = "bIsRespawnable", ToolTip = "The time it will take to respawn the item(s). In real-time minutes."))
    float RespawnTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact|Gather|Respawn", meta=(AllowPrivateAccess = "true",
        EditCondition = "bIsRespawnable", ToolTip = "The time it will take to respawn the item(s). In real-time minutes."))
    float RespawnTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact|Gather|Respawn", meta=(AllowPrivateAccess = "true",
        EditCondition = "bIsRespawnable", ToolTip = "The radius that will allow the interactable to respawn."))
    float RespawnRadius;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact|Gather", meta=(AllowPrivateAccess = "true"))
    float Distance;

    UPROPERTY()
    float DeltaTimeToRespawn;
    
    UPROPERTY()
    AStarlightGameState* GameState;

    UPROPERTY()
    UStarlightGameInstance* GameInstance;

    /* Used for Tick updates per 3 seconds. */
    UPROPERTY()
    FTimerHandle RespawnTimerHandle;

    // Collision Holder
    ECollisionEnabled::Type CollisionType;

    /* Just used for property changes in detail menu. */
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent & PropertyChangedEvent) override;

public:
    // Sets default values for this actor's properties
    AInteractGather();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /* Set CurrenTime. */
    UFUNCTION(BlueprintCallable, Category="Interact|Gather")
    void SetCurrentTime(const float InTime);
    
    /* Reset the gathering */
    UFUNCTION(BlueprintCallable, Category="Interact|Gather")
    void ResetGather();

    /* Spawn Item */
    UFUNCTION(BlueprintCallable, Category="Interact|Gather")
    void SpawnItem();

    UFUNCTION()
    void ClearItems();

    UFUNCTION()
    void MarkPackageDirty() const;
public:
    /* Used for interaction. */
    virtual void Interact() override;
    
    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex) override;
};
