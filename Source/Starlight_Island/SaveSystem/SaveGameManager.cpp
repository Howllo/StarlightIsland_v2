﻿// Copyrighted Celestial Games. All Rights Reserved.


#include "SaveGameManager.h"


// Sets default values
ASaveGameManager::ASaveGameManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASaveGameManager::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void ASaveGameManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

