// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_StackedProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UUI_StackedProgressBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* ProgressBarImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* MaterialInstance;
	UMaterialInstanceDynamic* MaterialInstanceDynamic;

public:
	float Percent;
	float InterpSpeed = 1.35f;
	float WidthOverride = 2.f;
};
