// Copyrighted Celestial Games. All Rights Reserved.

#include "TreeCutting.h"
#include "Components/CapsuleComponent.h"
#include "Inventory/ItemActor.h"
#include "Starlight_Island/Characters/PlayerCharacter/PlayerCharacter.h"
#include "Starlight_Island/GameMode/StarlightGameInstance.h"
#include "Starlight_Island/GameMode/StarlightGameState.h"
#include "Starlight_Island/Misc/ItemFactory.h"
#include "Starlight_Island/NativeTags/CollisionTag.h"

// Sets default values for this component's properties
UTreeCutting::UTreeCutting()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Hit Register
	HitRegister = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hit Register"));
	HitRegister->InitCapsuleSize(70.f, 200.f);
	HitRegister->SetCollisionProfileName(TEXT("Trigger"));
	HitRegister->OnComponentBeginOverlap.AddDynamic(this, &UTreeCutting::OnOverlapBegin);
	HitRegister->ComponentTags.Add(CollisionTag::Gather.GetTag().GetTagName());
	
	// Set the Tree Health.
	TreeTotalHealth = 1.f;

	// Force when falling over.
	ForceFallingOver = 10.f;

	// Set Stuck Allowed Timer;
	StuckTimer = 5.f;

	// Set Defaults For Tree Rotation Limits.
	TreeRotationLimitsRoll.X = -55.f;
	TreeRotationLimitsRoll.Y = 55.f;
	TreeRotationLimitsPitch.X = -55.f;
	TreeRotationLimitsPitch.Y = 55.f;

	// Set the Total Amount of Spawn.
	StartingItemQuantity = 1;
}

// Called when the game starts
void UTreeCutting::BeginPlay()
{
	Super::BeginPlay();

	// Get the Game State.
	StarlightGameState = Cast<AStarlightGameState>(GetWorld()->GetGameState());

	// Get Game Instance
	StarlightGameInstance = Cast<UStarlightGameInstance>(GetWorld()->GetGameInstance());
	
	// Create Spawn Location
	CreateSpawnLocations();

	// Draw Debug Sphere
	if(bDrawDebugMarker)
	{
		DrawDebugMarker();
	}
}

void UTreeCutting::OnRegister()
{
	Super::OnRegister();
	
	OwnerOfComponent = GetOwner();
	if(OwnerOfComponent)
	{
		USceneComponent* Root = OwnerOfComponent->GetRootComponent();
		
		HitRegister->SetupAttachment(Root);
		HitRegister->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	}

	// Set Tree's Current HP.
	TreeCurrentHealth = TreeTotalHealth;

	// Get Mesh and set mesh information.
	if(OwnerOfComponent)
	{
		Mesh = OwnerOfComponent->GetComponentByClass<UStaticMeshComponent>();
		Mesh->SetMobility(EComponentMobility::Movable);
		
		if(Mesh && !Mesh->ComponentHasTag("Gather"))
		{
			Mesh->ComponentTags.Add(FName("Gather"));
		}
		Mesh->SetAngularDamping(10.0f);
		Mesh->SetMassOverrideInKg(NAME_None, 3000.f, true);
	}
}

void UTreeCutting::DrawDebugMarker()
{
	for(const USceneComponent* SpawnLocation : SpawnLocations)
	{
		DrawDebugSphere(GetWorld(), SpawnLocation->GetComponentLocation(),
			15.f, 16, FColor::Green, true, 120, 0, 1);
	}
}

void UTreeCutting::CreateItem(const int32 Index)
{
	if (StarlightGameState && GetWorld())
	{
		if(const AItemActor* ItemActor = StarlightGameInstance->GetItemFactory()->CreateItemActor(ItemID,
			SpawnLocations[Index]->GetComponentLocation(),OwnerOfComponent->GetActorRotation(),
			StartingItemQuantity, GetWorld()))
		{
			ItemActor->GetMesh()->SetSimulatePhysics(true);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ItemActor is nullptr in UTreeCutting."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StarlightGameState is nullptr."));
	}
}

void UTreeCutting::DestroyTree(bool bIsStuck)
{
	FRotator Rotator = Mesh->GetRelativeRotation();
	if( ((Rotator.Roll > TreeRotationLimitsRoll.X && Rotator.Roll < TreeRotationLimitsRoll.Y) &&
		(Rotator.Pitch > TreeRotationLimitsPitch.X && Rotator.Pitch < TreeRotationLimitsPitch.Y)) ||
		bIsStuck)
	{
		return;
	}
	
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(RotationCheckTimerHandle);
	TimerManager.ClearTimer(StuckTimerDestroy);
	
	
	// Spawn and Calculate the FVector spawn location.
	if (Mesh && TotalAmountSpawn > 0)
	{
		for (int32 i = 0; i < TotalAmountSpawn; i++)
		{
			CreateItem(i);
		}
	}
	
	// Destroy the AActor.
	OwnerOfComponent->Destroy();
}

void UTreeCutting::DealDamage(float Damage)
{
	TreeCurrentHealth -= Damage;
}

void UTreeCutting::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	// TODO: Implement the logic for dealing damage to the tree instead of using player character body.
    if(!PlayerCharacter && OtherActor->IsA(APlayerCharacter::StaticClass()))
    {
        PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
    }
    
	if(PlayerCharacter && PlayerCharacter->PlayerTag == CollisionTag::Player &&
	    OverlappedComp->ComponentHasTag(CollisionTag::Gather.GetTag().GetTagName()))
	{
		DealDamage(10.f);
		
		if(TreeCurrentHealth <= 0)
		{
			Mesh->SetSimulatePhysics(true);
			Mesh->AddForce(PlayerCharacter->GetActorForwardVector() + ForceFallingOver);
			FTimerManager& TimerManager = GetWorld()->GetTimerManager();
			
			TreeDestroyTimerDel.BindUFunction(this, FName("DestroyTree"), false);
			TimerManager.SetTimer(RotationCheckTimerHandle, TreeDestroyTimerDel, 0.20f,
				true);

			TreeStuckTimerDel.BindUFunction(this, FName("DestroyTree"), true);
			TimerManager.SetTimer(StuckTimerDestroy,TreeStuckTimerDel,StuckTimer,
				false);

			if(bDrawDebugMarker)
			{
				TimerManager.SetTimer(DebugTracker, this, &UTreeCutting::DrawDebugMarker, 0.2f,
					true);
			}
		}
	}
}

void UTreeCutting::CreateSpawnLocations()
{
	// Distance between the spawn points.
	float Interval = TotalDistanceForSpawning - (SizeOfItem * TotalAmountSpawn) / (TotalAmountSpawn - 1);
	
	for(int32 i = 0; i < TotalAmountSpawn; i++)
	{
		FVector SpawnPosition = FVector(0.0f, 0.0f, MinimumDistanceForSpawning + (Interval * i));
		FName ComponentName = *FString::Printf(TEXT("Spawn Point %d"), i);

		if(USceneComponent* SpawnLocation = NewObject<USceneComponent>(this, ComponentName))
		{
			SpawnLocations.Add(SpawnLocation);
			SpawnLocation->AttachToComponent(OwnerOfComponent->GetRootComponent(),
				FAttachmentTransformRules::KeepRelativeTransform, ComponentName);
			SpawnLocation->RegisterComponent();
			SpawnLocation->SetRelativeLocation(SpawnPosition);
		}
	}
}
