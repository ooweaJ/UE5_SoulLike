// Fill out your copyright notice in the Description page of Project Settings.
#include "Actor/Controller/PlayerController/MenuPlayerController.h"
#include "ASGameInstance.h"

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay(); 

	if (UASGameInstance* ASGameInstance = Cast<UASGameInstance>(GetGameInstance()))
	{
		ASGameInstance->LoadinGameMenu(); 
	}
}
