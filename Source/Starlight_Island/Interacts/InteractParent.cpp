// Copyrighted Celestial Games. All Rights Reserved.


#include "InteractParent.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Starlight_Island/Characters/PlayerCharacter/PlayerCharacter.h"
#include "Starlight_Island/Characters/PlayerCharacter/PlayerCharController.h"

// Sets default values
AInteractParent::AInteractParent() : InteractTriggerBox(nullptr)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Set up the root component
    RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = RootComp;
    
    // Interact Sphere
    InteractTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Interact Collider"));
    InteractTriggerBox->InitBoxExtent(FVector(100.f, 100.f, 100.f));
    InteractTriggerBox->SetRelativeLocation(FVector(0.f, -20.f, 0.f));
    InteractTriggerBox->ComponentTags.Add("Interact");
    InteractTriggerBox->SetupAttachment(RootComponent);
    InteractTriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    InteractTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractParent::OnOverlapBegin);
    InteractTriggerBox->OnComponentEndOverlap.AddDynamic(this, &AInteractParent::OnOverlapEnd);
    
    // Create the mesh component
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComp->SetupAttachment(RootComponent);

    // Set the interact action.
    bIsAllowedToInteract = true;
}

// Called when the game starts or when spawned
void AInteractParent::BeginPlay()
{
    Super::BeginPlay();
}

void AInteractParent::SetupInteractAction(AActor* OtherActor)
{
    if(const APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
    {
        PlayerCharacter = const_cast<APlayerCharacter*>(Player);
        PlayerController = PlayerCharacter->GetController<APlayerCharController>();
        EnhancedInputComponent = Player->GetEnhancedInputComponent();
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this,
            &AInteractParent::Interact);
    }
}

void AInteractParent::CheckWidgetVisibility()
{
    if(!InteractWidget && InteractRef.IsValid())
    {
        FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
        Streamable.RequestAsyncLoad(InteractRef.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this,
            &AInteractParent::SetInteractWidget));
        return;
    }

    if(const ESlateVisibility Visibility = InteractWidget->GetVisibility(); Visibility != ESlateVisibility::Hidden)
    {
        InteractWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    else if(Visibility == ESlateVisibility::Hidden && bIsAllowedToInteract)
    {
        InteractWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AInteractParent::SetInteractWidget()
{
    if(UClass* LoadedClass = InteractRef.LoadSynchronous())
    {
        if(UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), LoadedClass))
        {
            InteractWidget = Widget;
            InteractWidget->AddToViewport();
        }
    }
}

void AInteractParent::UnbindInteractAction(UObject* InOwner)
{
    if(InOwner && EnhancedInputComponent)
    {
        EnhancedInputComponent->ClearBindingsForObject(InOwner);
    }
}

void AInteractParent::BeginDestroy()
{
    Super::BeginDestroy();
}

void AInteractParent::LockPlayerControl(const float Seconds)
{
    if(PlayerController)
    {
        PlayerCharacter->StunCharacter(Seconds);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Player Controller is null"));
    }
}

void AInteractParent::Interact()
{
}

void AInteractParent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
}

void AInteractParent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
