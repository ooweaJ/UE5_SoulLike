// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Loading/LoadingScreenWidget.h"
#include "Components/TextBlock.h"
#include "Components/CircularThrobber.h"

void ULoadingScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULoadingScreenWidget::UpdateUIWhenLoadComplete()
{
	LoadingTextBlock->SetText(FText::FromString("Complete!"));
	LoadingCircularThrobber->SetVisibility(ESlateVisibility::Hidden);
}

