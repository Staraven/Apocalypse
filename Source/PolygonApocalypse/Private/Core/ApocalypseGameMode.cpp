#include "Core/ApocalypseGameMode.h"
#include "Character/ApocalypseCharacter.h"
#include "Core/ApocalypsePlayerController.h"

AApocalypseGameMode::AApocalypseGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClass(
	   TEXT("/Game/Blueprints/Character/BP_ApocalypseCharacter"));
	if (PlayerPawnClass.Class)
	{
		DefaultPawnClass = PlayerPawnClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(
	   TEXT("/Game/BP_ApocalypsePlayerController"));
	if (PlayerControllerBPClass.Class)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}