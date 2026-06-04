// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Controller/PlayerController/LoadingPlayerController.h"
#include "UI/Loading/LoadingScreenWidget.h"

ALoadingPlayerController::ALoadingPlayerController()
{
	{
		static ConstructorHelpers::FClassFinder<ULoadingScreenWidget> LoadingScreenWidgetObject(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/_dev/UI/Loading/UI_Loading.UI_Loading_C'"));
		ensure(LoadingScreenWidgetObject.Class);
		LoadingScreenWidgetClass = LoadingScreenWidgetObject.Class;
	}
}

void ALoadingPlayerController::UpdateLoadComplete()
{
	if (LoadingScreenWidget)
	{
		LoadingScreenWidget->UpdateUIWhenLoadComplete();
	}
}

void ALoadingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (LoadingScreenWidgetClass)
	{
		LoadingScreenWidget = CreateWidget<ULoadingScreenWidget>(this, LoadingScreenWidgetClass);
	}
	LoadingScreenWidget->AddToViewport();
}