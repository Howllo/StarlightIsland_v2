// Copyrighted Celestial Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharController.generated.h"

/**
 * Player controller that only communicates between the server and the client.
 */
UCLASS()
class STARLIGHT_ISLAND_API APlayerCharController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APlayerCharController();
};
