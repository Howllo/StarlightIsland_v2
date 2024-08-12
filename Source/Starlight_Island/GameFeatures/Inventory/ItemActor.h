// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

enum class EPlayerInventoryStatus : uint8;
class APlayerCharacter;
class USphereComponent;
class UItem;

UCLASS()
class STARLIGHT_ISLAND_API AItemActor : public AActor
{
	GENERATED_BODY()

	// To prevent player from using the chase mechanic to bring back item to base with full inventory.
	UPROPERTY()
	TMap<APlayerCharacter*, EPlayerInventoryStatus> InventoryFull;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemActor, meta=(AllowPrivateAccess = "true",
		ToolTip = "The sphere component that will detect the player."))
	USphereComponent* DetectPlayerRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemActor, meta=(AllowPrivateAccess = "true",
		ToolTip = "The sphere component that will interact with the player."))
	USphereComponent* InteractWithPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemActor, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemActor, meta=(AllowPrivateAccess = "true",
		ToolTip = "The item that will be held by the actor."))
	UItem* ItemHolder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemActor, meta=(AllowPrivateAccess = "true",
		ToolTip = "The speed in which the item will follow the player."))
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemActor, meta=(AllowPrivateAccess = "true",
		ToolTip = "The timer to start following the player."))
	float TimerToStartFollow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemActor, meta=(AllowPrivateAccess = "true",
		ToolTip = "The timer that increase the speed of the follow."))
	float TimerToIncreaseInterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemActor, meta=(AllowPrivateAccess = "true",
		ToolTip = "How much the speed increase based on the timer to increase interp speed."))
	float InterpSpeedIncrease;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=ItemActor, meta=(AllowPrivateAccess = "true",
		ToolTip = "The player character that the item will follow."))
	TArray<APlayerCharacter*> PossibleTargets;
	
	UPROPERTY()
	FTimerHandle ActivateFollowAgain;

	UPROPERTY()
	FTimerHandle ActivateFollow;

	UPROPERTY()
	FTimerHandle IncreaseInterpSpeed;

	UPROPERTY()
	bool bIsTracking = false;

	UPROPERTY()
	FTimerHandle DespawnTimer;
public:
	// Sets default values for this actor's properties
	AItemActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemActor)
	float RestartFollowTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Restarts the follow of the player character.
	 */
	UFUNCTION()
	void FollowPlayer();
	
	/**
	 * Increase speed of the interpolation to prevent issues with the item following the player forever.
	 */
	UFUNCTION()
	void IncreaseSpeed();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Used to set the AActor ItemHolder. */
	UFUNCTION()
	void SetItemHolder(UItem* Item);

	/* Get Mesh */
	UFUNCTION()
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }
	
	UFUNCTION()
	FORCEINLINE void SetInterpSpeed(float Speed) { InterpSpeed = Speed; }
	
	UFUNCTION()
	FORCEINLINE void SetTimerToStartFollow(float Timer) { TimerToStartFollow = Timer; }

	UFUNCTION()
	FORCEINLINE void SetTimerToIncreaseInterpSpeed(float Timer) { TimerToIncreaseInterpSpeed = Timer; }

	UFUNCTION()
	FORCEINLINE void SetInterpSpeedIncrease(float Speed) { InterpSpeedIncrease = Speed; }

	// Called when the timer is up and the item will be destroyed.
	FORCEINLINE void DespawnItemActor() { Destroy(); }
};
