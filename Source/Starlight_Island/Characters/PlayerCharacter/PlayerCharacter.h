 // Copyright Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "NativeGameplayTags.h"
#include "Starlight_Island/Enumeration/EPlayerState.h"
#include "PlayerCharacter.generated.h"

class UTemperatureComponent;
class UStatsComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UInventoryComponent;
class APlayerCharState;
class APlayerCharController;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    EPlayerGameState PlayerGameState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UTemperatureComponent* TemperatureComponent;
    
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

#pragma region Close Up
	/* Get the current distance between the character mesh and the camera. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Close Up", meta = (AllowPrivateAccess = "true"))
	float CloseUpDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Close Up", meta = (AllowPrivateAccess = "true"))
	float CloseUpOffSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera|Close Up", meta = (AllowPrivateAccess = "true"))
	float CloseUpInterpSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Close Up", meta = (AllowPrivateAccess = "true"))
	FVector CloseUpStartLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Close Up", meta = (AllowPrivateAccess = "true"))
	FVector CloseUpEndLocation;

	UPROPERTY()
	FVector CloseUpOriginal;
#pragma endregion

	/* Inventory System Cached */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* Inventory;

	UPROPERTY()
	const APlayerCharState* CurrentPlayerState;

	UPROPERTY()
	const APlayerCharController* CurrentPlayerController;

	UPROPERTY()
	UEnhancedInputComponent* EnhancedInput;

    UPROPERTY()
    FTimerHandle StunHandler;

    UPROPERTY()
    bool bIsButtonDown;

    UPROPERTY()
    UInputComponent* CacheInputComponent;
public:
	APlayerCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
    FGameplayTag PlayerTag;
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

    /* Called when the user is still pushing down the move button. */
    void CheckMovement();

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/* Move the camera down toward the character face. */
	void InterpCameraOnCloseUP(const int32 direction, const float DeltaSecond);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /**
     * @brief Jump the player character.
     */
    void PlayerJump();

    /**
     * @brief Check if the player can perform an action.
     * @return True if the player can perform an action, false otherwise.
     */
    bool CheckIfPlayerCanPerformAction(EPlayerGameState AllowedState = EPlayerGameState::Default) const;

    /**
     * @brief Used to set the player state.
     */
    void StopMoving();
    
	// To add mapping context
	virtual void BeginPlay() override;

	// Constantly called.
	virtual void Tick(float DeltaSeconds) override;

	/**
     * @brief When the player is controlling the pawn.
     * @param NewController The controller that is controlling the pawn.
     */
	virtual void PossessedBy(AController* NewController) override;

    /**
     * @brief When the player is no longer controlling the pawn.
     */
    virtual void UnPossessed() override;

    /**
     * @brief Unstun the character.
     */
    void UnStunCharacter();

public:
    /**
     * @brief Stun the character for a duration.
     * @param StunDuration The duration to stun the character.
     */
    void StunCharacter(const float StunDuration);
    
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/* Return Default Mapping Context */
	FORCEINLINE UInputMappingContext* GetDefaultMappingContext() const { return DefaultMappingContext; }

	/* Get the interact action. */
	FORCEINLINE UInputAction* GetInteractAction() const { return InteractAction; }

	/* Get the enhanced input component. */
	FORCEINLINE UEnhancedInputComponent* GetEnhancedInputComponent() const { return EnhancedInput; }

	/* Get the player stats. */
	UStatsComponent* GetPlayerStatsComponent() const;

	/** Returns the inventory of the player character. **/
	class UInventoryComponent* GetInventory() const;
};
