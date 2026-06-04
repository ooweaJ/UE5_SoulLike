// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LoadingPlayerController.generated.h"

class ULoadingScreenWidget;

UCLASS()
class SOULLIKE_API ALoadingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALoadingPlayerController();

public:
	void UpdateLoadComplete();

protected:
	virtual void BeginPlay() override;

private:
	TSubclassOf<ULoadingScreenWidget> LoadingScreenWidgetClass;

	UPROPERTY()
	ULoadingScreenWidget* LoadingScreenWidget;
};