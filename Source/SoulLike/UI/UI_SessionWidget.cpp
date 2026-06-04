// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_SessionWidget.h"

void UUI_SessionWidget::Setup()
{
	AddToViewport();
	SetIsFocusable(true);

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); 

	UWorld* world = GetWorld();
	if (world == nullptr) return;

	APlayerController* controller = world->GetFirstPlayerController();
	if (controller == nullptr) return;
	controller->SetInputMode(inputMode);
	controller->bShowMouseCursor = true;
}

void UUI_SessionWidget::Teardown()
{
	RemoveFromParent();
	SetIsFocusable(false);

	FInputModeGameOnly inputMode;

	UWorld* world = GetWorld();
	if (world == nullptr) return;

	APlayerController* controller = world->GetFirstPlayerController();
	if (controller == nullptr) return;
	controller->SetInputMode(inputMode);
	controller->bShowMouseCursor = false;
}
