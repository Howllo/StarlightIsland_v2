// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveGameManager.generated.h"

UCLASS()
class STARLIGHT_ISLAND_API ASaveGameManager : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASaveGameManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
