// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/MenuInterface.h"
#include "UI_SessionWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UUI_SessionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup();
	void Teardown();

	FORCEINLINE void SetMenuInterface(IMenuInterface* InMenuInterface) { MenuInterface = InMenuInterface; }

protected:
	IMenuInterface* MenuInterface;
};
