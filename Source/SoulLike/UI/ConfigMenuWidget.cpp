// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ConfigMenuWidget.h"
#include "UI/ConfigWidget.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Scalability.h"

void UConfigMenuWidget::NativeConstruct()
{
	Super::NativeConstruct(); 

	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(this, &UConfigMenuWidget::OnApplyButtonClicked); 
	}

	if (GoBackButton)
	{
		GoBackButton->OnClicked.AddDynamic(this, &UConfigMenuWidget::OnGoBackButtonClicked); 
	}

	InitVerticalBox(); 
	
}

void UConfigMenuWidget::NativeDestruct()
{
	Super::NativeDestruct(); 

	if (ApplyButton)
	{
		ApplyButton->OnClicked.RemoveDynamic(this, &UConfigMenuWidget::OnApplyButtonClicked);
	}

	if (GoBackButton)
	{
		GoBackButton->OnClicked.RemoveDynamic(this, &UConfigMenuWidget::OnGoBackButtonClicked);
	}

}

void UConfigMenuWidget::InitVerticalBox()
{
	const TArray<UWidget*>& ConfigChildren = ConfigMenuVerticalBox->GetAllChildren();
	const uint8 NumChildren = ConfigChildren.Num(); 

	for (uint8 i = 0; i < NumChildren; ++i)
	{
		if (UConfigWidget* Config = Cast<UConfigWidget>(ConfigChildren[i]))
		{
			Config->SetConfigNameText(static_cast<EConfigName>(i));
			Config->SetConfigNameIndex(i);

			uint8 LastConfigQualityIndex = Config->GetLastQualityIndex(); 

			if (Config->GetbInit())
			{
				Config->SetConfigQualityText(static_cast<EConfigQuality>(LastConfigQualityIndex));
				Config->SetConfigQualityIndex(LastConfigQualityIndex);
			}
			else
			{
				Scalability::FQualityLevels QualityLevels = Scalability::GetQualityLevels(); 
				const EConfigName& ConfigName = static_cast<EConfigName>(i);

				switch (ConfigName)
				{
				case EConfigName::Texture:
				{
					Config->SetConfigQualityText(static_cast<EConfigQuality>(QualityLevels.TextureQuality)); 
					Config->SetConfigQualityIndex(QualityLevels.TextureQuality);
					break; 
				}
				case EConfigName::AntiAliasing:
				{
					Config->SetConfigQualityText(static_cast<EConfigQuality>(QualityLevels.AntiAliasingQuality));
					Config->SetConfigQualityIndex(QualityLevels.AntiAliasingQuality);
					break;
				}
				case EConfigName::Shadow:
				{
					Config->SetConfigQualityText(static_cast<EConfigQuality>(QualityLevels.ShadowQuality));
					Config->SetConfigQualityIndex(QualityLevels.ShadowQuality);
					break;
				}
				case EConfigName::Shading:
				{
					Config->SetConfigQualityText(static_cast<EConfigQuality>(QualityLevels.ShadingQuality));
					Config->SetConfigQualityIndex(QualityLevels.ShadingQuality);
					break;
				}
				case EConfigName::Effect:
				{
					Config->SetConfigQualityText(static_cast<EConfigQuality>(QualityLevels.EffectsQuality));
					Config->SetConfigQualityIndex(QualityLevels.EffectsQuality);
					break;
				}
				case EConfigName::GlobalIllumination:
				{
					Config->SetConfigQualityText(static_cast<EConfigQuality>(QualityLevels.GlobalIlluminationQuality));
					Config->SetConfigQualityIndex(QualityLevels.GlobalIlluminationQuality);
					break;
				}
				}
				Config->SetbInit(true); 
			}
		}
	}
}

void UConfigMenuWidget::OnApplyButtonClicked()
{
	const TArray<UWidget*>& ConfigChildren = ConfigMenuVerticalBox->GetAllChildren();
	const uint8 NumChildren = ConfigChildren.Num();

	Scalability::FQualityLevels QualityLevels = Scalability::GetQualityLevels();

	for (uint8 i = 0; i < NumChildren; ++i)
	{
		if (UConfigWidget* Config = Cast<UConfigWidget>(ConfigChildren[i]))
		{
			const uint8& CurrentQualityIndex = Config->GetConfigQualityIndex(); 
			const uint8& CurrentNameIndex = Config->GetConfigNameIndex(); 
			const EConfigName& ConfigName = static_cast<EConfigName>(CurrentNameIndex); 


			switch (ConfigName)
			{
			case EConfigName::Texture:
			{
				QualityLevels.TextureQuality = CurrentQualityIndex;
				break;
			}
			case EConfigName::AntiAliasing:
			{
				QualityLevels.AntiAliasingQuality = CurrentQualityIndex; 
				break;
			}
			case EConfigName::Shadow:
			{
				QualityLevels.ShadowQuality = CurrentQualityIndex; 
				break; 
			}
			case EConfigName::Shading:
			{
				QualityLevels.ShadingQuality = CurrentQualityIndex; 
				break; 
			}
			case EConfigName::Effect:
			{
				QualityLevels.EffectsQuality = CurrentQualityIndex; 
				break;
			}
			case EConfigName::GlobalIllumination:
			{
				QualityLevels.GlobalIlluminationQuality = CurrentQualityIndex; 
				break; 
			}
			}

			Config->SetLastQualityIndex(CurrentQualityIndex); 
		}
	}
	Scalability::SetQualityLevels(QualityLevels);
}

void UConfigMenuWidget::OnGoBackButtonClicked()
{
	UWidget* ParentWidget = GetParent(); 

	if (UWidgetSwitcher* ParentSwitcher = Cast<UWidgetSwitcher>(ParentWidget))
	{
		ParentSwitcher->SetActiveWidgetIndex(0); 
	}
}
