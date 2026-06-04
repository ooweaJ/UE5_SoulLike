// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_PlayerStatusBar.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UUI_PlayerStatusBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USizeBox* Size;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UUI_StackedProgressBar* StackedProgressBar;

	UPROPERTY(BlueprintReadWrite)
	float Percent;
	UPROPERTY(BlueprintReadWrite)
	float InWidthOverride;
};
