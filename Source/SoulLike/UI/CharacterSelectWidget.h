// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "ASGameInstance.h"
#include "CharacterSelectWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnCharacterSelectButtonEvent, UCharacterSelectWidget*);

UCLASS()
class SOULLIKE_API UCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()

	friend class UMenuWidget; 

public:
	void SetImageBrush(const FString MaterialPath);
	bool SetCharacterClassWidget(ECharacterClass InCharacterClass);
	ECharacterClass GetCharacterClassName() const; 

protected:
	virtual void NativeOnInitialized() override; 

	UFUNCTION()
	void OnCharacterSelectButtonClicked(); 

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CharacterClassTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* CharacterImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CharacterSelectButton;

	ECharacterClass CharacterClassName; 

	FOnCharacterSelectButtonEvent CharacterSelectButtonClicked;
};
