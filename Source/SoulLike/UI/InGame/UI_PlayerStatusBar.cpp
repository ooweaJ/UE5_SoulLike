// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame/UI_PlayerStatusBar.h"
#include "UI/InGame/UI_StackedProgressBar.h"
#include "Components/SizeBox.h"
void UUI_PlayerStatusBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	StackedProgressBar->Percent = Percent;

	float Width = InWidthOverride / StackedProgressBar->WidthOverride;
	Size->SetWidthOverride(Width);
}