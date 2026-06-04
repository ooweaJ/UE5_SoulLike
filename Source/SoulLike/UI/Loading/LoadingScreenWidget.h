// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenWidget.generated.h"

class UTextBlock;
class UCircularThrobber;

UCLASS()
class SOULLIKE_API ULoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateUIWhenLoadComplete();

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* LoadingTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCircularThrobber* LoadingCircularThrobber;
};