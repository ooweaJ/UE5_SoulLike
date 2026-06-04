// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConfigMenuWidget.generated.h"

class UVerticalBox;
class UConfigWidget; 
class UButton; 

UCLASS()
class SOULLIKE_API UConfigMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override; 

	virtual void NativeDestruct() override; 

	void InitVerticalBox(); 

	UFUNCTION()
	void OnApplyButtonClicked();

	UFUNCTION()
	void OnGoBackButtonClicked(); 

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* ConfigMenuVerticalBox; 

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UConfigWidget* ConfigTexture; 

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UConfigWidget* ConfigAntiAliasing; 

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UConfigWidget* ConfigShadow;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UConfigWidget* ConfigShading;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UConfigWidget* ConfigEffect;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UConfigWidget* ConfigGlobalIllumination;

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ApplyButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GoBackButton; 
};
