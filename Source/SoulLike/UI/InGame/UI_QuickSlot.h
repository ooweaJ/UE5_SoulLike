// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_QuickSlot.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API UUI_QuickSlot : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* UpperSlot;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* LeftSlot;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* RightSlot;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* LowerSlot;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PotionAmount;

private:
	struct FPotion* PlayerPotion;
};
