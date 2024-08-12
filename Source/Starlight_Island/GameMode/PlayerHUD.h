// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class STARLIGHT_ISLAND_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> InteractWidget;
    
public:
    APlayerHUD();
};
