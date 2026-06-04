// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuWidget.h"
#include "UI/CharacterSelectWidget.h"
#include "UI/ConfigMenuWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ASGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/LevelTransitionUtils.h"
#include "Styling/SlateTypes.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Load and Find CharacterSelectWidget
	LoadClass<UClass>(nullptr, 
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/_dev/UI/Menu/UI_CharacterSelect.UI_CharacterSelect_C'"), nullptr, LOAD_None, nullptr);

	CharacterSelectWidgetClass = FindObject<UClass>(
		nullptr,
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/_dev/UI/Menu/UI_CharacterSelect.UI_CharacterSelect_C'"));

	// Place character class on grid in CharacterSelectWidget
	PopulateGrid(); 

	// Load and Find CharacterSelectWidget
	LoadClass<UClass>(nullptr,
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/_dev/UI/Menu/UI_ConfigMenu.UI_ConfigMenu_C'"), nullptr, LOAD_None, nullptr);

	CharacterSelectWidgetClass = FindObject<UClass>(
		nullptr,
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/_dev/UI/Menu/UI_CharacterSelect.UI_CharacterSelect_C'"));


	// Bind a function pointer to the buttons
	if (ConnectButton)
	{
		ConnectButton->OnClicked.AddDynamic(this, &UMenuWidget::OnConnectButtonClicked);
	}

	if (ConfigButton)
	{
		ConfigButton->OnClicked.AddDynamic(this, &UMenuWidget::OnConfigButtonClicked); 
	}


	if (SelectStartButton)
	{
		SelectStartButton->OnClicked.AddDynamic(this, &UMenuWidget::OnSelectStartButtonClicked); 
	}

	if (SelectGoBackButton)
	{
		SelectGoBackButton->OnClicked.AddDynamic(this, &UMenuWidget::OnSelectGoBackButtonClicked); 
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UMenuWidget::OnExitButtonClicked);
	}

}

void UMenuWidget::NativeDestruct()
{
	Super::NativeDestruct(); 

	if (ConnectButton)
	{
		ConnectButton->OnClicked.RemoveDynamic(this, &UMenuWidget::OnConnectButtonClicked);
	}

	if (ConfigButton)
	{
		ConfigButton->OnClicked.RemoveDynamic(this, &UMenuWidget::OnConfigButtonClicked);
	}


	if (SelectStartButton)
	{
		SelectStartButton->OnClicked.RemoveDynamic(this, &UMenuWidget::OnSelectStartButtonClicked);
	}

	if (SelectGoBackButton)
	{
		SelectGoBackButton->OnClicked.RemoveDynamic(this, &UMenuWidget::OnSelectGoBackButtonClicked);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.RemoveDynamic(this, &UMenuWidget::OnExitButtonClicked);
	}
}

void UMenuWidget::SetButtonNormalStyle(UButton* InButton, FLinearColor InLinearColor)
{
	if (!InButton) { return; }

	FButtonStyle ButtonStyle = InButton->GetStyle(); 
	ButtonStyle.Normal.TintColor = FSlateColor(InLinearColor); 
	InButton->SetStyle(ButtonStyle); 
}

void UMenuWidget::Setup()
{
	AddToViewport();
	SetIsFocusable(true);

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	UWorld* world = GetWorld();
	if (world == nullptr) return;

	APlayerController* controller = world->GetFirstPlayerController();
	if (controller == nullptr) return;
	controller->SetInputMode(inputMode);
	controller->bShowMouseCursor = true;
}


void UMenuWidget::OnConnectButtonClicked()
{
	MenuSwitcher->SetActiveWidgetIndex(static_cast<uint8>(EMenu::CharacterSelect));
}

void UMenuWidget::OnExitButtonClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); 
	if (PlayerController)
	{
		if (PlayerController->IsLocalController())
		{
			UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
		}
	}
}

void UMenuWidget::OnConfigButtonClicked()
{
	MenuSwitcher->SetActiveWidgetIndex(static_cast<uint8>(EMenu::Config));
}

void UMenuWidget::OnSelectStartButtonClicked()
{
	if (!CurrentClickedButton) { return; }

	UCharacterSelectWidget* CharacterSelectWidget = CurrentClickedButton->GetTypedOuter<UCharacterSelectWidget>(); 

	if (!CharacterSelectWidget)
	{
		return;
	}

	FCharacterData Data; 
	Data.CharacterClassName = CharacterSelectWidget->CharacterClassName;

	FString LevelPath = TEXT("/Game/_dev/Level/SessionWorld");
	LevelTransitionUtils::OpenLevelWithData(this, LevelPath, Data); 

}

void UMenuWidget::OnSelectGoBackButtonClicked()
{
	MenuSwitcher->SetActiveWidgetIndex(static_cast<int32>(EMenu::Main)); 

	SetButtonNormalStyle(LastClickedButton); 
	SetButtonNormalStyle(CurrentClickedButton); 

	LastClickedButton = nullptr; 
	CurrentClickedButton = nullptr; 
}

void UMenuWidget::OnCharacterClassButtonClicked(UCharacterSelectWidget* InWidget)
{
	CurrentClickedButton = InWidget->CharacterSelectButton; 

	if (!LastClickedButton) 
	{ 
		LastClickedButton = CurrentClickedButton;
	}
	else
	{
		SetButtonNormalStyle(LastClickedButton);
	}

	SetButtonNormalStyle(CurrentClickedButton, FLinearColor(1.f, 0.f, 0.f, 0.3f));

	LastClickedButton = CurrentClickedButton; 
}

void UMenuWidget::PopulateGrid()
{
	if (!CharacterGridPanel || !CharacterSelectWidgetClass) { return; }

	int32 Row = 0; 
	int32 Column = 0; 

	for (int i = 0; i < static_cast<int32>(ECharacterClass::_End); ++i)
	{
		UCharacterSelectWidget* CharacterSelectWidget = CreateWidget<UCharacterSelectWidget>(GetWorld(), CharacterSelectWidgetClass);

		bool bSetCharClassName = CharacterSelectWidget->SetCharacterClassWidget(static_cast<ECharacterClass>(i));

		if (!bSetCharClassName){check(false); return; }

		CharacterSelectWidget->CharacterSelectButtonClicked.BindUFunction(this, TEXT("OnCharacterClassButtonClicked")); 

		UUniformGridSlot* GridSlot = CharacterGridPanel->AddChildToUniformGrid(CharacterSelectWidget, Row, Column); 

		++Column; 
	}

}
