// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UUI_ServerRow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup(class UUI_ServerMenu* InParent, uint32 InIndex);

private:
	UFUNCTION()
	void OnClicked();
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUser;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Connection;

	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton;

	UPROPERTY(BlueprintReadOnly)
	bool bSelected = false;

private:
	class UUI_ServerMenu* Parent;
	uint32 Index;
	FLinearColor  RandomColor;
};
