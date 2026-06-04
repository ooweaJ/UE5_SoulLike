// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ConfigWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UConfigWidget::NativeConstruct()
{
	Super::NativeConstruct(); 

	if (ConfigMinusButton)
	{
		ConfigMinusButton->OnClicked.AddDynamic(this, &UConfigWidget::OnConfigMinusButtonClicked); 
	}

	if (ConfigPlusButton)
	{
		ConfigPlusButton->OnClicked.AddDynamic(this, &UConfigWidget::OnConfigPlusButtonClicked); 
	}
}

void UConfigWidget::NativeDestruct()
{
	Super::NativeDestruct(); 

	if (ConfigMinusButton)
	{
		ConfigMinusButton->OnClicked.RemoveDynamic(this, &UConfigWidget::OnConfigMinusButtonClicked);
	}

	if (ConfigPlusButton)
	{
		ConfigPlusButton->OnClicked.RemoveDynamic(this, &UConfigWidget::OnConfigPlusButtonClicked);
	}
}

void UConfigWidget::SetConfigNameText(EConfigName ConfigName)
{
	if (!ConfigNameTextBlock) { check(false); return; }
	
	switch (ConfigName)
	{
	case EConfigName::Texture:
	{
		ConfigNameTextBlock->SetText(FText::FromString(TEXT("텍스처"))); 
		break; 
	}
	case EConfigName::AntiAliasing:
	{
		ConfigNameTextBlock->SetText(FText::FromString(TEXT("안티에일리어싱")));
		break;
	}
	case EConfigName::Shadow:
	{
		ConfigNameTextBlock->SetText(FText::FromString(TEXT("그림자")));
		break;
	}
	case EConfigName::Shading:
	{
		ConfigNameTextBlock->SetText(FText::FromString(TEXT("셰이딩")));
		break;
	}
	case EConfigName::Effect:
	{
		ConfigNameTextBlock->SetText(FText::FromString(TEXT("이펙트")));
		break;
	}
	case EConfigName::GlobalIllumination:
	{
		ConfigNameTextBlock->SetText(FText::FromString(TEXT("글로벌일루미네이션")));
		break;
	}
	}
}


void UConfigWidget::SetConfigQualityText(EConfigQuality ConfigQuality)
{
	if (!ConfigQualityTextBlock) { check(false); return; }

	switch (ConfigQuality)
	{
	case EConfigQuality::lowest:
	{
		ConfigQualityTextBlock->SetText(FText::FromString(TEXT("최하")));
		break; 
	}
	case EConfigQuality::low:
	{
		ConfigQualityTextBlock->SetText(FText::FromString(TEXT("하")));
		break;
	}
	case EConfigQuality::medium:
	{
		ConfigQualityTextBlock->SetText(FText::FromString(TEXT("중")));
		break;
	}
	case EConfigQuality::high:
	{
		ConfigQualityTextBlock->SetText(FText::FromString(TEXT("상")));
		break;
	}
	case EConfigQuality::highest:
	{
		ConfigQualityTextBlock->SetText(FText::FromString(TEXT("최상")));
		break;
	}
	}
}

void UConfigWidget::OnConfigMinusButtonClicked()
{
	if (CurrentConfigQualityIndex >= static_cast<uint8>(EConfigQuality::_End)) { check(false); return; }
	if (CurrentConfigQualityIndex == 0) { return; }

	SetConfigQualityText(static_cast<EConfigQuality>(--CurrentConfigQualityIndex));
}

void UConfigWidget::OnConfigPlusButtonClicked() 
{
	if (CurrentConfigQualityIndex == static_cast<uint8>(EConfigQuality::_End)) { check(false); return; }
	if (CurrentConfigQualityIndex == static_cast<uint8>(EConfigQuality::_End) - 1) { return; }

	SetConfigQualityText(static_cast<EConfigQuality>(++CurrentConfigQualityIndex));
}
