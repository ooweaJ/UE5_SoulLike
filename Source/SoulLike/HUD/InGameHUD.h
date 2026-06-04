// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* MainUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* WidgetClass;
};
