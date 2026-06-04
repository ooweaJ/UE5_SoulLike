// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GameMode/MenuGameModeBase.h"
#include "Actor/Controller/PlayerController/MenuPlayerController.h"

AMenuGameModeBase::AMenuGameModeBase()
{
	PlayerControllerClass = AMenuPlayerController::StaticClass(); 
}

void AMenuGameModeBase::BeginPlay()
{
	Super::BeginPlay(); 
}

void AMenuGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason); 
}