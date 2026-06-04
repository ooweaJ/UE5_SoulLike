// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MenuWidget.generated.h"

class UCharacterSelectWidget; 
class UConfigMenuWidget; 
class UWidgetSwitcher; 
class UUniformGridPanel; 
class UButton; 
class UTextBlock; 

UENUM() 
enum class EMenu : uint8
{
	Main,
	CharacterSelect,
	Config
};

UCLASS()
class SOULLIKE_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override; 

	virtual void NativeDestruct() override; 

	void SetButtonNormalStyle(UButton* InButton, FLinearColor InLinearColor = FLinearColor(1.f, 1.f, 1.f, 0.f)); 
	void Setup();
public:
	UFUNCTION()
	void OnConnectButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UFUNCTION()
	void OnConfigButtonClicked();

	UFUNCTION()
	void OnSelectStartButtonClicked();

	UFUNCTION()
	void OnSelectGoBackButtonClicked();

	UFUNCTION()
	void OnCharacterClassButtonClicked(UCharacterSelectWidget* InWidget); 

	
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ConnectButton; 

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ConfigButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SelectStartButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SelectGoBackButton;

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher; 

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UUniformGridPanel* CharacterGridPanel; 

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UConfigMenuWidget* ConfigMenu; 

	UPROPERTY()
	TSubclassOf<UCharacterSelectWidget> CharacterSelectWidgetClass; 

	UPROPERTY()
	TSubclassOf<UConfigMenuWidget> ConfigMenuWidgetClass; 

protected:
	virtual void PopulateGrid(); 

private:
	UButton* CurrentClickedButton = nullptr; 
	UButton* LastClickedButton = nullptr; 
};
