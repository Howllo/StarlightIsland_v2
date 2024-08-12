// Copyrighted Celestial Games. All Rights Reserved.


#include "ItemActor.h"
#include "InventoryComponent.h"
#include "Item.h"
#include "Components/SphereComponent.h"
#include "Starlight_Island/Characters/PlayerCharacter/PlayerCharacter.h"
#include "Starlight_Island/Enumeration/EPlayerInventoryStatus.h"
#include "Starlight_Island/NativeTags/CollisionTag.h"

AItemActor::AItemActor()
{
	// Replication
	bReplicates = true;
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup Player Detection Sphere
	DetectPlayerRadius = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetect"));
	DetectPlayerRadius->SetSphereRadius(500.f);
	DetectPlayerRadius->ComponentTags.Add(CollisionTag::Detection.GetTag().GetTagName());
	DetectPlayerRadius->SetCollisionProfileName(TEXT("Trigger"));
	DetectPlayerRadius->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlapBegin);
	
	// Setup Interact Sphere
	InteractWithPlayer = CreateDefaultSubobject<USphereComponent>(TEXT("Interact"));
	InteractWithPlayer->SetSphereRadius(75.f);
	InteractWithPlayer->ComponentTags.Add(CollisionTag::Interact.GetTag().GetTagName());
	InteractWithPlayer->SetCollisionProfileName(TEXT("Trigger"));
	InteractWithPlayer->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlapBegin);

	// Setup Static Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(Mesh);

	// Setup Interp Speed
	InterpSpeed = 5.f;

	// Start Follow Player Timers
	TimerToStartFollow = 1.f;
	RestartFollowTimer = 10.f;

	// Setup Increase Interp Speed
	TimerToIncreaseInterpSpeed = 1.f;
	InterpSpeedIncrease = 1.f;
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	InteractWithPlayer->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	DetectPlayerRadius->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);

	// Start Timer to Follow Player.
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(ActivateFollow, this, &AItemActor::FollowPlayer,
		TimerToStartFollow, false);

	// Start Timer to Increase Interp Speed.
	TimerManager.SetTimer(IncreaseInterpSpeed, this, &AItemActor::IncreaseSpeed,
		TimerToIncreaseInterpSpeed, true);
}

void AItemActor::FollowPlayer()
{
	bIsTracking = true;
}

void AItemActor::IncreaseSpeed()
{
	InterpSpeed += InterpSpeedIncrease;
}

void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float dist = 0.f;
	APlayerCharacter* target = nullptr;
	for(APlayerCharacter* Target : PossibleTargets)
	{
		float tempDist = FVector::Distance(Target->GetActorLocation(), GetActorLocation());
		if(tempDist < dist || dist == 0.f)
		{
			dist = tempDist;
			target = Target;
		}
	}

	if( (target && !InventoryFull.Contains(target) && bIsTracking))
	{
		FVector InterLoc = FMath::VInterpTo(GetActorLocation(), target->GetActorLocation(),
			DeltaTime, InterpSpeed);
		SetActorLocation(InterLoc);
	}
}

void AItemActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APlayerCharacter* Player = nullptr;
    if(OtherActor->IsA(APlayerCharacter::StaticClass()))
    {
        Player = Cast<APlayerCharacter>(OtherActor);
    }
    
	if(Player && Player->PlayerTag == CollisionTag::Player &&
	    OverlappedComp->ComponentHasTag(CollisionTag::Detection.GetTag().GetTagName()))
	{
		PossibleTargets.Add(Player);
	}
	else if(Player && Player->PlayerTag == CollisionTag::Player &&
	    OverlappedComp->ComponentHasTag(CollisionTag::Interact.GetTag().GetTagName()))
	{
		if(Player->GetInventory())
		{
			if(Player->GetInventory()->Add(ItemHolder))
			{
				ItemHolder = nullptr;
				Destroy();
			}
			else
			{
				if(Player->GetInventory()->CheckInventoryFull())
				{
					InventoryFull[Player] = EPlayerInventoryStatus::Full;
				}
				
				bIsTracking = false;
				GetWorld()->GetTimerManager().SetTimer(ActivateFollowAgain, this,
					&AItemActor::FollowPlayer, RestartFollowTimer, false);
			}
		}
	}
}

void AItemActor::SetItemHolder(UItem* Item)
{
	if(Item && Mesh)
	{
		ItemHolder = Item;
		Mesh->SetStaticMesh(Item->ItemMesh);

		// Start Timer to Despawn Item Actor.
		GetWorld()->GetTimerManager().SetTimer(DespawnTimer, this,
			&AItemActor::DespawnItemActor, 300.f, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Item is nullptr in AITEmActor."))
	}
}
