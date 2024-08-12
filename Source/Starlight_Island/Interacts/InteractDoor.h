// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractParent.h"
#include "GameFramework/Actor.h"
#include "InteractDoor.generated.h"

enum class EDoorStatus : uint8;

/**
 * Generic class for interactable door.
 *
 * Used for door that can be opened and closed. Allows for open only door or the ability to open and close.
 * It with timer and for animation to be played when the door is opened or closed.
 */
UCLASS()
class STARLIGHT_ISLAND_API AInteractDoor : public AInteractParent
{
    GENERATED_BODY()

    UPROPERTY()
    bool bHasSetBindActionInteract;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact|Interact Door", meta=(AllowPrivateAccess = "true"))
    bool bIsDoorCloseable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact|Interact Door", meta = (AllowPrivateAccess = "true"))
    bool bIsDoorInteractable;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Interact Door", meta = (AllowPrivateAccess = "true"))
    EDoorStatus DoorStatus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact|Interact Door", meta = (AllowPrivateAccess = "true"))
    float DoorOpenCloseTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact|Interact Door", meta = (AllowPrivateAccess = "true"))
    float PLayerLockTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact|Interact Door", meta = (AllowPrivateAccess = "true",
        ToolTip = "This is for timer to start the collision again."))
    float TimerForCollision;
    
    UPROPERTY()
    FTimerHandle DoorTimer;
    
    UPROPERTY()
    FTimerHandle DoorPhysicsTimer;
public:
    // Sets default values for this actor's properties
    AInteractDoor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /* Reusable code for setting visibility of a widget. */
    virtual void CheckWidgetVisibility() override;

    /* Open and close the door. */
    void OpenAndCloseDoor();

    /* Used to start the collision. Exist because DoorTimer does trigger at the correct time. */
    void StartCollisionTimer();

    /* Start timer for the interp and collision. */
    void StartTimers();
public:

    // Interact function that need be override in child classes.
    virtual void Interact() override;

    UFUNCTION(BlueprintCallable, Category = "Interact")
    FORCEINLINE UEnhancedInputComponent* GetEnhancedInputComponent() const { return EnhancedInputComponent; }

    UFUNCTION(BlueprintCallable, Category = "Interact")
    FORCEINLINE UInputMappingContext* GetDefaultMappingContext() const { return DefaultMappingContext; }
    
    // Called when the player overlaps with the interact sphere.
    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult) override;

    virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent*
        OtherComp, int32 OtherBodyIndex) override;
    
};
