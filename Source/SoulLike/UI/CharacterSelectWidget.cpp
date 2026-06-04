// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterSelectWidget.h"
#include "Materials/MaterialInterface.h"

void UCharacterSelectWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized(); 

	CharacterSelectButton->OnClicked.AddDynamic(this, &ThisClass::OnCharacterSelectButtonClicked); 
}

void UCharacterSelectWidget::OnCharacterSelectButtonClicked()
{
	if (CharacterSelectButtonClicked.IsBound())
	{
		CharacterSelectButtonClicked.Execute(this); 
	}
}

void UCharacterSelectWidget::SetImageBrush(const FString MaterialPath)
{
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, *MaterialPath);

	if (Material)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(Material);
		Brush.ImageSize = FVector2D(400.f, 700.f); 
		CharacterImage->SetBrush(Brush);
	}
}

bool UCharacterSelectWidget::SetCharacterClassWidget(ECharacterClass InCharacterClass)
{
	//if (!type) { check(false); return false; }

	CharacterClassName = InCharacterClass; 

	FText CharacterClassNameText; 

	switch (InCharacterClass)
	{
	case ECharacterClass::Warrior:
	{
		CharacterClassNameText = FText::FromString(TEXT("Warrior"));
		const FString MaterialPath = TEXT("/Script/Engine.Material'/Game/_dev/Actor/Character/Player/Warrior/MT_Warrior.MT_Warrior'");
		SetImageBrush(MaterialPath); 

		break;
	}
		

	case ECharacterClass::Assassin:
	{
		CharacterClassNameText = FText::FromString(TEXT("Assassin"));
		const FString MaterialPath = TEXT("/Script/Engine.Material'/Game/_dev/Actor/Character/Player/Assassin/MT_Assassin.MT_Assassin'");
		SetImageBrush(MaterialPath);

		break;
	}
	case ECharacterClass::Katana:
	{
		CharacterClassNameText = FText::FromString(TEXT("Katana"));
		const FString MaterialPath = TEXT("/Script/Engine.Material'/Game/_dev/Actor/Character/Player/Katana/MT_Katana.MT_Katana'");
		SetImageBrush(MaterialPath);

		break;
	}
		 

	case ECharacterClass::Spearman:
	{
		CharacterClassNameText = FText::FromString(TEXT("Spearman"));
		const FString MaterialPath = TEXT("/Script/Engine.Material'/Game/_dev/Actor/Character/Player/Spear/MT_Spear.MT_Spear'");
		SetImageBrush(MaterialPath);

		break;
	}
	}

	CharacterClassTextBlock->SetText(CharacterClassNameText); 

	return true; 
}

ECharacterClass UCharacterSelectWidget::GetCharacterClassName() const
{
	return CharacterClassName;
}
