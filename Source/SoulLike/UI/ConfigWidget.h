// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConfigWidget.generated.h"

class UTextBlock; 
class UButton; 


UENUM()
enum class EConfigName : uint8
{
	Texture, 
	AntiAliasing, 
	Shadow, 
	Shading, 
	Effect,
	GlobalIllumination, 
};


UENUM()
enum class EConfigQuality : uint8
{
	lowest, 
	low, 
	medium,
	high,
	highest, 
	_End
};

UCLASS()
class SOULLIKE_API UConfigWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override; 

	virtual void NativeDestruct() override; 

public:
	// Config Name 
	uint8 GetConfigNameIndex() const { return ConfigNameIndex; }

	void SetConfigNameText(EConfigName ConfigName); 

	void SetConfigNameIndex(uint8 InIndex) { ConfigNameIndex = InIndex; }

	// Config Quality 
	uint8 GetConfigQualityIndex() const { return CurrentConfigQualityIndex; }

	void SetConfigQualityText(EConfigQuality ConfigQuality); 

	void SetConfigQualityIndex(uint8 InIndex) { CurrentConfigQualityIndex = InIndex; }

	// Config Last Quality 
	uint8 GetLastQualityIndex() const { return LastConfigQualityIndex; }

	void SetLastQualityIndex(uint8 InIndex) { LastConfigQualityIndex = InIndex; }

	// Config Initialization
	bool GetbInit() const { return bInit; }

	void SetbInit(bool InInit) { bInit = InInit; }

public:
	UFUNCTION()
	void OnConfigMinusButtonClicked(); 

	UFUNCTION()
	void OnConfigPlusButtonClicked(); 

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ConfigNameTextBlock; 

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ConfigMinusButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ConfigQualityTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ConfigPlusButton;


private:
	uint8 ConfigNameIndex; 
			
	uint8 CurrentConfigQualityIndex; 

	uint8 LastConfigQualityIndex; 

	bool bInit = false; 
};
