// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "TreeCutting.generated.h"

class UStarlightGameInstance;
class UCapsuleComponent;
class UArrowComponent;
class UItem;
class AStarlightGameState;

/**
 * Class for cutting down trees and getting resources from them.
 * This may need to be converted to a AActor class.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARLIGHT_ISLAND_API UTreeCutting : public UActorComponent
{
	GENERATED_BODY()

public:
	UTreeCutting();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tree, meta=(AllowPrivateAccess = "true"))
	TArray<USceneComponent*> SpawnLocations;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tree, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* HitRegister;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tree, meta = (AllowPrivateAccess = "true",
		ToolTip = "Total amount of health that will be taken away before the tree will fall and give resources.", ClampMin="1.0"))
	float TreeTotalHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Tree, meta = (AllowPrivateAccess = "true",
		ToolTip = "The current amount of health that the tree has."))
	float TreeCurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tree, meta = (AllowPrivateAccess = "true",
		ToolTip = "The amount of force that will be applied to the tree when it falls over.", ClampMin="0.0"))
	float ForceFallingOver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tree, meta = (AllowPrivateAccess = "true",
		ToolTip="Two point roll rotation limits before the tree destroy X > Tree Not Destroy < Y. X = Min, Y = Max.",
		ClampMin="-360.0", ClampMax="360.0"))
	FVector2D TreeRotationLimitsRoll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tree, meta = (AllowPrivateAccess = "true",
			ToolTip="Two point pitch rotation limits before the tree destroy X > Tree Not Destroy < Y. X = Min, Y = Max.",
			ClampMin="-360.0", ClampMax="360.0"))
	FVector2D TreeRotationLimitsPitch;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category=Tree,
	meta = (ToolTip = "The timer in which the tree will automatically be destroyed.", ClampMin="0.0"))
	float StuckTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Tree, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	
#pragma region Spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tree|Spawning", meta=(AllowPrivateAccess = "true",
		ToolTip = "Draw the debug marker to show where the items will spawn."))
	bool bDrawDebugMarker;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tree|Spawning", meta = (AllowPrivateAccess = "true",
		ToolTip = "Use the Item ID to look up in the Item Database to pull information.", ClampMin="0"))
	FGameplayTag ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tree|Spawning", meta = (AllowPrivateAccess = "true",
		ToolTip = "Takes in the amount of items in the stack. \nExample: If you want a stack to start with 5 item then enter 5.",
		ClampMin="1"))
	int32 StartingItemQuantity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tree|Spawning", meta = (AllowPrivateAccess = "true",
		ToolTip = "The total amount of spawn that will happen. \nExample: 2 = Spawning SpawnItem twice in two different location.",
		ClampMin="0"))
	int32 TotalAmountSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tree|Spawning", meta=(AllowPrivateAccess = "true",
		ToolTip = "This is used for the size of an item for calculation of spawning.", ClampMin="0.0"))
	float SizeOfItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tree|Spawning", meta=(AllowPrivateAccess = "true",
		ToolTip = "This is the max distance that will be allowed to spawn at. Anything past this will be set to this location.",
		ClampMin="0.0"))
	float TotalDistanceForSpawning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tree|Spawning", meta=(AllowPrivateAccess = "true",
		ToolTip = "The minimum distance that will be allowed to spawn at. Anything below this will be set to this location.",
		ClampMin="0.0"))
	float MinimumDistanceForSpawning;
#pragma endregion
	
	UPROPERTY()
	class APlayerCharacter* PlayerCharacter;
	
	// Track the added distance to prevent item(s) from going past Total Distance.
	UPROPERTY()
	float DistanceTracker;
	
	UPROPERTY()
	AActor* OwnerOfComponent;

	UPROPERTY()
	FTimerHandle RotationCheckTimerHandle;

	UPROPERTY()
	FTimerHandle StuckTimerDestroy;

	UPROPERTY()
	FTimerHandle DebugTracker;

	UPROPERTY()
	AStarlightGameState* StarlightGameState;

	UPROPERTY()
	UStarlightGameInstance* StarlightGameInstance;

	// Delegate for DestroyTree.
	FTimerDelegate TreeDestroyTimerDel;

	// Delegate for Stuck Tree
	FTimerDelegate TreeStuckTimerDel;

	void CreateItem(int32 Index);
	
protected:
	//Called when the game starts
	virtual void BeginPlay() override;

	// Start on component added.
	virtual void OnRegister() override;

	/* Draw the debug sphere to show spawn locations. */
	void DrawDebugMarker();
	
public:
	// Destroy the tree once it hits the ground and spawn material that goes to the player.
	UFUNCTION(BlueprintCallable, Category=Tree)
	void DestroyTree(bool bIsStuck);

	UFUNCTION()
	void DealDamage(float Damage);
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:

	// Exist to create the spawn locations for the items.
	void CreateSpawnLocations();
};
