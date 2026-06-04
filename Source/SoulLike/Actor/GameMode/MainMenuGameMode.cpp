#include "Actor/GameMode/MainMenuGameMode.h"

void AMainMenuGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NumberOfPlayers == 0)
	{

	}
}
