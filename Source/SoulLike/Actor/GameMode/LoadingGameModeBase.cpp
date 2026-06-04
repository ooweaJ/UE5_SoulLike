// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GameMode/LoadingGameModeBase.h"
#include "Actor/Controller/PlayerController/LoadingPlayerController.h"

ALoadingGameModeBase::ALoadingGameModeBase()
{
	bUseSeamlessTravel = true; 

	PlayerControllerClass = ALoadingPlayerController::StaticClass();
}
