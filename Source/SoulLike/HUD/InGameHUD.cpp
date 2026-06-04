// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "UI/InGame/UI_MainInGame.h"

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
	MainUI = CreateWidget(PC, WidgetClass);

	MainUI->AddToViewport();
}
