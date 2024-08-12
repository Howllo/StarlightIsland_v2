// Copyrighted Celestial Games. All Rights Reserved.


#include "InteractGather.h"
#include "Components/SphereComponent.h"
#include "Starlight_Island/Characters/PlayerCharacter/PlayerCharacter.h"
#include "Starlight_Island/Manager//TimeManager.h"
#include "Starlight_Island/GameFeatures/Inventory/InventoryComponent.h"
#include "Starlight_Island/GameFeatures/Inventory/Item.h"
#include "Starlight_Island/GameMode/StarlightGameInstance.h"
#include "Starlight_Island/GameMode/StarlightGameState.h"
#include "Starlight_Island/Misc/ItemFactory.h"

AInteractGather::AInteractGather()
{
    bDebug = false;
    DebugSphere = nullptr;
    StackItemAmount = 1;
    bIsGathered = false;
    bObjectHidden = false;
    bIsRespawnable = false;
    TotalItemToSpawn = 1;
    RespawnTime = 0.0f;
    RespawnTimer = 0.0f;
    CurrentTime = 0.0f;
    bIsAllowedToInteract = true;
    DeltaTimeToRespawn = 0.0f;
    GameState = nullptr;
    Distance = 0.0f;
    RespawnRadius = 0.0f;
    CollisionType = MeshComp->GetCollisionEnabled();
    ItemDatabase = nullptr;
    GameInstance = nullptr; 
}

void AInteractGather::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    
    if (PropertyName == GET_MEMBER_NAME_CHECKED(AInteractGather, ItemID))
    {
        SpawnItem();
    }
    else if (PropertyName == GET_MEMBER_NAME_CHECKED(AInteractGather, StackItemAmount))
    {
        if (StackItemAmount < 1)
        {
            StackItemAmount = 1;
        }
    }
    else if (PropertyName == GET_MEMBER_NAME_CHECKED(AInteractGather, RespawnTime))
    {
        if (RespawnTime < 0.0f)
        {
            RespawnTime = 0.0f;
        }
    }
    else if( PropertyName == GET_MEMBER_NAME_CHECKED(AInteractGather, RespawnRadius))
    {
        // Set the debug sphere radius
        if(bDebug)
        {
            DebugSphere->SetSphereRadius(RespawnRadius);
            DebugSphere->SetHiddenInGame(false);
        }
    }
    else if(PropertyName == GET_MEMBER_NAME_CHECKED(AInteractGather, TotalItemToSpawn))
    {
        if(TotalItemToSpawn < 1)
        {
            TotalItemToSpawn = 1;
        }
        SpawnItem();
    }
    else if(PropertyName == GET_MEMBER_NAME_CHECKED(AInteractGather, bIsRespawnable))
    {
        if(bIsRespawnable)
        {
            bIsGathered = false;
        }
    } else if(PropertyName == GET_MEMBER_NAME_CHECKED(AInteractGather, bDebug))
    {
        if(bDebug)
        {
            DebugSphere = NewObject<USphereComponent>(this, TEXT("Debug Sphere"));
            DebugSphere->SetupAttachment(RootComponent);
            DebugSphere->InitSphereRadius(RespawnRadius);
            DebugSphere->SetCollisionProfileName(TEXT("Trigger"));
        }
        else
        {
            DebugSphere->DestroyComponent();
            DebugSphere = nullptr;
        }
    }

    // Set Dirty Flag
    MarkPackageDirty();
}

void AInteractGather::BeginPlay()
{
    Super::BeginPlay();

    // Set Game Instance
    GameInstance = Cast<UStarlightGameInstance>(GetGameInstance());

    // Set Game State
    GameState = Cast<AStarlightGameState>(GetWorld()->GetGameState());

    // Set up Delegate for Current Time
    GameState->GetTimeManager()->OnCurrentTime.AddUObject(this, &AInteractGather::SetCurrentTime);

    // Fixes any item that is not set.
    for(int i = 0; i < Items.Num(); i++)
    {
        if(!Items[i])
        {
            Items[i] = UItemFactory::CreateItem(ItemID, StackItemAmount, ItemDatabase, this);
        }
    } 
}

void AInteractGather::SetCurrentTime(const float InTime)
{
    CurrentTime = InTime;
}

void AInteractGather::ResetGather()
{
    if(!PlayerCharacter) { return; }
    
    Distance = FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation());
    CurrentTime = GameState->GetTimeManager()->GetCurrentTime();
    
    if(CurrentTime >= RespawnTime && bIsRespawnable && Distance <= RespawnRadius)
    {
        bIsGathered = false;
        bIsAllowedToInteract = true;

        if(bObjectHidden)
        {
            SetActorHiddenInGame(false);
            MeshComp->SetCollisionEnabled(CollisionType);
        }

        // Clear the Respawn Timer
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
    }

    // Check if the player is in the radius.
    if(Distance < 300.f)
    {
        CheckWidgetVisibility();
    }
}

void AInteractGather::SpawnItem()
{
    ClearItems();
    for(int i = 0; i < TotalItemToSpawn; i++)
    {
        if(UItem* Item = UItemFactory::CreateItem(ItemID, StackItemAmount, ItemDatabase, this))
        {
            Item->SetFlags(RF_Transactional);
            Item->Modify();

            Items.Add(Item);
        }
    }
}

void AInteractGather::ClearItems()
{
    for (UItem* Item : Items)
    {
        if(Item)
        {
            Item->Modify();
            Item->ConditionalBeginDestroy();
        }
    }
    Items.Empty();
}

void AInteractGather::MarkPackageDirty() const
{
    if(GetPackage())
    {
        GetPackage()->SetDirtyFlag(true);
    }
}


void AInteractGather::Interact()
{
    if(!bIsGathered)
    {
        bIsGathered = true;
        bIsAllowedToInteract = false;
        CheckWidgetVisibility();
        
        for(const UItem* Item : Items)
        {
            PlayerCharacter->GetInventory()->Add(UItem::Copy(Item, PlayerCharacter->GetInventory()));
        }

        if(bObjectHidden)
        {
            SetActorHiddenInGame(true);
            
            // Save collision type
            CollisionType = MeshComp->GetCollisionEnabled();

            // Set collision to nothing.
            MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
        
        if(bIsRespawnable)
        {
            RespawnTimer = GameState->GetTimeManager()->CalculatedFutureTime(RespawnTime);

            // Set to three second to prevent over use of computing resources.
            GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AInteractGather::ResetGather,
                3.f, true);
        }
    }
}

void AInteractGather::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if(OtherActor->IsA(APlayerCharacter::StaticClass()) && OverlappedComp->ComponentHasTag("Interact"))
    {
        CheckWidgetVisibility();
        
        SetupInteractAction(OtherActor);
    }
    else if(OtherActor->IsA(APlayerCharacter::StaticClass()) && OverlappedComp->ComponentHasTag("PlayerDetect"))
    {
        if(!bIsGathered) { return; }
        
        if(RespawnTimerHandle.IsValid())
        {
            GetWorld()->GetTimerManager().UnPauseTimer(RespawnTimerHandle);
        }
        
        ResetGather();
    }
}

void AInteractGather::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnOverlapEnd(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

    if(OtherActor->IsA(APlayerCharacter::StaticClass()) && OverlappedComp->ComponentHasTag("Interact"))
    {
        CheckWidgetVisibility();
        
        UnbindInteractAction(this);
    }
    else if(OtherActor->IsA(APlayerCharacter::StaticClass()) && OverlappedComp->ComponentHasTag("PlayerDetect"))
    {
        GetWorld()->GetTimerManager().PauseTimer(RespawnTimerHandle);
    }
}
