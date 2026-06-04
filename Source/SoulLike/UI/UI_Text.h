// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Text.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UUI_Text : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* Image;
};
