// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame/UI_StackedProgressBar.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Components/Image.h"

void UUI_StackedProgressBar::NativePreConstruct()
{
	MaterialInstanceDynamic = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, MaterialInstance);
	ProgressBarImage->SetBrushFromMaterial(MaterialInstanceDynamic);
}

void UUI_StackedProgressBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	MaterialInstanceDynamic->SetScalarParameterValue(TEXT("TopProgress"), Percent);
	
	float Temp = MaterialInstanceDynamic->K2_GetScalarParameterValue(TEXT("BottomProgress"));
	float Result = FMath::FInterpTo(Temp, Percent, InDeltaTime, InterpSpeed);

	MaterialInstanceDynamic->SetScalarParameterValue(TEXT("BottomProgress"), Result);
}
