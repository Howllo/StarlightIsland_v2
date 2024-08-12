// Copyrighted Celestial Games. All Rights Reserved.


#include "StarlightGameMode.h"
#include "PlayerCharState.h"
#include "PlayerHUD.h"
#include "StarlightGameState.h"
#include "Starlight_Island/Characters/PlayerCharacter/PlayerCharController.h"
#include "UObject/ConstructorHelpers.h"

AStarlightGameMode::AStarlightGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Blueprints/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Set Default Player State
	PlayerStateClass = APlayerCharState::StaticClass();

	// Set Default Player Controller
	PlayerControllerClass = APlayerCharController::StaticClass();

	// Set Default GameState
	GameStateClass = AStarlightGameState::StaticClass();

	// Set HUD
	HUDClass = APlayerHUD::StaticClass();
	
}