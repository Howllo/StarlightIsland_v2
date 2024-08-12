// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnhancedInputSubsystems.h"
#include "Starlight_Island/Characters/PlayerCharacter/PlayerCharController.h"
#include "InteractParent.generated.h"

class UUserWidget;
class APlayerCharacter;
class UBoxComponent;
class UInputAction;
class UWidgetComponent;
class UInputMappingContext;

/**
 * Generic class for interactable objects to derive from.
 */
UCLASS()
class STARLIGHT_ISLAND_API AInteractParent : public AActor
{
    GENERATED_BODY()

public:
    AInteractParent();

protected:
    UPROPERTY()
    USceneComponent* RootComp;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
    UStaticMeshComponent* MeshComp;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
    UBoxComponent* InteractTriggerBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Interact")
    TSoftClassPtr<UUserWidget> InteractRef;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interact")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
    UInputAction* InteractAction;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
    UEnhancedInputComponent* EnhancedInputComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact", meta =
        (ToolTip = "How many second the player will be locked to animation while interacting.", ClampMin = 0.0))
    float InputLockTime = 0.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact", meta = (AllowPrivateAccess = "true"))
    UUserWidget* InteractWidget;

    // Check if the player is allowed to interact with the interactable.
    UPROPERTY(meta=(ToolTip = "If the player is allowed to interact with the interactable object."))
    bool bIsAllowedToInteract;
    
    UPROPERTY()
    FTimerHandle LockTimer;

    UPROPERTY()
    APlayerCharController* PlayerController;

    UPROPERTY()
    APlayerCharacter* PlayerCharacter;

    UPROPERTY()
    // Save ECollisionEnabled to restore after interaction.
    TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled;
    
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Set up the input component for the interactable object.
    virtual void SetupInteractAction(AActor* OtherActor);

    // Set up the input component for the interactable object.
    virtual void UnbindInteractAction(UObject* InOwner);

    // Called when the game starts or when spawned
    virtual void BeginDestroy() override;
public:
    /* Interact function that need be override in child classes. */
    UFUNCTION(BlueprintCallable, Category = "Interact")
    virtual void Interact();

    /* Reusable code for setting visibility of a widget. */
    virtual void CheckWidgetVisibility();

    virtual void SetInteractWidget();
    
    /* Locks player control for interactions. */
    virtual void LockPlayerControl(float Seconds);
    
    // Called when the player overlaps with the interact sphere.
    UFUNCTION()
    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult);

    // Called when the player stops overlapping with the interact sphere.
    UFUNCTION()
    virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
