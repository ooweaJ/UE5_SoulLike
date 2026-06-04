// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UWidgetSwitcher; 
class UBackgroundBlur; 
class UImage; 
class UButton; 
class UVerticalBox; 
class UTextBlock; 
class UConfigMenuWidget; 

UENUM()
enum class EPauseMenu : uint8
{
	Pause,
	Config
};

UCLASS()
class SOULLIKE_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override; 

	virtual void NativeDestruct() override;

public:
	UWidgetSwitcher* GetPauseMenuWidgetSwitcher() const { return PauseMenuWidgetSwitcher; }

public:
	UFUNCTION()
	void OnResumeButtonClicked(); 

	UFUNCTION()
	void OnConfigButtonClicked();

	UFUNCTION()
	void OnConnectExitButtonClicked();

	UFUNCTION()
	void OnGameExitButtonClicked();

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* PauseMenuWidgetSwitcher; 

// Pause Menu 
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBackgroundBlur* BackGroundBlur; 

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* PauseMenuImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* VerticalBox; 

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ResumeButton; 

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ConfigButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ConnectExitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GameExitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PauseMenuText; 


// Config Menu
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UConfigMenuWidget* ConfigMenu; 
};
