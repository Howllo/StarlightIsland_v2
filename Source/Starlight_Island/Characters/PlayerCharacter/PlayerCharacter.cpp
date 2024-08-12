 // Copyright Celestial Games. All Rights Reserved.

#include "PlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerCharController.h"
#include "TemperatureComponent.h"
#include "Starlight_Island/Enumeration/EPlayerState.h"
#include "Starlight_Island/GameMode/PlayerCharState.h"
#include "Starlight_Island/NativeTags/CollisionTag.h"

 DEFINE_LOG_CATEGORY(LogTemplateCharacter);

APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->GravityScale = 2.1f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 80.f));

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Setup Mesh to proper location within capsule component. 
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -92.5f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// Close up
	CloseUpOffSet = 10.f;
	CloseUpInterpSpeed = 20.0f;

    // Player Tag using FGameplayTag
    PlayerTag = CollisionTag::Player;

    // PlayerState
    PlayerGameState = EPlayerGameState::Idle;

    // Temperature Component
    TemperatureComponent = CreateDefaultSubobject<UTemperatureComponent>(TEXT("TemperatureComponent"));
}

 void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Close up - Distance
	CloseUpDistance = FVector::Distance(GetMesh()->GetComponentLocation(), FollowCamera->GetComponentLocation());

	// Close Up - Original Boom
	CloseUpOriginal = CameraBoom->GetRelativeLocation();
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CloseUpDistance = FVector::Distance(GetMesh()->GetComponentLocation(), FollowCamera->GetComponentLocation());

	if(CloseUpDistance < 200)
	{
		InterpCameraOnCloseUP(1, DeltaSeconds);
	}
	else if (CloseUpDistance >= 200)
	{
		InterpCameraOnCloseUP(-1, DeltaSeconds);
	}

    if(PlayerGameState == EPlayerGameState::Jumping)
    {
        if(!GetCharacterMovement()->IsFalling())
        {
            PlayerGameState = EPlayerGameState::Idle;
        }
    }

    CheckMovement();
}

 void APlayerCharacter::PossessedBy(AController* NewController)
 {
	 Super::PossessedBy(NewController);

	// Cache Variables of Player State.
	CurrentPlayerState = Cast<APlayerCharState>(GetPlayerState());
 }

 void APlayerCharacter::UnPossessed()
 {
	 Super::UnPossessed();
	
	CurrentPlayerState = nullptr;
	CurrentPlayerController = nullptr;
 }

 void APlayerCharacter::StunCharacter(const float StunDuration)
 {
    PlayerGameState = EPlayerGameState::Stunned;
    GetWorld()->GetTimerManager().SetTimer(StunHandler, this, &APlayerCharacter::UnStunCharacter,
        StunDuration, false);
 }

 void APlayerCharacter::UnStunCharacter()
 {
    PlayerGameState = EPlayerGameState::Idle;
 }

 UStatsComponent* APlayerCharacter::GetPlayerStatsComponent() const
 {
	return CurrentPlayerState->GetStatsComponent();
 }

 UInventoryComponent* APlayerCharacter::GetInventory() const
 {
	if(CurrentPlayerState && CurrentPlayerState->GetInventory())
	{
		return CurrentPlayerState->GetInventory();
	}
	return nullptr;
 }

 void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    InputComponent = PlayerInputComponent;
    
	// Add Input Mapping Context
	if (const APlayerCharController* PlayerController = Cast<APlayerCharController>(GetController()))
	{
		CurrentPlayerController = PlayerController;
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInput = EnhancedInputComponent;
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::PlayerJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Ongoing, this, &APlayerCharacter::CheckMovement);
	    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopMoving);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	}
}

 void APlayerCharacter::PlayerJump()
 {
    if(CheckIfPlayerCanPerformAction()) return;

    PlayerGameState = EPlayerGameState::Jumping;
    Jump();
 }

 bool APlayerCharacter::CheckIfPlayerCanPerformAction(const EPlayerGameState AllowedState) const
 {
     switch (AllowedState)
     {
        case EPlayerGameState::Default:
            return PlayerGameState >= EPlayerGameState::Jumping && PlayerGameState <= EPlayerGameState::Dead;
        case EPlayerGameState::Jumping:
            return PlayerGameState >= EPlayerGameState::Falling && PlayerGameState <= EPlayerGameState::Dead;
         default:
            return PlayerGameState >= EPlayerGameState::Jumping && PlayerGameState <= EPlayerGameState::Dead;
     }
 }

 void APlayerCharacter::StopMoving()
 {
    PlayerGameState = EPlayerGameState::Idle;
 }

 void APlayerCharacter::Move(const FInputActionValue& Value)
{
    if(CheckIfPlayerCanPerformAction(EPlayerGameState::Jumping)) return;
    
	// input is a Vector2D
    const FVector2D MovementVector = Value.Get<FVector2D>();

    // Set State
    PlayerGameState = EPlayerGameState::Walking;

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

 void APlayerCharacter::CheckMovement()
 {
    if(!CheckIfPlayerCanPerformAction())
    {
        SetupPlayerInputComponent(CacheInputComponent);
    }
 }

 void APlayerCharacter::Look(const FInputActionValue& Value)
{
    if(CheckIfPlayerCanPerformAction()) return;
    
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::InterpCameraOnCloseUP(int32 direction, float DeltaSecond)
{
	// To location.
	if(direction > 0)
	{
		CloseUpEndLocation = FVector(0.f, 0.f, CloseUpOriginal.Z - CloseUpOffSet);
	}
	else if (direction < 0)
	{
		CloseUpEndLocation = CloseUpOriginal;
	}
	
	// Current Location
	CloseUpStartLocation = FVector(0.f, 0.f, CameraBoom->GetRelativeLocation().Z);

	FVector InterLoc = FMath::VInterpTo(CloseUpStartLocation, CloseUpEndLocation,
		DeltaSecond, CloseUpInterpSpeed);
	CameraBoom->SetRelativeLocation(InterLoc);
}