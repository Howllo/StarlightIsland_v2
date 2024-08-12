// Copyrighted Celestial Games. All Rights Reserved.


#include "InteractDoor.h"
#include "Blueprint/UserWidget.h"
#include "Starlight_Island/Characters/PlayerCharacter/PlayerCharacter.h"
#include "Starlight_Island/Enumeration/EDoorStatus.h"

// Sets default values
AInteractDoor::AInteractDoor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Set Door.
    DoorStatus = EDoorStatus::CloseAwaitOpen;

    // Set Door to be interactable.
    bIsDoorInteractable = true;

    // Set Door to be closeable.
    bIsDoorCloseable = true;

    // Set Door open and close time.
    DoorOpenCloseTime = 5.f;

    // Set Player lock timer.
    PLayerLockTimer = 3.f;

    // Set Door to not have set bind action interact.
    bHasSetBindActionInteract = false;

    // Set the timer for collision.
    TimerForCollision = 0.25f;
}

// Called when the game starts or when spawned
void AInteractDoor::BeginPlay()
{
    Super::BeginPlay();

    // Set the original collision profile.
    CollisionEnabled = MeshComp->GetCollisionEnabled();
}

void AInteractDoor::CheckWidgetVisibility()
{
    if(!InteractWidget && InteractRef)
    {
        InteractWidget = CreateWidget<UUserWidget>(GetWorld(), InteractRef.Get());
        if(InteractWidget)
        {
            InteractWidget->AddToViewport();
            CheckWidgetVisibility();
        }
    }
    
    ESlateVisibility Visibility = InteractWidget->GetVisibility();
    
    if(Visibility != ESlateVisibility::Hidden)
    {
        InteractWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    else if(Visibility == ESlateVisibility::Hidden && bIsDoorInteractable && DoorStatus != EDoorStatus::OpenCannotClose)
    {
        InteractWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AInteractDoor::OpenAndCloseDoor()
{
    if(!MeshComp) { return; }

    const float DeltaTime = GetWorld()->GetDeltaSeconds();
    
    if(DoorStatus == EDoorStatus::OpenAwaitClose || DoorStatus == EDoorStatus::OpenCannotClose
        && MeshComp->GetRelativeRotation().Yaw > -90.f)
    {
        // Rotation interp.
        MeshComp->SetRelativeRotation(FMath::RInterpTo(
            MeshComp->GetRelativeRotation(),
            FRotator(0.f, -90.f, 0.f), DeltaTime, DoorOpenCloseTime));

        // Stop the timer once the door is fully open.
        if(MeshComp->GetRelativeRotation().Yaw <= -90.f)
        {
            UE_LOG(LogTemp, Warning, TEXT("OpenAwaitClose"));
            GetWorld()->GetTimerManager().ClearTimer(DoorTimer);
        }
    }
    else if(DoorStatus == EDoorStatus::CloseAwaitOpen && MeshComp->GetRelativeRotation().Yaw < 0.0f)
    {
        
        MeshComp->SetRelativeRotation(FMath::RInterpTo(
            MeshComp->GetRelativeRotation(),
            FRotator(0.f, 0.f, 0.f), DeltaTime, DoorOpenCloseTime));

        if(MeshComp->GetRelativeRotation().Yaw >= 0.f)
        {
            UE_LOG(LogTemp, Warning, TEXT("CloseAwaitOpen"));
            GetWorld()->GetTimerManager().ClearTimer(DoorTimer);
        }
    }
}

void AInteractDoor::StartCollisionTimer()
{
    MeshComp->SetCollisionEnabled(CollisionEnabled.GetValue());
    bIsDoorInteractable = true;
}

void AInteractDoor::StartTimers()
{
    if(!bIsDoorInteractable) { return; }
    
    // 0.0166 is about 60 frames per second.
    GetWorld()->GetTimerManager().SetTimer(DoorTimer, this, &AInteractDoor::OpenAndCloseDoor,
        0.0166, true);

    // Set the collision to be enabled after the door is fully open.
    GetWorld()->GetTimerManager().SetTimer(DoorPhysicsTimer, this, &AInteractDoor::StartCollisionTimer,
        TimerForCollision, false);
    
    bIsDoorInteractable = false;
}

void AInteractDoor::Interact()
{
    Super::Interact();
    if(!bIsDoorInteractable) { return; }

    if(DoorStatus == EDoorStatus::CloseAwaitOpen)
    {
        DoorStatus = EDoorStatus::OpenAwaitClose;
        LockPlayerControl(PLayerLockTimer);
        MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        if(!bIsDoorCloseable)
        {
            CheckWidgetVisibility();
            DoorStatus = EDoorStatus::OpenCannotClose;
        }

        // Start the timer for interp and collision.
        StartTimers();
    }
    else if(DoorStatus == EDoorStatus::OpenAwaitClose)
    {
        DoorStatus = EDoorStatus::CloseAwaitOpen;
        LockPlayerControl(PLayerLockTimer);
        MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        
        // Start the timer for interp and collision.
        StartTimers();
    }
}

void AInteractDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if(OtherActor->IsA(APlayerCharacter::StaticClass()))
    {
        // Set up the input component for the interactable object.
        SetupInteractAction(OtherActor);

        // Check the visibility of the widget.
        CheckWidgetVisibility();
    }
}

void AInteractDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnOverlapEnd(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
    
    if(OtherActor->IsA(APlayerCharacter::StaticClass()))
    {
        // Remove the binding action interact.
        UnbindInteractAction(this);
        
        // Check the visibility of the widget.
        CheckWidgetVisibility();
    }
}
